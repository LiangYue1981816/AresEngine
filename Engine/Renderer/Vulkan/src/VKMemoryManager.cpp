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

			VkDeviceSize ALLOCATOR_MEMORY_SIZE = 4 * 1024 * 1024;
			VkDeviceSize allocatorMemorySize = std::max(ALLOCATOR_MEMORY_SIZE, memorySize);
			CVKMemoryAllocator *pAllocator = new CVKMemoryAllocator(m_pDevice, memoryTypeIndex, memoryAlignment, allocatorMemorySize);

			if (m_pAllocatorListHeads[memoryTypeIndex][memoryAlignment]) {
				m_pAllocatorListHeads[memoryTypeIndex][memoryAlignment]->pPrev = pAllocator;
				pAllocator->pNext = m_pAllocatorListHeads[memoryTypeIndex][memoryAlignment];
			}

			m_pAllocatorListHeads[memoryTypeIndex][memoryAlignment] = pAllocator;
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

}
