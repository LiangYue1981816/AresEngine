#pragma once
#include "rbtree.h"
#include "VKRenderer.h"


class CVKMemoryAllocator
{
	friend class CVKMemoryManager;


private:
	typedef struct mem_node {
		mem_node(uint32_t _index)
		{
			index = _index;
			pListHead = nullptr;
		}

		uint32_t size(void)
		{
			return (index + 1) * MIN_ALIGNMENT;
		}

		rb_node node;
		uint32_t index;
		CVKMemory *pListHead;
	} mem_node;


private:
	CVKMemoryAllocator(CVKDevice *pDevice, uint32_t memoryTypeIndex, VkDeviceSize memorySize);
	virtual ~CVKMemoryAllocator(void);


private:
	VkDeviceSize GetFreeSize(void) const;
	VkDeviceSize GetFullSize(void) const;

	uint32_t GetMemoryTypeIndex(void) const;

private:
	CVKMemory* AllocMemory(VkDeviceSize alignment, VkDeviceSize size);
	void FreeMemory(CVKMemory *pMemory);

private:
	void InitNodes(uint32_t numNodes);
	void FreeNodes(uint32_t numNodes);
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
	static const uint32_t MIN_ALIGNMENT = 256;

private:
	uint32_t m_memoryTypeIndex;
	VkMemoryPropertyFlags m_memoryPropertyFlags;

	VkDeviceSize m_freeSize;
	VkDeviceSize m_fullSize;
	VkDeviceMemory m_vkMemory;

private:
	rb_root m_root;
	mem_node **m_nodes;

private:
	CVKDevice *m_pDevice;

private:
	CVKMemoryAllocator *pNext;
	CVKMemoryAllocator *pPrev;
};
