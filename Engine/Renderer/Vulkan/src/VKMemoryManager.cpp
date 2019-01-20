#include "VKRenderer.h"


static uint32_t GetMemoryTypeIndex(const VkPhysicalDeviceMemoryProperties &memoryProperties, VkFlags memoryTypeBits, VkMemoryPropertyFlags memoryPropertyFlags)
{
	for (uint32_t index = 0; index < 2; index++) {
		for (uint32_t indexMemoryType = 0; indexMemoryType < memoryProperties.memoryTypeCount; indexMemoryType++) {
			if ((memoryTypeBits & (1 << indexMemoryType)) &&
				(memoryProperties.memoryTypes[indexMemoryType].propertyFlags & memoryPropertyFlags) == memoryPropertyFlags) {
				return indexMemoryType;
			}
		}

		memoryPropertyFlags = memoryPropertyFlags & ~VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT;
	}

	return 0xffffffff;
}


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

CVKMemory* CVKMemoryManager::AllocMemory(VkDeviceSize memorySize, VkDeviceSize memoryAlignment, VkFlags memoryTypeBits, VkMemoryPropertyFlags memoryPropertyFlags)
{
	uint32_t memoryTypeIndex = GetMemoryTypeIndex(m_pDevice->GetPhysicalDeviceMemoryProperties(), memoryTypeBits, memoryPropertyFlags);
	if (memoryTypeIndex == 0xffffffff) return nullptr;

	atomic_spin_autolock autolock(&m_lock);
	{
		memoryAlignment = ALIGN_BYTE(memoryAlignment, 256);
		memorySize = ALIGN_BYTE(memorySize, memoryAlignment);

		do {
			if (CVKMemoryAllocator *pAllocator = m_pAllocatorListHeads[memoryAlignment][memoryTypeIndex]) {
				do {
					if (CVKMemory *pMemory = pAllocator->AllocMemory(memorySize)) {
						return pMemory;
					}
				} while ((pAllocator = pAllocator->pNext) != nullptr);
			}

			VkDeviceSize memoryPoolSize = 0;
			{
				const VkDeviceSize MEMORY_POOL_ALIGNMENT = 8 * 1024 * 1024;
				const VkDeviceSize MEMORY_POOL_DEVICE_LOCAL_MEMORY_SIZE = 64 * 1024 * 1024;
				const VkDeviceSize MEMORY_POOL_HOST_VISIBLE_MEMORY_SIZE = 16 * 1024 * 1024;
				const VkDeviceSize MEMORY_POOL_HOST_VISIBLE_AND_DEVICE_LOCAL_MEMORY_SIZE = 16 * 1024 * 1024;

				if ((memoryPropertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) == 0 && (memoryPropertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) != 0) {
					memoryPoolSize = MEMORY_POOL_DEVICE_LOCAL_MEMORY_SIZE;
				}

				if ((memoryPropertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) != 0 && (memoryPropertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) == 0) {
					memoryPoolSize = MEMORY_POOL_HOST_VISIBLE_MEMORY_SIZE;
				}

				if ((memoryPropertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) != 0 && (memoryPropertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) != 0) {
					memoryPoolSize = MEMORY_POOL_HOST_VISIBLE_AND_DEVICE_LOCAL_MEMORY_SIZE;
				}

				memoryPoolSize = std::max(memoryPoolSize, memorySize);
				memoryPoolSize = ALIGN_BYTE(memoryPoolSize, MEMORY_POOL_ALIGNMENT);
			}

			CVKMemoryAllocator *pAllocator = new CVKMemoryAllocator(m_pDevice, memoryTypeIndex, memoryPoolSize, memoryAlignment);
			{
				if (m_pAllocatorListHeads[memoryAlignment][memoryTypeIndex]) {
					m_pAllocatorListHeads[memoryAlignment][memoryTypeIndex]->pPrev = pAllocator;
					pAllocator->pNext = m_pAllocatorListHeads[memoryAlignment][memoryTypeIndex];
				}

				m_pAllocatorListHeads[memoryAlignment][memoryTypeIndex] = pAllocator;
			}
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

			if (m_pAllocatorListHeads[memoryAlignment][memoryTypeIndex] == pAllocator) {
				m_pAllocatorListHeads[memoryAlignment][memoryTypeIndex] =  pAllocator->pNext;
			}

			if (pAllocator->pPrev) {
				pAllocator->pPrev->pNext = pAllocator->pNext;
			}

			if (pAllocator->pNext) {
				pAllocator->pNext->pPrev = pAllocator->pPrev;
			}

			delete pAllocator;
		}
	}
}

void CVKMemoryManager::Log(void) const
{
	LogOutput(LOG_TAG_RENDERER, "MemoryManager:\n");

	for (const auto &itAligmentAllocator : m_pAllocatorListHeads) {
		for (const auto &itTypeAllocator : itAligmentAllocator.second) {
			if (CVKMemoryAllocator *pAllocator = itTypeAllocator.second) {
				do {
					LogOutput(LOG_TAG_RENDERER, "\tAllocator: alignment=%d free=%d full=%d type=%d device_local=%s host_visible=%s host_coherent=%s host_cached=%s\n",
						pAllocator->GetAlignment(),
						pAllocator->GetFreeSize(),
						pAllocator->GetFullSize(),
						pAllocator->GetMemoryTypeIndex(),
						pAllocator->IsDeviceLocal() ? "true" : "false",
						pAllocator->IsHostVisible() ? "true" : "false",
						pAllocator->IsHostCoherent() ? "true" : "false",
						pAllocator->IsHostCached() ? "true" : "false");
				} while ((pAllocator = pAllocator->pNext) != nullptr);
			}
		}
	}
}
