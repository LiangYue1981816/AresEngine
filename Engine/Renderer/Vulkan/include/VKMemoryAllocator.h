#pragma once
#include "rbtree.h"
#include "VKRenderer.h"


class CVKMemoryAllocator
{
	friend class CVKMemory;
	friend class CVKMemoryManager;


private:
	CVKMemoryAllocator(CVKDevice* pDevice, uint32_t memoryTypeIndex, VkDeviceSize memorySize);
	virtual ~CVKMemoryAllocator(void);


private:
	bool Create(uint32_t memoryTypeIndex, VkDeviceSize memorySize);
	void Destroy(void);

private:
	void* GetMemoryAddress(void) const;
	VkDeviceMemory GetMemory(void) const;

	VkDeviceSize GetFreeSize(void) const;
	VkDeviceSize GetFullSize(void) const;

	uint32_t GetMemoryTypeIndex(void) const;
	VkMemoryPropertyFlags GetMemoryPropertyFlags(void) const;

private:
	CVKMemory* AllocMemory(VkDeviceSize size);
	void FreeMemory(CVKMemory* pMemory);

private:
	void InitNodes(void);
	void FreeNodes(void);

	void InsertMemory(CVKMemory* pMemory);
	void RemoveMemory(CVKMemory* pMemory);
	CVKMemory* MergeMemory(CVKMemory* pMemory, CVKMemory* pMemoryNext);
	CVKMemory* SearchMemory(VkDeviceSize size, VkDeviceSize alignment) const;

private:
	bool IsDeviceLocal(void) const;
	bool IsHostVisible(void) const;
	bool IsHostCoherent(void) const;
	bool IsHostCached(void) const;
	bool IsLazilyAllocated(void) const;


private:
	rb_root m_root;
	eastl::unordered_map<mem_node*, VkDeviceSize> m_nodes;

private:
	void* m_pMemoryAddress;
	VkDeviceMemory m_vkMemory;

	VkDeviceSize m_memoryFreeSize;
	VkDeviceSize m_memoryFullSize;

	uint32_t m_memoryTypeIndex;
	VkMemoryPropertyFlags m_memoryPropertyFlags;

private:
	CVKDevice* m_pDevice;

private:
	CVKMemoryAllocator* pNext;
	CVKMemoryAllocator* pPrev;
};
