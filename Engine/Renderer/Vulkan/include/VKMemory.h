#pragma once
#include "VKRenderer.h"


class CVKMemory
{
	friend class CVKMemoryAllocator;
	friend class CVKMemoryManager;


private:
	CVKMemory(CVKMemoryAllocator *pAllocator, CVKDevice *pDevice, VkDeviceMemory vkMemory, VkMemoryPropertyFlags flags, VkDeviceSize offset, VkDeviceSize aligmentOffset, VkDeviceSize size);
	virtual ~CVKMemory(void);


private:
	CVKMemoryAllocator* GetAllocator(void) const;

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

public:
	VkDeviceSize GetOffset(void) const;
	VkDeviceSize GetAligmentOffset(void) const;
	VkDeviceSize GetSize(void) const;


private:
	VkMemoryPropertyFlags m_flags;
	VkDeviceSize m_offset;
	VkDeviceSize m_aligmentOffset;
	VkDeviceSize m_size;

private:
	VkDeviceMemory m_vkMemory;
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
