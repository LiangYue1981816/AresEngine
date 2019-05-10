#include "VKRenderer.h"


CVKAllocator::CVKAllocator(void)
	: m_allocatedSize(0)
	, m_maxAllocatedSize(0)
{
	m_vkCallbacks = {};
	m_vkCallbacks.pUserData = this;
	m_vkCallbacks.pfnAllocation = CVKAllocator::vkAllocationFunction;
	m_vkCallbacks.pfnReallocation = CVKAllocator::vkReallocationFunction;
	m_vkCallbacks.pfnFree = CVKAllocator::vkFreeFunction;
	m_vkCallbacks.pfnInternalAllocation = CVKAllocator::vkInternalAllocationNotification;
	m_vkCallbacks.pfnInternalFree = CVKAllocator::vkInternalFreeNotification;
}

CVKAllocator::~CVKAllocator(void)
{

}

uint32_t CVKAllocator::GetAllocatedSize(void) const
{
	return m_allocatedSize;
}

uint32_t CVKAllocator::GetMaxAllocatedSize(void) const
{
	return m_maxAllocatedSize;
}

const VkAllocationCallbacks* CVKAllocator::GetAllocationCallbacks(void) const
{
	return &m_vkCallbacks;
}

void* VKAPI_PTR CVKAllocator::vkAllocationFunction(void* pUserData, size_t size, size_t alignment, VkSystemAllocationScope allocationScope)
{
	ASSERT(size);
	ASSERT(alignment);
	ASSERT(pUserData);

	CVKAllocator* pAllocator = (CVKAllocator*)pUserData;
	void* pPointer = AllocMemory(size);

	pAllocator->m_allocatedSize += GET_MEM_SIZE(pPointer);
	pAllocator->m_maxAllocatedSize = pAllocator->m_maxAllocatedSize >= pAllocator->m_allocatedSize ? pAllocator->m_maxAllocatedSize : pAllocator->m_allocatedSize;

	return pPointer;
}

void* VKAPI_PTR CVKAllocator::vkReallocationFunction(void* pUserData, void* pOriginal, size_t size, size_t alignment, VkSystemAllocationScope allocationScope)
{
	ASSERT(size);
	ASSERT(alignment);
	ASSERT(pUserData);
	ASSERT(pOriginal);

	CVKAllocator* pAllocator = (CVKAllocator*)pUserData;
	void* pPointer = AllocMemory(size);

	memcpy(pPointer, pOriginal, std::min(GET_MEM_SIZE(pPointer), GET_MEM_SIZE(pOriginal)));
	pAllocator->m_allocatedSize += GET_MEM_SIZE(pPointer);
	pAllocator->m_allocatedSize -= GET_MEM_SIZE(pOriginal);
	pAllocator->m_maxAllocatedSize = pAllocator->m_maxAllocatedSize >= pAllocator->m_allocatedSize ? pAllocator->m_maxAllocatedSize : pAllocator->m_allocatedSize;
	FreeMemory(pOriginal);

	return pPointer;
}

void VKAPI_PTR CVKAllocator::vkFreeFunction(void* pUserData, void* pPointer)
{
	ASSERT(pPointer);
	ASSERT(pUserData);

	CVKAllocator* pAllocator = (CVKAllocator*)pUserData;

	pAllocator->m_allocatedSize -= GET_MEM_SIZE(pPointer);
	FreeMemory(pPointer);
}

void VKAPI_PTR CVKAllocator::vkInternalAllocationNotification(void* pUserData, size_t size, VkInternalAllocationType allocationType, VkSystemAllocationScope allocationScope)
{
	ASSERT(size);
	ASSERT(pUserData);

	CVKAllocator* pAllocator = (CVKAllocator*)pUserData;

	pAllocator->m_allocatedSize += size;
	pAllocator->m_maxAllocatedSize = pAllocator->m_maxAllocatedSize >= pAllocator->m_allocatedSize ? pAllocator->m_maxAllocatedSize : pAllocator->m_allocatedSize;
}

void VKAPI_PTR CVKAllocator::vkInternalFreeNotification(void* pUserData, size_t size, VkInternalAllocationType allocationType, VkSystemAllocationScope allocationScope)
{
	ASSERT(size);
	ASSERT(pUserData);

	CVKAllocator* pAllocator = (CVKAllocator*)pUserData;

	pAllocator->m_allocatedSize -= size;
	pAllocator->m_maxAllocatedSize = pAllocator->m_maxAllocatedSize >= pAllocator->m_allocatedSize ? pAllocator->m_maxAllocatedSize : pAllocator->m_allocatedSize;
}
