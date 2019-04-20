#pragma once
#include "VKRenderer.h"


class CVKMemory
{
	friend class CVKMemoryAllocator;
	friend class CVKMemoryManager;


private:
	CVKMemory(CVKMemoryAllocator *pAllocator, CVKDevice *pDevice, VkDeviceSize memorySize, VkDeviceSize memoryOffset);
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
	bool BeginMap(VkDeviceSize offset, VkDeviceSize size, void **ppAddress) const;
	bool EndMap(void) const;
	bool Flush(VkDeviceSize offset, VkDeviceSize size) const;
	bool Invalidate(VkDeviceSize offset, VkDeviceSize size) const;

public:
	bool IsDeviceLocal(void) const;
	bool IsHostVisible(void) const;
	bool IsHostCoherent(void) const;
	bool IsHostCached(void) const;
	bool IsLazilyAllocated(void) const;


private:
	VkDeviceSize m_memorySize;
	VkDeviceSize m_memoryOffset;

private:
	CVKMemoryAllocator *m_pAllocator;

private:
	CVKDevice *m_pDevice;

private:
	bool bInUse;

	CVKMemory *pNext;
	CVKMemory *pPrev;

	CVKMemory *pFreeNext;
	CVKMemory *pFreePrev;
};
