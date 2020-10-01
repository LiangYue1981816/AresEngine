#pragma once
#include "VKRenderer.h"


class CVKMemoryManager
{
	friend class CVKDevice;
	friend class CVKMemory;
	friend class CVKMemoryAllocator;


private:
	CVKMemoryManager(CVKDevice* pDevice, VkDeviceSize vkHostMemorySize, VkDeviceSize vkHostOrCacheMemorySize, VkDeviceSize vkHostOrCacheOrDeviceMemorySize, VkDeviceSize vkDeviceMemorySize, VkDeviceSize vkDeviceOrLazilyMemorySize);
	virtual ~CVKMemoryManager(void);


public:
	CVKMemory* AllocMemory(VkDeviceSize memorySize, VkDeviceSize memoryAlignment, VkMemoryPropertyFlags memoryPropertyFlags, VkResourceType type);
	void FreeMemory(CVKMemory* pMemory);


private:
	pthread_mutex_t lock;
	eastl::unordered_map<uint32_t, CVKMemoryAllocator*> m_pAllocators;

private:
	CVKDevice* m_pDevice;
};
