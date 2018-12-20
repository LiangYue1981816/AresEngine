#pragma once
#include "rbtree.h"
#include "VKRenderer.h"


class CVKMemoryAllocator
{
	friend class CVKMemoryManager;


private:
	typedef struct mem_node {
		rb_node node;
		VkDeviceSize size;
		CVKMemory *pListHead;
	} mem_node;


private:
	CVKMemoryAllocator(CVKDevice *pDevice, uint32_t memoryTypeIndex, VkDeviceSize memorySize, VkDeviceSize memoryAlignment);
	virtual ~CVKMemoryAllocator(void);


private:
	void InitNodes(uint32_t numNodes);
	void InsertMemory(CVKMemory *pMemory);
	void RemoveMemory(CVKMemory *pMemory);
	CVKMemory* MergeMemory(CVKMemory *pMemory, CVKMemory *pMemoryNext);
	CVKMemory* SearchMemory(VkDeviceSize size) const;


private:
	uint32_t m_indexType;
	VkDeviceSize m_freeSize;
	VkDeviceSize m_fullSize;
	VkDeviceSize m_alignment;

private:
	rb_root m_root;
	mem_node *m_nodes;
	CVKMemory *m_pListHead;

private:
	VkDeviceMemory m_vkMemory;

private:
	CVKDevice *m_pDevice;

private:
	CVKMemoryAllocator *pNext;
	CVKMemoryAllocator *pPrev;
};
