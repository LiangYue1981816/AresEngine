#pragma once
#include "rbtree.h"
#include "VKRenderer.h"


typedef enum VkResourceType {
	VK_RESOURCE_TYPE_FREE = 0,
	VK_RESOURCE_TYPE_UNKNOW,
	VK_RESOURCE_TYPE_BUFFER,
	VK_RESOURCE_TYPE_IMAGE_LINEAR,
	VK_RESOURCE_TYPE_IMAGE_OPTIMAL,
} VkResourceType;

typedef struct mem_node {
	mem_node(CVKMemory* pFreeMemory);
	~mem_node(void);

	rb_node node;
	CVKMemory* pMemory;
} mem_node;


class CVKMemory
{
	friend struct mem_node;
	friend class CVKMemoryAllocator;
	friend class CVKMemoryManager;


private:
	CVKMemory(CVKDevice* pDevice, CVKMemoryAllocator* pAllocator, VkDeviceSize memorySize, VkDeviceSize memoryOffset);
	virtual ~CVKMemory(void);


private:
	CVKMemoryAllocator* GetAllocator(void) const;

public:
	VkDeviceSize GetSize(void) const;
	VkDeviceSize GetOffset(void) const;

public:
	bool BindImage(VkImage vkImage) const;
	bool BindBuffer(VkBuffer vkBuffer) const;

public:
	bool BeginMap(void);
	bool CopyData(VkDeviceSize offset, VkDeviceSize size, const void* data);
	bool EndMap(void);

public:
	bool IsDeviceLocal(void) const;
	bool IsHostVisible(void) const;
	bool IsHostCoherent(void) const;
	bool IsHostCached(void) const;
	bool IsLazilyAllocated(void) const;


private:
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

	VkDeviceSize m_memorySize;
	VkDeviceSize m_memoryOffset;
	VkDeviceSize m_memoryPadding;

private:
	CVKMemoryAllocator* m_pAllocator;

private:
	CVKDevice* m_pDevice;

private:
	VkResourceType type;

	CVKMemory* pNext;
	CVKMemory* pPrev;

private:
	mem_node* pMemoryNode;
};
