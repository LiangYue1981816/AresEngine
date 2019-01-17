#pragma once
#include "VKRenderer.h"


class CVKAllocator
{
	friend class CVKInstance;


private:
	CVKAllocator(void);
	virtual ~CVKAllocator(void);


public:
	uint32_t GetAllocatedSize(void) const;
	uint32_t GetMaxAllocatedSize(void) const;

public:
	const VkAllocationCallbacks* GetAllocationCallbacks(void) const;

private:
	static void* VKAPI_PTR vkAllocationFunction(void *pUserData, size_t size, size_t alignment, VkSystemAllocationScope allocationScope);
	static void* VKAPI_PTR vkReallocationFunction(void *pUserData, void *pOriginal, size_t size, size_t alignment, VkSystemAllocationScope allocationScope);
	static void  VKAPI_PTR vkFreeFunction(void *pUserData, void *pPointer);
	static void  VKAPI_PTR vkInternalAllocationNotification(void *pUserData, size_t size, VkInternalAllocationType allocationType, VkSystemAllocationScope allocationScope);
	static void  VKAPI_PTR vkInternalFreeNotification(void *pUserData, size_t size, VkInternalAllocationType allocationType, VkSystemAllocationScope allocationScope);


private:
	uint32_t m_allocatedSize;
	uint32_t m_maxAllocatedSize;

private:
	VkAllocationCallbacks m_vkCallbacks;
};
