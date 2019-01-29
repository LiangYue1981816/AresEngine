#include "VKRenderer.h"


CVKMemoryManager::CVKMemoryManager(CVKDevice *pDevice)
	: m_pDevice(pDevice)
{
	atomic_spin_init(&m_lock);
}

CVKMemoryManager::~CVKMemoryManager(void)
{
	for (const auto &itAligmentAllocator : m_pAllocatorListHeads) {
		for (const auto &itTypeAllocator : itAligmentAllocator.second) {
			if (CVKMemoryAllocator *pAllocator = itTypeAllocator.second) {
				CVKMemoryAllocator *pAllocatorNext = nullptr;
				do {
					pAllocatorNext = pAllocator->pNext;
					delete pAllocator;
				} while ((pAllocator = pAllocatorNext) != nullptr);
			}
		}
	}

	m_pAllocatorListHeads.clear();
}

uint32_t CVKMemoryManager::GetMemoryTypeIndex(const VkPhysicalDeviceMemoryProperties &memoryProperties, VkFlags memoryTypeBits, VkMemoryPropertyFlags &memoryPropertyFlags, VkDeviceSize memorySize)
{
	uint32_t indexTry = 0;

	do {
		for (uint32_t indexMemoryType = 0; indexMemoryType < memoryProperties.memoryTypeCount; indexMemoryType++) {
			if ((memoryTypeBits & (1 << indexMemoryType)) &&
				(memoryProperties.memoryTypes[indexMemoryType].propertyFlags & memoryPropertyFlags) == memoryPropertyFlags &&
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
			memoryPropertyFlags |=  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
			break;

		default:
			return 0xffffffff;
		}
	} while (true);
}

CVKMemory* CVKMemoryManager::AllocMemory(VkDeviceSize memorySize, VkDeviceSize memoryAlignment, VkFlags memoryTypeBits, VkMemoryPropertyFlags memoryPropertyFlags)
{
	uint32_t memoryTypeIndex = GetMemoryTypeIndex(m_pDevice->GetPhysicalDeviceMemoryProperties(), memoryTypeBits, memoryPropertyFlags, 0);
	if (memoryTypeIndex == 0xffffffff) return nullptr;

	atomic_spin_autolock autolock(&m_lock);
	{
		memoryAlignment = ALIGN_BYTE(memoryAlignment, 256); // The min alignment size is 256B
		memorySize = ALIGN_BYTE(memorySize, memoryAlignment);

		do {
			if (CVKMemoryAllocator *pAllocator = m_pAllocatorListHeads[memoryAlignment][memoryTypeIndex]) {
				do {
					if (CVKMemory *pMemory = pAllocator->AllocMemory(memorySize)) {
						return pMemory;
					}
				} while ((pAllocator = pAllocator->pNext) != nullptr);
			}

			VkDeviceSize memoryAllocatorSize = 0;
			{
				const VkDeviceSize MEMORY_POOL_ALIGNMENT = 8 * 1024 * 1024;
				const VkDeviceSize MEMORY_POOL_DEVICE_LOCAL_MEMORY_SIZE = 64 * 1024 * 1024;
				const VkDeviceSize MEMORY_POOL_HOST_VISIBLE_MEMORY_SIZE = 16 * 1024 * 1024;
				const VkDeviceSize MEMORY_POOL_HOST_VISIBLE_AND_DEVICE_LOCAL_MEMORY_SIZE = 8 * 1024 * 1024;

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
				memoryAllocatorSize = ALIGN_BYTE(memoryAllocatorSize, MEMORY_POOL_ALIGNMENT);

				memoryTypeIndex = GetMemoryTypeIndex(m_pDevice->GetPhysicalDeviceMemoryProperties(), memoryTypeBits, memoryPropertyFlags, memoryAllocatorSize);
				if (memoryTypeIndex == 0xffffffff) return nullptr;
			}

			CVKMemoryAllocator *pAllocator = new CVKMemoryAllocator(m_pDevice, memoryTypeIndex, memoryAllocatorSize, memoryAlignment);
			{
				if (m_pAllocatorListHeads[memoryAlignment][memoryTypeIndex]) {
					m_pAllocatorListHeads[memoryAlignment][memoryTypeIndex]->pPrev = pAllocator;
					pAllocator->pNext = m_pAllocatorListHeads[memoryAlignment][memoryTypeIndex];
				}

				m_pAllocatorListHeads[memoryAlignment][memoryTypeIndex] = pAllocator;
			}
			m_allocatedMemoryHeapSize[m_pDevice->GetPhysicalDeviceMemoryProperties().memoryTypes[memoryTypeIndex].heapIndex] += pAllocator->GetFullSize();
		} while (true);
	}

	return nullptr;
}

void CVKMemoryManager::FreeMemory(CVKMemory *pMemory)
{
	if (pMemory == nullptr) {
		return;
	}

	atomic_spin_autolock autolock(&m_lock);
	{
		CVKMemoryAllocator *pAllocator = pMemory->GetAllocator();
		pAllocator->FreeMemory(pMemory);

		if (pAllocator->GetFreeSize() == pAllocator->GetFullSize()) {
			uint32_t memoryAlignment = pAllocator->GetAlignment();
			uint32_t memoryTypeIndex = pAllocator->GetMemoryTypeIndex();

			m_allocatedMemoryHeapSize[m_pDevice->GetPhysicalDeviceMemoryProperties().memoryTypes[memoryTypeIndex].heapIndex] -= pAllocator->GetFullSize();
			{
				if (m_pAllocatorListHeads[memoryAlignment][memoryTypeIndex] == pAllocator) {
					m_pAllocatorListHeads[memoryAlignment][memoryTypeIndex] = pAllocator->pNext;
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
	atomic_spin_autolock autolock(&m_lock);
	{
		LogOutput(LOG_TAG_RENDERER, "MemoryManager:\n");

		for (const auto &itAligmentAllocator : m_pAllocatorListHeads) {
			for (const auto &itTypeAllocator : itAligmentAllocator.second) {
				if (CVKMemoryAllocator *pAllocator = itTypeAllocator.second) {
					do {
						LogOutput(LOG_TAG_RENDERER, "\tAllocator: type=%d alignment=%d free=%d full=%d device_local=%s host_visible=%s host_coherent=%s host_cached=%s\n",
							pAllocator->GetMemoryTypeIndex(),
							pAllocator->GetAlignment(),
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
}
