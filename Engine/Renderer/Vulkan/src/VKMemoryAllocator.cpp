#include "VKRenderer.h"


#define MIN_ALIGNMENT 64
#define NODE_INDEX(size) (((size) / MIN_ALIGNMENT) - 1)

CVKMemoryAllocator::CVKMemoryAllocator(CVKDevice *pDevice, uint32_t memoryTypeIndex, VkDeviceSize memorySize)
	: m_pDevice(pDevice)

	, m_indexType(memoryTypeIndex)
	, m_freeSize(ALIGN_BYTE(memorySize, MIN_ALIGNMENT))
	, m_fullSize(ALIGN_BYTE(memorySize, MIN_ALIGNMENT))

	, m_vkMemory(VK_NULL_HANDLE)

	, m_root{ nullptr }
	, m_nodes(nullptr)

	, pNext(nullptr)
	, pPrev(nullptr)
{
	VkMemoryAllocateInfo allocateInfo = {};
	allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocateInfo.pNext = nullptr;
	allocateInfo.allocationSize = m_fullSize;
	allocateInfo.memoryTypeIndex = m_indexType;
	CALL_VK_FUNCTION_RETURN(vkAllocateMemory(m_pDevice->GetDevice(), &allocateInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkMemory));

	InitNodes((uint32_t)(m_fullSize / MIN_ALIGNMENT));
	InsertMemory(new CVKMemory(this, m_pDevice, m_vkMemory, m_pDevice->GetPhysicalDeviceMemoryProperties().memoryTypes[m_indexType].propertyFlags, 0, 0, m_fullSize));
}

CVKMemoryAllocator::~CVKMemoryAllocator(void)
{
	FreeNodes((uint32_t)(m_fullSize / MIN_ALIGNMENT));

	if (m_vkMemory) {
		vkFreeMemory(m_pDevice->GetDevice(), m_vkMemory, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
	}
}

CVKMemory* CVKMemoryAllocator::AllocMemory(VkDeviceSize alignment, VkDeviceSize size)
{
	//  Device Memory
	//
	//
	//             Memory Handle 
	//             |                 Memory Size              |
	//  -------------------------------------------------------------------
	// |           |       |                |                 |            |
	// |    ...    |       |   RequestSize  | New Memory Size |     ...    |
	// |___________|_______|________________|_________________|____________|
	//             |       |                |                 |
	//             Offset  |                |                 Next Memory Handle
	//                     |                |
	//                     Alignment Offset |
	//                                      |
	//                                      New Memory Handle

	size = ALIGN_BYTE(size, MIN_ALIGNMENT);

	if (m_freeSize >= size) {
		if (CVKMemory *pMemory = SearchMemory(size)) {
			RemoveMemory(pMemory);

			if (pMemory->m_size >= size + MIN_ALIGNMENT) {
				CVKMemory *pMemoryNext = new CVKMemory(this, m_pDevice, m_vkMemory, m_pDevice->GetPhysicalDeviceMemoryProperties().memoryTypes[m_indexType].propertyFlags, pMemory->m_offset + size, 0, pMemory->m_size - size);
				{
					pMemoryNext->pNext = pMemory->pNext;
					pMemoryNext->pPrev = pMemory;
					pMemory->pNext = pMemoryNext;

					if (pMemoryNext->pNext) {
						pMemoryNext->pNext->pPrev = pMemoryNext;
					}

					InsertMemory(pMemoryNext);
				}

				pMemory->m_size = size;
			}

			pMemory->bInUse = true;
			m_freeSize -= pMemory->m_size;

			return pMemory;
		}
	}

	return nullptr;
}

void CVKMemoryAllocator::FreeMemory(CVKMemory *pMemory)
{
	pMemory->bInUse = false;
	m_freeSize += pMemory->m_size;

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

void CVKMemoryAllocator::InitNodes(uint32_t numNodes)
{
	m_root = RB_ROOT;
	m_nodes = new mem_node[numNodes];

	for (uint32_t indexNode = 0; indexNode < numNodes; indexNode++) {
		m_nodes[indexNode].size = (indexNode + 1) * MIN_ALIGNMENT;
		m_nodes[indexNode].pListHead = nullptr;
	}
}

void CVKMemoryAllocator::FreeNodes(uint32_t numNodes)
{
	if (m_nodes) {
		for (uint32_t indexNode = 0; indexNode < numNodes; indexNode++) {
			if (CVKMemory *pMemory = m_nodes[indexNode].pListHead) {
				CVKMemory *pMemoryNext = nullptr;
				do {
					pMemoryNext = pMemory->pFreeNext;
					delete pMemory;
				} while  ((pMemory = pMemoryNext) != nullptr);
			}
		}

		delete m_nodes;
	}
}

void CVKMemoryAllocator::InsertMemory(CVKMemory *pMemory)
{
	ASSERT(pMemory->bInUse == false);

	mem_node *pMemoryNode = &m_nodes[NODE_INDEX(pMemory->m_size)];
	rb_node **node = &m_root.rb_node;
	rb_node *parent = nullptr;

	while (*node) {
		mem_node *pMemoryNodeCur = container_of(*node, mem_node, node);

		parent = *node;

		if (pMemoryNode->size > pMemoryNodeCur->size) {
			node = &(*node)->rb_right;
			continue;
		}

		if (pMemoryNode->size < pMemoryNodeCur->size) {
			node = &(*node)->rb_left;
			continue;
		}

		ASSERT(pMemoryNode == pMemoryNodeCur);

		pMemory->pFreePrev = nullptr;
		pMemory->pFreeNext = pMemoryNode->pListHead;
		pMemoryNode->pListHead->pFreePrev = pMemory;
		pMemoryNode->pListHead = pMemory;

		return;
	}

	pMemory->pFreeNext = nullptr;
	pMemory->pFreePrev = nullptr;
	pMemoryNode->pListHead = pMemory;

	rb_init_node(&pMemoryNode->node);
	rb_link_node(&pMemoryNode->node, parent, node);
	rb_insert_color(&pMemoryNode->node, &m_root);
}

void CVKMemoryAllocator::RemoveMemory(CVKMemory *pMemory)
{
	ASSERT(pMemory->bInUse == false);
	mem_node *pMemoryNode = &m_nodes[NODE_INDEX(pMemory->m_size)];

	if (pMemory->pFreeNext) {
		pMemory->pFreeNext->pFreePrev = pMemory->pFreePrev;
	}

	if (pMemory->pFreePrev) {
		pMemory->pFreePrev->pFreeNext = pMemory->pFreeNext;
	}

	if (pMemoryNode->pListHead == pMemory) {
		pMemoryNode->pListHead =  pMemory->pFreeNext;
	}

	if (pMemoryNode->pListHead == nullptr) {
		rb_erase(&pMemoryNode->node, &m_root);
	}
}

CVKMemory* CVKMemoryAllocator::MergeMemory(CVKMemory *pMemory, CVKMemory *pMemoryNext)
{
	ASSERT(pMemory->m_offset + pMemory->m_size == pMemoryNext->m_offset);

	pMemory->m_size = pMemory->m_size + pMemoryNext->m_size;
	pMemory->pNext = pMemoryNext->pNext;

	if (pMemoryNext->pNext) {
		pMemoryNext->pNext->pPrev = pMemory;
	}

	delete pMemoryNext;
	return pMemory;
}

CVKMemory* CVKMemoryAllocator::SearchMemory(VkDeviceSize size) const
{
	mem_node *pMemoryNode = nullptr;
	rb_node *node = m_root.rb_node;

	while (node) {
		mem_node *pMemoryNodeCur = container_of(node, mem_node, node);

		if (size > pMemoryNodeCur->size) {
			node = node->rb_right;
			continue;
		}

		pMemoryNode = pMemoryNodeCur;

		if (size < pMemoryNodeCur->size) {
			node = node->rb_left;
			continue;
		}

		ASSERT(pMemoryNode->pListHead);
		ASSERT(pMemoryNode->pListHead->bInUse == false);
		ASSERT(pMemoryNode->pListHead->m_size / MIN_ALIGNMENT * MIN_ALIGNMENT >= size);

		break;
	}

	return pMemoryNode ? pMemoryNode->pListHead : nullptr;
}

bool CVKMemoryAllocator::IsDeviceLocal(void) const
{
	return m_pDevice->GetPhysicalDeviceMemoryProperties().memoryTypes[m_indexType].propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT ? true : false;
}

bool CVKMemoryAllocator::IsHostVisible(void) const
{
	return m_pDevice->GetPhysicalDeviceMemoryProperties().memoryTypes[m_indexType].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT ? true : false;
}

bool CVKMemoryAllocator::IsHostCoherent(void) const
{
	return m_pDevice->GetPhysicalDeviceMemoryProperties().memoryTypes[m_indexType].propertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT ? true : false;
}

bool CVKMemoryAllocator::IsHostCached(void) const
{
	return m_pDevice->GetPhysicalDeviceMemoryProperties().memoryTypes[m_indexType].propertyFlags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT ? true : false;
}

bool CVKMemoryAllocator::IsLazilyAllocated(void) const
{
	return m_pDevice->GetPhysicalDeviceMemoryProperties().memoryTypes[m_indexType].propertyFlags & VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT ? true : false;
}

uint32_t CVKMemoryAllocator::GetMemoryTypeIndex(void) const
{
	return m_indexType;
}

VkDeviceSize CVKMemoryAllocator::GetFreeSize(void) const
{
	return m_freeSize;
}

VkDeviceSize CVKMemoryAllocator::GetFullSize(void) const
{
	return m_fullSize;
}
