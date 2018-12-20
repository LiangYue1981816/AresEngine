#include "VKRenderer.h"


CVKMemoryAllocator::CVKMemoryAllocator(CVKDevice *pDevice, uint32_t memoryTypeIndex, VkDeviceSize memorySize, VkDeviceSize memoryAlignment)
	: m_pDevice(pDevice)

	, m_indexType(memoryTypeIndex)
	, m_freeSize(memorySize)
	, m_fullSize(memorySize)
	, m_alignment(memoryAlignment)

	, m_root{ nullptr }
	, m_nodes(nullptr)
	, m_pListHead(nullptr)

	, m_vkMemory(VK_NULL_HANDLE)

	, pNext(nullptr)
	, pPrev(nullptr)
{
	VkMemoryAllocateInfo allocateInfo = {};
	allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocateInfo.pNext = nullptr;
	allocateInfo.allocationSize = m_fullSize;
	allocateInfo.memoryTypeIndex = m_indexType;
	CALL_VK_FUNCTION_RETURN(vkAllocateMemory(m_pDevice->GetDevice(), &allocateInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkMemory));

	m_nodes = new mem_node[m_fullSize / m_alignment];
	m_pListHead = new CVKMemory(this, m_pDevice, m_vkMemory, m_pDevice->GetPhysicalDeviceMemoryProperties().memoryTypes[m_indexType].propertyFlags, m_alignment, 0, m_fullSize);
}

CVKMemoryAllocator::~CVKMemoryAllocator(void)
{
	if (m_nodes) {
		delete[] m_nodes;
	}

	if (m_pListHead) {
		delete m_pListHead;
	}

	if (m_vkMemory) {
		vkFreeMemory(m_pDevice->GetDevice(), m_vkMemory, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
	}
}

void CVKMemoryAllocator::InitNodes(uint32_t numNodes)
{

}

void CVKMemoryAllocator::InsertMemory(CVKMemory *pMemory)
{

}

void CVKMemoryAllocator::RemoveMemory(CVKMemory *pMemory)
{

}

CVKMemory* CVKMemoryAllocator::MergeMemory(CVKMemory *pMemory, CVKMemory *pMemoryNext)
{
	return nullptr;
}

CVKMemory* CVKMemoryAllocator::SearchMemory(VkDeviceSize size) const
{
	return nullptr;
}
