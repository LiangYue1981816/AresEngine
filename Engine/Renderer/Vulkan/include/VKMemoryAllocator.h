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
	CVKMemoryAllocator(CVKDevice *pDevice, uint32_t memoryTypeIndex, VkDeviceSize memorySize);
	virtual ~CVKMemoryAllocator(void);


private:
	CVKMemory* AllocMemory(VkDeviceSize alignment, VkDeviceSize size);
	void FreeMemory(CVKMemory *pMemory);

private:
	void InitNodes(uint32_t numNodes);
	void InsertMemory(CVKMemory *pMemory);
	void RemoveMemory(CVKMemory *pMemory);
	CVKMemory* MergeMemory(CVKMemory *pMemory, CVKMemory *pMemoryNext);
	CVKMemory* SearchMemory(VkDeviceSize size) const;

private:
	bool IsDeviceLocal(void) const;
	bool IsHostVisible(void) const;
	bool IsHostCoherent(void) const;
	bool IsHostCached(void) const;
	bool IsLazilyAllocated(void) const;

private:
	uint32_t GetMemoryTypeIndex(void) const;
	VkDeviceSize GetFreeSize(void) const;
	VkDeviceSize GetFullSize(void) const;


private:
	uint32_t m_indexType;
	VkDeviceSize m_freeSize;
	VkDeviceSize m_fullSize;

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
