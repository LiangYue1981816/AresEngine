#pragma once
#include "VKRenderer.h"


class CVKMemoryManager
{
	friend class CVKDevice;


private:
	CVKMemoryManager(CVKDevice *pDevice);
	virtual ~CVKMemoryManager(void);


private:
	CVKMemory* AllocMemory(VkDeviceSize memorySize, VkDeviceSize memoryAlignment, VkFlags memoryTypeBits, VkMemoryPropertyFlags memoryPropertyFlags);
	void FreeMemory(CVKMemory *pMemory);

private:
	void Log(void) const;


private:
	std::atomic_flag m_lock;
	eastl::unordered_map<uint32_t, eastl::unordered_map<VkDeviceSize, CVKMemoryAllocator*>> m_pAllocatorListHeads;

private:
	CVKDevice *m_pDevice;
};
