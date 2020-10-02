#include "VKRenderer.h"


CVKMemoryAllocator::CVKMemoryAllocator(CVKDevice* pDevice, uint32_t memoryTypeIndex, VkDeviceSize memorySize)
	: m_pDevice(pDevice)

	, m_pMemoryAddress(nullptr)
	, m_vkMemory(VK_NULL_HANDLE)

	, m_memoryFreeSize(memorySize)
	, m_memoryFullSize(memorySize)
	, m_memoryPropertyFlags(pDevice->GetPhysicalDeviceMemoryProperties().memoryTypes[memoryTypeIndex].propertyFlags)
{
	VkMemoryAllocateInfo allocateInfo = {};
	allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocateInfo.pNext = nullptr;
	allocateInfo.allocationSize = memorySize;
	allocateInfo.memoryTypeIndex = memoryTypeIndex;
	CALL_VK_FUNCTION_RETURN(vkAllocateMemory(m_pDevice->GetDevice(), &allocateInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkMemory));

	InitNodes();
	InsertMemory(new CVKMemory(m_pDevice, this, memorySize, 0));

	if (IsHostVisible()) {
		vkMapMemory(m_pDevice->GetDevice(), m_vkMemory, 0, VK_WHOLE_SIZE, 0, &m_pMemoryAddress);
	}
}

CVKMemoryAllocator::~CVKMemoryAllocator(void)
{
	if (IsHostVisible()) {
		vkUnmapMemory(m_pDevice->GetDevice(), m_vkMemory);
	}

	FreeNodes();

	vkFreeMemory(m_pDevice->GetDevice(), m_vkMemory, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
}

void* CVKMemoryAllocator::GetMemoryAddress(void) const
{
	return m_pMemoryAddress;
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

VkDeviceSize CVKMemoryAllocator::GetCommitmentSize(void) const
{
	VkDeviceSize size;
	vkGetDeviceMemoryCommitment(m_pDevice->GetDevice(), m_vkMemory, &size);
	return size;
}

VkMemoryPropertyFlags CVKMemoryAllocator::GetMemoryPropertyFlags(void) const
{
	return m_memoryPropertyFlags;
}

CVKMemory* CVKMemoryAllocator::AllocMemory(VkDeviceSize size, VkDeviceSize alignment, VkResourceType type)
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

	if (m_memoryFreeSize >= size) {
		VkDeviceSize offset;
		VkDeviceSize padding;

		if (CVKMemory* pMemory = SearchMemory(size, alignment, type, offset, padding)) {
			RemoveMemory(pMemory);

			if (pMemory->m_memorySize > padding + size) {
				CVKMemory* pMemoryNext = new CVKMemory(m_pDevice, this, pMemory->m_memorySize - (padding + size), pMemory->m_memoryOffset + padding + size);
				{
					pMemoryNext->pNext = pMemory->pNext;
					pMemoryNext->pPrev = pMemory;
					pMemory->pNext = pMemoryNext;

					if (pMemoryNext->pNext) {
						pMemoryNext->pNext->pPrev = pMemoryNext;
					}

					InsertMemory(pMemoryNext);
				}

				pMemory->m_memorySize = padding + size;
			}

			pMemory->type = type;
			pMemory->m_memoryPadding = padding;

			m_memoryFreeSize -= pMemory->m_memorySize;

			return pMemory;
		}
	}

	return nullptr;
}

void CVKMemoryAllocator::FreeMemory(CVKMemory* pMemory)
{
	m_memoryFreeSize += pMemory->m_memorySize;

	pMemory->type = VK_RESOURCE_TYPE_FREE;
	pMemory->m_memoryPadding = 0;

	if (pMemory->pNext && pMemory->pNext->type == VK_RESOURCE_TYPE_FREE) {
		RemoveMemory(pMemory->pNext);
		pMemory = MergeMemory(pMemory, pMemory->pNext);
	}

	if (pMemory->pPrev && pMemory->pPrev->type == VK_RESOURCE_TYPE_FREE) {
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
}

void CVKMemoryAllocator::InsertMemory(CVKMemory* pMemory)
{
	ASSERT(pMemory);
	ASSERT(pMemory->type == VK_RESOURCE_TYPE_FREE);
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
	ASSERT(pMemory->type == VK_RESOURCE_TYPE_FREE);
	ASSERT(pMemory->pMemoryNode != nullptr);

	rb_erase(&pMemory->pMemoryNode->node, &m_root);
	m_nodes.erase(pMemory->pMemoryNode);

	delete pMemory->pMemoryNode;
}

CVKMemory* CVKMemoryAllocator::MergeMemory(CVKMemory* pMemory, CVKMemory* pMemoryNext)
{
	ASSERT(pMemory);
	ASSERT(pMemoryNext);
	ASSERT(pMemory->m_memoryOffset + pMemory->m_memorySize == pMemoryNext->m_memoryOffset);

	pMemory->m_memorySize = pMemoryNext->m_memorySize + pMemory->m_memorySize;
	pMemory->pNext = pMemoryNext->pNext;

	if (pMemoryNext->pNext) {
		pMemoryNext->pNext->pPrev = pMemory;
	}

	delete pMemoryNext;
	return pMemory;
}

static bool IsNeedCheckAliasing(VkResourceType prevType, VkResourceType currType)
{
	if (prevType == VK_RESOURCE_TYPE_BUFFER && currType == VK_RESOURCE_TYPE_IMAGE_OPTIMAL) {
		return true;
	}

	if (prevType == VK_RESOURCE_TYPE_IMAGE_LINEAR && currType == VK_RESOURCE_TYPE_IMAGE_OPTIMAL) {
		return true;
	}

	return false;
}

static bool IsMemoryAliasing(VkDeviceSize prevResourceOffset, VkDeviceSize prevResourceSize, VkDeviceSize currResourceOffset, VkDeviceSize bufferImageGranularity)
{
	// https://www.khronos.org/registry/vulkan/specs/1.2-extensions/html/vkspec.html#resources-bufferimagegranularity

	VkDeviceSize prevResourceEnd = prevResourceOffset + prevResourceSize - 1;
	VkDeviceSize prevResourceEndPage = prevResourceEnd & ~(bufferImageGranularity - 1);
	VkDeviceSize currResourceStart = currResourceOffset;
	VkDeviceSize currResourceStartPage = currResourceStart & ~(bufferImageGranularity - 1);
	return prevResourceEndPage > currResourceStartPage;
}

CVKMemory* CVKMemoryAllocator::SearchMemory(VkDeviceSize size, VkDeviceSize alignment, VkResourceType type, VkDeviceSize &offset, VkDeviceSize &padding) const
{
	const VkDeviceSize bufferImageGranularity = m_pDevice->GetPhysicalDeviceLimits().bufferImageGranularity;

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

		offset = ALIGN_BYTE(pMemoryNodeCur->pMemory->m_memoryOffset, alignment);
		{
			if (bufferImageGranularity > 1) {
				if (CVKMemory* pMemoryPrev = pMemoryNodeCur->pMemory->pPrev) {
					if (IsNeedCheckAliasing(pMemoryPrev->type, type) &&
						IsMemoryAliasing(pMemoryPrev->m_memoryOffset, pMemoryPrev->m_memorySize, offset, bufferImageGranularity)) {
						offset = ALIGN_BYTE(offset, bufferImageGranularity);
						ASSERT(IsMemoryAliasing(pMemoryPrev->m_memoryOffset, pMemoryPrev->m_memorySize, offset, bufferImageGranularity) == false);
					}
				}
			}
		}
		padding = offset - pMemoryNodeCur->pMemory->m_memoryOffset;

		if (pMemoryNodeCur->pMemory->m_memorySize < padding + size) {
			node = node->rb_right;
			continue;
		}

		pMemoryNode = pMemoryNodeCur;

		if (pMemoryNodeCur->pMemory->m_memorySize > padding + size) {
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
