#include "VKRenderer.h"


CVKMemoryAllocator::CVKMemoryAllocator(CVKDevice* pDevice, uint32_t memoryTypeIndex, VkDeviceSize memorySize)
	: m_pDevice(pDevice)

	, m_pMemoryAddress(nullptr)
	, m_vkMemory(VK_NULL_HANDLE)

	, m_memoryFreeSize(0)
	, m_memoryFullSize(0)
	, m_memoryTypeIndex(0)
	, m_memoryPropertyFlags(0)

	, pNext(nullptr)
	, pPrev(nullptr)
{
	Create(memoryTypeIndex, memorySize);
}

CVKMemoryAllocator::~CVKMemoryAllocator(void)
{
	Destroy();
}

bool CVKMemoryAllocator::Create(uint32_t memoryTypeIndex, VkDeviceSize memorySize)
{
	ASSERT(memorySize);
	ASSERT(memoryTypeIndex != INVALID_VALUE);

	m_memoryFreeSize = memorySize;
	m_memoryFullSize = memorySize;
	m_memoryTypeIndex = memoryTypeIndex;
	m_memoryPropertyFlags = m_pDevice->GetPhysicalDeviceMemoryProperties().memoryTypes[memoryTypeIndex].propertyFlags;

	VkMemoryAllocateInfo allocateInfo = {};
	allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocateInfo.pNext = nullptr;
	allocateInfo.allocationSize = memorySize;
	allocateInfo.memoryTypeIndex = memoryTypeIndex;
	CALL_VK_FUNCTION_RETURN_BOOL(vkAllocateMemory(m_pDevice->GetDevice(), &allocateInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkMemory));

	if (IsHostVisible()) {
		vkMapMemory(m_pDevice->GetDevice(), m_vkMemory, 0, VK_WHOLE_SIZE, 0, &m_pMemoryAddress);
	}

	InitNodes();
	InsertMemory(new CVKMemory(m_pDevice, this, memorySize, 0));

	return true;
}

void CVKMemoryAllocator::Destroy(void)
{
	ASSERT(m_vkMemory);

	FreeNodes();

	if (IsHostVisible()) {
		vkUnmapMemory(m_pDevice->GetDevice(), m_vkMemory);
	}

	vkFreeMemory(m_pDevice->GetDevice(), m_vkMemory, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());

	m_pMemoryAddress = nullptr;
	m_vkMemory = VK_NULL_HANDLE;

	m_memoryFreeSize = 0;
	m_memoryFullSize = 0;
	m_memoryTypeIndex = 0;
	m_memoryPropertyFlags = 0;
}

void* CVKMemoryAllocator::GetMemoryAddress(void) const
{
	ASSERT(m_pMemoryAddress);
	return m_pMemoryAddress;
}

VkDeviceMemory CVKMemoryAllocator::GetMemory(void) const
{
	ASSERT(m_vkMemory);
	return m_vkMemory;
}

VkDeviceSize CVKMemoryAllocator::GetFreeSize(void) const
{
	return m_memoryFreeSize;
}

VkDeviceSize CVKMemoryAllocator::GetFullSize(void) const
{
	return m_memoryFullSize;
}

uint32_t CVKMemoryAllocator::GetMemoryTypeIndex(void) const
{
	return m_memoryTypeIndex;
}

VkMemoryPropertyFlags CVKMemoryAllocator::GetMemoryPropertyFlags(void) const
{
	return m_memoryPropertyFlags;
}

CVKMemory* CVKMemoryAllocator::AllocMemory(VkDeviceSize size, VkDeviceSize alignment)
{
	//  Memory Pool
	//
	//
	//             Memory Handle 
	//             |                         Memory Size                         |
	//  -------------------------------------------------------------------------
	// |           |         |                |                     |            |
	// |    ...    | Padding |  Request Size  |   New Memory Size   |     ...    |
	// |___________|_________|________________|_____________________|____________|
	//             |         |                |                     |
	//             Offset    |                |                     Next Memory Handle
	//             |         |                New Memory Handle
	//             |         Alignment Offset
	//             |
	//             Allocated Memory Handle
	ASSERT(m_vkMemory);

	if (m_memoryFreeSize >= size) {
		if (CVKMemory* pMemory = SearchMemory(size, alignment)) {
			RemoveMemory(pMemory);

			VkDeviceSize alignmentOffset = ALIGN_BYTE(pMemory->m_memoryOffset, alignment);
			VkDeviceSize padding = alignmentOffset - pMemory->m_memoryOffset;

			if (pMemory->m_memorySize - padding > size) {

			}

			pMemory->bInUse = true;
			pMemory->m_memoryPadding = padding;
			m_memoryFreeSize -= pMemory->m_memorySize;

			return pMemory;
		}
	}

	return nullptr;
	/*
	VkDeviceSize requestSize = ALIGN_BYTE(size, m_memoryAlignment);

	if (m_memoryFreeSize >= requestSize) {
		if (CVKMemory* pMemory = SearchMemory(requestSize)) {
			RemoveMemory(pMemory);

			if (pMemory->m_memorySize >= requestSize + m_memoryAlignment) {
				CVKMemory* pMemoryNext = new CVKMemory(m_pDevice, this, pMemory->m_memorySize - requestSize, pMemory->m_memoryOffset + requestSize);
				{
					pMemoryNext->pNext = pMemory->pNext;
					pMemoryNext->pPrev = pMemory;
					pMemory->pNext = pMemoryNext;

					if (pMemoryNext->pNext) {
						pMemoryNext->pNext->pPrev = pMemoryNext;
					}

					InsertMemory(pMemoryNext);
				}

				pMemory->m_memorySize = requestSize;
			}

			pMemory->bInUse = true;
			m_memoryFreeSize -= pMemory->m_memorySize;

			return pMemory;
		}
	}

	return nullptr;
	*/
}

void CVKMemoryAllocator::FreeMemory(CVKMemory* pMemory)
{
	ASSERT(pMemory);
	ASSERT(pMemory->bInUse);
	ASSERT(pMemory->pMemoryNode == nullptr);
	ASSERT(m_vkMemory);

	pMemory->bInUse = false;
	pMemory->m_memoryPadding = 0;
	m_memoryFreeSize += pMemory->m_memorySize;

	if (pMemory->pNext && pMemory->pNext->bInUse == false) {
		RemoveMemory(pMemory->pNext);
		pMemory = MergeMemory(pMemory, pMemory->pNext);
	}

	if (pMemory->pPrev && pMemory->pPrev->bInUse == false) {
		RemoveMemory(pMemory->pPrev);
		pMemory = MergeMemory(pMemory->pPrev, pMemory);
	}

	InsertMemory(pMemory);
}

void CVKMemoryAllocator::InitNodes(void)
{
	m_root = RB_ROOT;
}

void CVKMemoryAllocator::FreeNodes(void)
{
	for (const auto& itNode : m_nodes) {
		delete itNode.first->pMemory;
		delete itNode.first;
	}

	m_root = RB_ROOT;
	m_nodes.clear();
}

void CVKMemoryAllocator::InsertMemory(CVKMemory* pMemory)
{
	ASSERT(pMemory);
	ASSERT(pMemory->bInUse == false);
	ASSERT(pMemory->pMemoryNode == nullptr);

	mem_node* pMemoryNode = new mem_node(pMemory);

	rb_node*  parent = nullptr;
	rb_node** node = &m_root.rb_node;

	while (*node) {
		mem_node* pMemoryNodeCur = container_of(*node, mem_node, node);

		parent = *node;

		if (pMemoryNodeCur->pMemory->m_memorySize < pMemoryNode->pMemory->m_memorySize) {
			node = &(*node)->rb_right;
			continue;
		}
		else {
			node = &(*node)->rb_left;
			continue;
		}
	}

	rb_init_node(&pMemoryNode->node);
	rb_link_node(&pMemoryNode->node, parent, node);
	rb_insert_color(&pMemoryNode->node, &m_root);

	m_nodes[pMemoryNode] = pMemoryNode->pMemory->m_memorySize;
}

void CVKMemoryAllocator::RemoveMemory(CVKMemory* pMemory)
{
	ASSERT(pMemory);
	ASSERT(pMemory->bInUse == false);
	ASSERT(pMemory->pMemoryNode != nullptr);

	rb_erase(&pMemory->pMemoryNode->node, &m_root);
	m_nodes.erase(pMemory->pMemoryNode);

	delete pMemory->pMemoryNode;
}

CVKMemory* CVKMemoryAllocator::MergeMemory(CVKMemory* pMemory, CVKMemory* pMemoryNext)
{
	ASSERT(pMemory->m_memoryOffset + pMemory->m_memorySize == pMemoryNext->m_memoryOffset);

	pMemory->m_memorySize = pMemoryNext->m_memorySize + pMemory->m_memorySize;
	pMemory->pNext = pMemoryNext->pNext;

	if (pMemoryNext->pNext) {
		pMemoryNext->pNext->pPrev = pMemory;
	}

	delete pMemoryNext;
	return pMemory;
}

CVKMemory* CVKMemoryAllocator::SearchMemory(VkDeviceSize size, VkDeviceSize alignment) const
{
	mem_node* pMemoryNode = nullptr;
	rb_node*  node = m_root.rb_node;

	while (node) {
		mem_node* pMemoryNodeCur = container_of(node, mem_node, node);

		ASSERT(pMemoryNodeCur);
		ASSERT(pMemoryNodeCur->pMemory);

		// Memory Block
		//
		// |             Memory Size             |
		// |-------------------------------------|
		// |         |                           |
		// | Padding |        Valid Size         |
		// |_________|___________________________|
		// |         |
		// Offset    |
		//           AlignmentOffset

		VkDeviceSize alignmentOffset = ALIGN_BYTE(pMemoryNodeCur->pMemory->m_memoryOffset, alignment);
		VkDeviceSize padding = alignmentOffset - pMemoryNodeCur->pMemory->m_memoryOffset;

		if (pMemoryNodeCur->pMemory->m_memorySize - padding < size) {
			node = node->rb_right;
			continue;
		}

		pMemoryNode = pMemoryNodeCur;

		if (pMemoryNodeCur->pMemory->m_memorySize - padding > size) {
			node = node->rb_left;
			continue;
		}

		break;
	}

	return pMemoryNode ? pMemoryNode->pMemory : nullptr;
}

bool CVKMemoryAllocator::IsDeviceLocal(void) const
{
	return (m_memoryPropertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) != 0;
}

bool CVKMemoryAllocator::IsHostVisible(void) const
{
	return (m_memoryPropertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) != 0;
}

bool CVKMemoryAllocator::IsHostCoherent(void) const
{
	return (m_memoryPropertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) != 0;
}

bool CVKMemoryAllocator::IsHostCached(void) const
{
	return (m_memoryPropertyFlags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT) != 0;
}

bool CVKMemoryAllocator::IsLazilyAllocated(void) const
{
	return (m_memoryPropertyFlags & VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT) != 0;
}
