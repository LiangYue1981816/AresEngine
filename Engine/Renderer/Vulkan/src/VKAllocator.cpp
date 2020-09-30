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
	CVKAllocator* pAllocator = (CVKAllocator*)pUserData;
	void* pPointer = AllocMemory(size);

	pAllocator->m_allocatedSize += GET_MEM_SIZE(pPointer);
	pAllocator->m_maxAllocatedSize = std::max(pAllocator->m_allocatedSize, pAllocator->m_maxAllocatedSize);

	return pPointer;
}

void* VKAPI_PTR CVKAllocator::vkReallocationFunction(void* pUserData, void* pOriginal, size_t size, size_t alignment, VkSystemAllocationScope allocationScope)
{
	CVKAllocator* pAllocator = (CVKAllocator*)pUserData;
	void* pPointer = AllocMemory(size);

	memcpy(pPointer, pOriginal, std::min(GET_MEM_SIZE(pPointer), GET_MEM_SIZE(pOriginal)));

	pAllocator->m_allocatedSize += GET_MEM_SIZE(pPointer);
	pAllocator->m_allocatedSize -= GET_MEM_SIZE(pOriginal);
	pAllocator->m_maxAllocatedSize = std::max(pAllocator->m_allocatedSize, pAllocator->m_maxAllocatedSize);

	FreeMemory(pOriginal);

	return pPointer;
}

void VKAPI_PTR CVKAllocator::vkFreeFunction(void* pUserData, void* pPointer)
{
	CVKAllocator* pAllocator = (CVKAllocator*)pUserData;

	pAllocator->m_allocatedSize -= GET_MEM_SIZE(pPointer);
	pAllocator->m_maxAllocatedSize = std::max(pAllocator->m_allocatedSize, pAllocator->m_maxAllocatedSize);

	FreeMemory(pPointer);
}

void VKAPI_PTR CVKAllocator::vkInternalAllocationNotification(void* pUserData, size_t size, VkInternalAllocationType allocationType, VkSystemAllocationScope allocationScope)
{
	CVKAllocator* pAllocator = (CVKAllocator*)pUserData;

	pAllocator->m_allocatedSize += size;
	pAllocator->m_maxAllocatedSize = std::max(pAllocator->m_allocatedSize, pAllocator->m_maxAllocatedSize);
}

void VKAPI_PTR CVKAllocator::vkInternalFreeNotification(void* pUserData, size_t size, VkInternalAllocationType allocationType, VkSystemAllocationScope allocationScope)
{
	CVKAllocator* pAllocator = (CVKAllocator*)pUserData;

	pAllocator->m_allocatedSize -= size;
	pAllocator->m_maxAllocatedSize = std::max(pAllocator->m_allocatedSize, pAllocator->m_maxAllocatedSize);
}
