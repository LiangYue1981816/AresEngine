#include "VKRenderer.h"


static uint32_t GetMemoryTypeIndex(const VkPhysicalDeviceMemoryProperties &memoryProperties, VkFlags memoryTypeBits, VkMemoryPropertyFlags memoryPropertyFlags)
{
	for (uint32_t indexMemoryType = 0; indexMemoryType < memoryProperties.memoryTypeCount; indexMemoryType++) {
		if ((memoryTypeBits & (1 << indexMemoryType)) &&
			(memoryProperties.memoryTypes[indexMemoryType].propertyFlags & memoryPropertyFlags) == memoryPropertyFlags) {
			return indexMemoryType;
		}
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

}

CVKMemory* CVKMemoryManager::AllocMemory(VkDeviceSize memorySize, VkDeviceSize memoryAlignment, VkFlags memoryTypeBits, VkMemoryPropertyFlags memoryPropertyFlags)
{
	uint32_t memoryTypeIndex = GetMemoryTypeIndex(m_pDevice->GetPhysicalDeviceMemoryProperties(), memoryTypeBits, memoryPropertyFlags);
	if (memoryTypeIndex == 0xffffffff) return nullptr;

	atomic_spin_autolock autolock(&m_lock);
	{
		do {
			if (CVKMemoryAllocator *pAllocator = m_pAllocatorListHeads[memoryTypeIndex][memoryAlignment]) {
				do {
					if (CVKMemory *pMemory = pAllocator->AllocMemory(memorySize)) {
						return pMemory;
					}
				} while ((pAllocator = pAllocator->pNext) != nullptr);
			}

			VkDeviceSize allocatorMemorySize = 0;
			{
				const VkDeviceSize ALLOCATOR_DEVICE_LOCAL_MEMORY_SIZE = 256 * 1024 * 1024;
				const VkDeviceSize ALLOCATOR_HOST_VISIBLE_MEMORY_SIZE = 256 * 1024 * 1024;
				const VkDeviceSize ALLOCATOR_HOST_VISIBLE_AND_DEVICE_LOCAL_MEMORY_SIZE = 64 * 1024 * 1024;

				if ((memoryPropertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) == 0 && (memoryPropertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) != 0) {
					allocatorMemorySize = ALLOCATOR_DEVICE_LOCAL_MEMORY_SIZE;
				}

				if ((memoryPropertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) != 0 && (memoryPropertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) == 0) {
					allocatorMemorySize = ALLOCATOR_HOST_VISIBLE_MEMORY_SIZE;
				}

				if ((memoryPropertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) != 0 && (memoryPropertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) != 0) {
					allocatorMemorySize = ALLOCATOR_HOST_VISIBLE_AND_DEVICE_LOCAL_MEMORY_SIZE;
				}

				allocatorMemorySize = std::max(allocatorMemorySize, memorySize);
			}

			CVKMemoryAllocator *pAllocator = new CVKMemoryAllocator(m_pDevice, memoryTypeIndex, memoryAlignment, allocatorMemorySize);
			{
				if (m_pAllocatorListHeads[memoryTypeIndex][memoryAlignment]) {
					m_pAllocatorListHeads[memoryTypeIndex][memoryAlignment]->pPrev = pAllocator;
					pAllocator->pNext = m_pAllocatorListHeads[memoryTypeIndex][memoryAlignment];
				}

				m_pAllocatorListHeads[memoryTypeIndex][memoryAlignment] = pAllocator;
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
			uint32_t memoryAlignment = pAllocator->GetMemoryAlignment();
			uint32_t memoryTypeIndex = pAllocator->GetMemoryTypeIndex();

			if (m_pAllocatorListHeads[memoryTypeIndex][memoryAlignment] == pAllocator) {
				m_pAllocatorListHeads[memoryTypeIndex][memoryAlignment]  = pAllocator->pNext;
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

	for (const auto &itTypeAllocator : m_pAllocatorListHeads) {
		for (const auto &itAligmentAllocator : itTypeAllocator.second) {
			if (const CVKMemoryAllocator *pAllocator = itAligmentAllocator.second) {
				do {
					LogOutput(LOG_TAG_RENDERER, "\tAllocator: free=%d full=%d type=%d aligment=%d device_local=%s host_visible=%s host_coherent=%s host_cached=%s\n",
						pAllocator->GetFreeSize(),
						pAllocator->GetFullSize(),
						pAllocator->GetMemoryTypeIndex(),
						pAllocator->GetMemoryAlignment(),
						pAllocator->IsDeviceLocal()  ? "true" : "false",
						pAllocator->IsHostVisible()  ? "true" : "false",
						pAllocator->IsHostCoherent() ? "true" : "false",
						pAllocator->IsHostCached()   ? "true" : "false");
				} while (pAllocator = pAllocator->pNext);
			}
		}
	}
}
