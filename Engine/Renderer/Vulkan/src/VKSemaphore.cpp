#include "VKRenderer.h"


CVKSemaphore::CVKSemaphore(CVKDevice* pDevice)
	: m_pDevice(pDevice)
	, m_vkSemaphore(VK_NULL_HANDLE)
{
	VkSemaphoreCreateInfo semaphoreCreateInfo = {};
	semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	semaphoreCreateInfo.pNext = nullptr;
	semaphoreCreateInfo.flags = 0;
	CALL_VK_FUNCTION_RETURN(vkCreateSemaphore(m_pDevice->GetDevice(), &semaphoreCreateInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkSemaphore));
}

CVKSemaphore::~CVKSemaphore(void)
{
	vkDestroySemaphore(m_pDevice->GetDevice(), m_vkSemaphore, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
}

VkSemaphore CVKSemaphore::GetSemaphore(void) const
{
	return m_vkSemaphore;
}
