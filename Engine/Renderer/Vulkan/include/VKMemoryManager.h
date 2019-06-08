#pragma once
#include "VKRenderer.h"


class CVKMemoryManager
{
	friend class CVKDevice;
	friend class CVKMemory;
	friend class CVKMemoryAllocator;


private:
	CVKMemoryManager(CVKDevice* pDevice);
	virtual ~CVKMemoryManager(void);


private:
	uint32_t GetMemoryTypeIndex(const VkPhysicalDeviceMemoryProperties& memoryProperties, VkMemoryPropertyFlags& memoryPropertyFlags, VkDeviceSize memorySize);

public:
	CVKMemory* AllocMemory(VkDeviceSize memorySize, VkMemoryPropertyFlags memoryPropertyFlags, VkResourceType type);
	CVKMemory* AllocMemory(VkDeviceSize memorySize, VkDeviceSize memoryAlignment, VkMemoryPropertyFlags memoryPropertyFlags, VkResourceType type);
	void FreeMemory(CVKMemory* pMemory);

public:
	void Log(void);


private:
	pthread_mutex_t lock;
	eastl::unordered_map<uint32_t, VkDeviceSize> m_allocatedMemoryHeapSize;
	eastl::unordered_map<uint32_t, CVKMemoryAllocator*> m_pAllocatorListHeads;

private:
	CVKDevice* m_pDevice;
};
