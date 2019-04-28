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
	uint32_t GetMemoryTypeIndex(const VkPhysicalDeviceMemoryProperties& memoryProperties, VkFlags memoryTypeBits, VkMemoryPropertyFlags& memoryPropertyFlags, VkDeviceSize memorySize);

public:
	CVKMemory* AllocMemory(VkDeviceSize memorySize, VkDeviceSize memoryAlignment, VkFlags memoryTypeBits, VkMemoryPropertyFlags memoryPropertyFlags);
	void FreeMemory(CVKMemory* pMemory);

public:
	void Log(void);


private:
	pthread_mutex_t m_lock;
	eastl::unordered_map<uint32_t, VkDeviceSize> m_allocatedMemoryHeapSize;
	eastl::unordered_map<uint32_t, eastl::unordered_map<uint32_t, CVKMemoryAllocator*>> m_pAllocatorListHeads;

private:
	CVKDevice* m_pDevice;
};
