#include "VKRenderer.h"


CVKMemoryManager::CVKMemoryManager(CVKDevice* pDevice)
	: m_pDevice(pDevice)
{
	pthread_mutex_init(&lock, nullptr);
}

CVKMemoryManager::~CVKMemoryManager(void)
{
	for (const auto& itAllocator : m_pAllocatorListHeads) {
		if (CVKMemoryAllocator* pAllocator = itAllocator.second) {
			CVKMemoryAllocator* pAllocatorNext = nullptr;
			do {
				pAllocatorNext = pAllocator->pNext;
				delete pAllocator;
			} while ((pAllocator = pAllocatorNext) != nullptr);
		}
	}

	pthread_mutex_destroy(&lock);
}

uint32_t CVKMemoryManager::GetMemoryTypeIndex(const VkPhysicalDeviceMemoryProperties& memoryProperties, VkMemoryPropertyFlags& memoryPropertyFlags, VkDeviceSize memorySize)
{
	uint32_t indexTry = 0;

	do {
		for (uint32_t indexMemoryType = 0; indexMemoryType < memoryProperties.memoryTypeCount; indexMemoryType++) {
			if ((memoryProperties.memoryTypes[indexMemoryType].propertyFlags & memoryPropertyFlags) == memoryPropertyFlags &&
				(memoryProperties.memoryHeaps[memoryProperties.memoryTypes[indexMemoryType].heapIndex].size * 80 / 100) > (m_allocatedMemoryHeapSize[memoryProperties.memoryTypes[indexMemoryType].heapIndex] + memorySize)) {
				return indexMemoryType;
			}
		}

		switch (indexTry) {
		case 0:
			memoryPropertyFlags &= ~VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT;
			break;

		case 1:
			memoryPropertyFlags &= ~VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
			memoryPropertyFlags |= VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
			break;

		default:
			return INVALID_VALUE;
		}
	} while (true);
}

CVKMemory* CVKMemoryManager::AllocMemory(VkDeviceSize memorySize, VkMemoryPropertyFlags memoryPropertyFlags, VkResourceType type)
{
	return AllocMemory(memorySize, 1, memoryPropertyFlags, type);
}

CVKMemory* CVKMemoryManager::AllocMemory(VkDeviceSize memorySize, VkDeviceSize memoryAlignment, VkMemoryPropertyFlags memoryPropertyFlags, VkResourceType type)
{
	uint32_t memoryTypeIndex = GetMemoryTypeIndex(m_pDevice->GetPhysicalDeviceMemoryProperties(), memoryPropertyFlags, 0);
	ASSERT(memoryTypeIndex != INVALID_VALUE);

	mutex_autolock autolock(&lock);
	{
		if (memoryPropertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) {
			memoryAlignment = ALIGN_BYTE(memoryAlignment, m_pDevice->GetPhysicalDeviceLimits().nonCoherentAtomSize);
		}

		memoryAlignment = ALIGN_BYTE(memoryAlignment, 256);
		memorySize = ALIGN_BYTE(memorySize, memoryAlignment);

		do {
			if (CVKMemoryAllocator* pAllocator = m_pAllocatorListHeads[memoryTypeIndex]) {
				do {
					if (CVKMemory* pMemory = pAllocator->AllocMemory(memorySize, memoryAlignment, type)) {
						return pMemory;
					}
				} while ((pAllocator = pAllocator->pNext) != nullptr);
			}

			VkDeviceSize memoryAllocatorSize = 0;
			{
				const VkDeviceSize MEMORY_POOL_DEVICE_LOCAL_MEMORY_SIZE = 32 * 1024 * 1024;
				const VkDeviceSize MEMORY_POOL_HOST_VISIBLE_MEMORY_SIZE = 32 * 1024 * 1024;
				const VkDeviceSize MEMORY_POOL_HOST_VISIBLE_AND_DEVICE_LOCAL_MEMORY_SIZE = 16 * 1024 * 1024;

				if ((memoryPropertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) == 0 && (memoryPropertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) != 0) {
					memoryAllocatorSize = MEMORY_POOL_DEVICE_LOCAL_MEMORY_SIZE;
				}

				if ((memoryPropertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) != 0 && (memoryPropertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) == 0) {
					memoryAllocatorSize = MEMORY_POOL_HOST_VISIBLE_MEMORY_SIZE;
				}

				if ((memoryPropertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) != 0 && (memoryPropertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) != 0) {
					memoryAllocatorSize = MEMORY_POOL_HOST_VISIBLE_AND_DEVICE_LOCAL_MEMORY_SIZE;
				}

				memoryAllocatorSize = std::max(memoryAllocatorSize, memorySize);
				memoryTypeIndex = GetMemoryTypeIndex(m_pDevice->GetPhysicalDeviceMemoryProperties(), memoryPropertyFlags, memoryAllocatorSize);
				if (memoryTypeIndex == INVALID_VALUE) return nullptr;
			}

			CVKMemoryAllocator* pAllocator = new CVKMemoryAllocator(m_pDevice, memoryTypeIndex, memoryAllocatorSize);
			{
				if (m_pAllocatorListHeads[memoryTypeIndex]) {
					m_pAllocatorListHeads[memoryTypeIndex]->pPrev = pAllocator;
					pAllocator->pNext = m_pAllocatorListHeads[memoryTypeIndex];
				}

				m_pAllocatorListHeads[memoryTypeIndex] = pAllocator;
			}
			m_allocatedMemoryHeapSize[m_pDevice->GetPhysicalDeviceMemoryProperties().memoryTypes[memoryTypeIndex].heapIndex] += pAllocator->GetFullSize();
		} while (true);
	}

	return nullptr;
}

void CVKMemoryManager::FreeMemory(CVKMemory* pMemory)
{
	mutex_autolock autolock(&lock);
	{
		ASSERT(pMemory);
		ASSERT(pMemory->GetAllocator());

		CVKMemoryAllocator* pAllocator = pMemory->GetAllocator();
		pAllocator->FreeMemory(pMemory);

		if (pAllocator->GetFreeSize() == pAllocator->GetFullSize()) {
			uint32_t memoryTypeIndex = pAllocator->GetMemoryTypeIndex();

			m_allocatedMemoryHeapSize[m_pDevice->GetPhysicalDeviceMemoryProperties().memoryTypes[memoryTypeIndex].heapIndex] -= pAllocator->GetFullSize();
			{
				if (m_pAllocatorListHeads[memoryTypeIndex] == pAllocator) {
					m_pAllocatorListHeads[memoryTypeIndex] = pAllocator->pNext;
				}

				if (pAllocator->pPrev) {
					pAllocator->pPrev->pNext = pAllocator->pNext;
				}

				if (pAllocator->pNext) {
					pAllocator->pNext->pPrev = pAllocator->pPrev;
				}
			}
			delete pAllocator;
		}
	}
}

void CVKMemoryManager::Log(void)
{
	mutex_autolock autolock(&lock);
	{
		LogOutput(LOG_TAG_RENDERER, "MemoryManager:\n");

		for (const auto& itAllocator : m_pAllocatorListHeads) {
			if (CVKMemoryAllocator* pAllocator = itAllocator.second) {
				do {
					LogOutput(LOG_TAG_RENDERER, "\tAllocator: type=%d free=%d full=%d device_local=%s host_visible=%s host_coherent=%s host_cached=%s\n",
						pAllocator->GetMemoryTypeIndex(),
						pAllocator->GetFreeSize(),
						pAllocator->GetFullSize(),
						pAllocator->IsDeviceLocal() ? "true" : "false",
						pAllocator->IsHostVisible() ? "true" : "false",
						pAllocator->IsHostCoherent() ? "true" : "false",
						pAllocator->IsHostCached() ? "true" : "false");
				} while ((pAllocator = pAllocator->pNext) != nullptr);
			}
		}
	}
}
