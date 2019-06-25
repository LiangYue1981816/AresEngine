#include "VKRenderer.h"


CVKSemaphore::CVKSemaphore(CVKDevice* pDevice)
	: m_pDevice(pDevice)
	, m_vkSemaphore(VK_NULL_HANDLE)
{
	Create();
}

CVKSemaphore::~CVKSemaphore(void)
{
	Destroy();
}

bool CVKSemaphore::Create(void)
{
	VkSemaphoreCreateInfo semaphoreCreateInfo = {};
	semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	semaphoreCreateInfo.pNext = nullptr;
	semaphoreCreateInfo.flags = 0;
	CALL_VK_FUNCTION_RETURN_BOOL(vkCreateSemaphore(m_pDevice->GetDevice(), &semaphoreCreateInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkSemaphore));

	return true;
}

void CVKSemaphore::Destroy(void)
{
	ASSERT(m_vkSemaphore);

	vkDestroySemaphore(m_pDevice->GetDevice(), m_vkSemaphore, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
	m_vkSemaphore = VK_NULL_HANDLE;
}

VkSemaphore CVKSemaphore::GetSemaphore(void) const
{
	return m_vkSemaphore;
}
