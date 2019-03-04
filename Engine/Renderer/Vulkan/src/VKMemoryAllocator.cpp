#include "VKRenderer.h"


#define NODE_INDEX(size) ((uint32_t)((size) / m_memoryAlignment) - 1)

CVKMemoryAllocator::CVKMemoryAllocator(CVKDevice *pDevice, uint32_t memoryTypeIndex, VkDeviceSize memorySize, VkDeviceSize memoryAlignment)
	: m_pDevice(pDevice)

	, m_root{nullptr}
	, m_nodes(nullptr)

	, m_vkMemory(VK_NULL_HANDLE)
	, m_memoryFreeSize(memorySize)
	, m_memoryFullSize(memorySize)
	, m_memoryAlignment(memoryAlignment)
	, m_memoryTypeIndex(memoryTypeIndex)
	, m_memoryPropertyFlags(pDevice->GetPhysicalDeviceMemoryProperties().memoryTypes[memoryTypeIndex].propertyFlags)

	, pNext(nullptr)
	, pPrev(nullptr)
{
	VkMemoryAllocateInfo allocateInfo = {};
	allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocateInfo.pNext = nullptr;
	allocateInfo.allocationSize = m_memoryFullSize;
	allocateInfo.memoryTypeIndex = m_memoryTypeIndex;
	vkAllocateMemory(m_pDevice->GetDevice(), &allocateInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkMemory);

	InitNodes();
	InsertMemory(new CVKMemory(this, m_pDevice, m_memoryFreeSize, 0));
}

CVKMemoryAllocator::~CVKMemoryAllocator(void)
{
	FreeNodes();
	vkFreeMemory(m_pDevice->GetDevice(), m_vkMemory, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
}

VkDeviceMemory CVKMemoryAllocator::GetMemory(void) const
{
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

VkDeviceSize CVKMemoryAllocator::GetAlignment(void) const
{
	return m_memoryAlignment;
}

uint32_t CVKMemoryAllocator::GetMemoryTypeIndex(void) const
{
	return m_memoryTypeIndex;
}

VkMemoryPropertyFlags CVKMemoryAllocator::GetMemoryPropertyFlags(void) const
{
	return m_memoryPropertyFlags;
}

CVKMemory* CVKMemoryAllocator::AllocMemory(VkDeviceSize size)
{
	//  Memory Pool
	//
	//
	//             Memory Handle 
	//             |                      Memory Size                      |
	//  -------------------------------------------------------------------
	// |           |                    |                     |            |
	// |    ...    |    Request Size    |   New Memory Size   |     ...    |
	// |___________|____________________|_____________________|____________|
	//             |                    |                     |
	//             Offset               |                     Next Memory Handle
	//             |                    New Memory Handle
	//             Allocated Memory Handle

	VkDeviceSize requestSize = ALIGN_BYTE(size, m_memoryAlignment);

	if (m_vkMemory != VK_NULL_HANDLE) {
		if (m_memoryFreeSize >= requestSize) {
			if (CVKMemory *pMemory = SearchMemory(requestSize)) {
				RemoveMemory(pMemory);

				if (pMemory->m_memorySize >= requestSize + m_memoryAlignment) {
					CVKMemory *pMemoryNext = new CVKMemory(this, m_pDevice, pMemory->m_memorySize - requestSize, pMemory->m_memoryOffset + requestSize);
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
	}

	return nullptr;
}

void CVKMemoryAllocator::FreeMemory(CVKMemory *pMemory)
{
	pMemory->bInUse = false;
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
	uint32_t numNodes = (uint32_t)(m_memoryFullSize / m_memoryAlignment);

	m_root = RB_ROOT;
	m_nodes = new mem_node* [numNodes];

	for (uint32_t indexNode = 0; indexNode < numNodes; indexNode++) {
		m_nodes[indexNode] = nullptr;
	}
}

void CVKMemoryAllocator::FreeNodes(void)
{
	uint32_t numNodes = (uint32_t)(m_memoryFullSize / m_memoryAlignment);

	for (uint32_t indexNode = 0; indexNode < numNodes; indexNode++) {
		if (m_nodes[indexNode]) {
			if (CVKMemory *pMemory = m_nodes[indexNode]->pListHead) {
				CVKMemory *pMemoryNext = nullptr;
				do {
					pMemoryNext = pMemory->pFreeNext;
					delete pMemory;
				} while (pMemory = pMemoryNext);
			}

			delete m_nodes[indexNode];
		}
	}

	delete[] m_nodes;
}

void CVKMemoryAllocator::InsertMemory(CVKMemory *pMemory)
{
	uint32_t indexNode = NODE_INDEX(pMemory->m_memorySize);

	if (m_nodes[indexNode] == nullptr) {
		m_nodes[indexNode] =  new mem_node(indexNode, m_memoryAlignment);
	}

	mem_node *pMemoryNode = m_nodes[indexNode];
	ASSERT(pMemoryNode->size == pMemory->m_memorySize);

	rb_node **node = &m_root.rb_node;
	rb_node *parent = nullptr;

	if (pMemoryNode->pListHead == nullptr) {
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

			ASSERT(false);
		}

		rb_init_node(&pMemoryNode->node);
		rb_link_node(&pMemoryNode->node, parent, node);
		rb_insert_color(&pMemoryNode->node, &m_root);

		pMemory->pFreeNext = nullptr;
		pMemory->pFreePrev = nullptr;
		pMemoryNode->pListHead = pMemory;
	}
	else {
		pMemory->pFreePrev = nullptr;
		pMemory->pFreeNext = pMemoryNode->pListHead;
		pMemoryNode->pListHead->pFreePrev = pMemory;
		pMemoryNode->pListHead = pMemory;
	}
}

void CVKMemoryAllocator::RemoveMemory(CVKMemory *pMemory)
{
	uint32_t indexNode = NODE_INDEX(pMemory->m_memorySize);

	mem_node *pMemoryNode = m_nodes[indexNode];
	ASSERT(pMemoryNode->size == pMemory->m_memorySize);

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
		delete pMemoryNode;

		m_nodes[indexNode] = nullptr;
	}
}

CVKMemory* CVKMemoryAllocator::MergeMemory(CVKMemory *pMemory, CVKMemory *pMemoryNext)
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
		ASSERT(pMemoryNode->pListHead->m_memorySize >= size);

		break;
	}

	return pMemoryNode ? pMemoryNode->pListHead : nullptr;
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
