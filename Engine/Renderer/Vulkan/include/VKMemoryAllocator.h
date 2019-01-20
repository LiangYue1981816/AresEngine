#pragma once
#include "rbtree.h"
#include "VKRenderer.h"


class CVKMemoryAllocator
{
	friend class CVKMemoryManager;


private:
	typedef struct mem_node {
		mem_node(uint32_t index, VkDeviceSize alignment)
		{
			size = (index + 1) * alignment;
			pListHead = nullptr;
		}

		rb_node node;
		VkDeviceSize size;
		CVKMemory *pListHead;
	} mem_node;


private:
	CVKMemoryAllocator(CVKDevice *pDevice, uint32_t memoryTypeIndex, VkDeviceSize memorySize, VkDeviceSize memoryAlignment);
	virtual ~CVKMemoryAllocator(void);


private:
	uint32_t GetMemoryTypeIndex(void) const;

	VkDeviceSize GetFreeSize(void) const;
	VkDeviceSize GetFullSize(void) const;
	VkDeviceSize GetAlignment(void) const;

private:
	CVKMemory* AllocMemory(VkDeviceSize size);
	void FreeMemory(CVKMemory *pMemory);

private:
	void InitNodes(void);
	void FreeNodes(void);
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
	uint32_t m_memoryTypeIndex;
	VkMemoryPropertyFlags m_memoryPropertyFlags;

	VkDeviceSize m_freeSize;
	VkDeviceSize m_fullSize;
	VkDeviceSize m_alignment;

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
