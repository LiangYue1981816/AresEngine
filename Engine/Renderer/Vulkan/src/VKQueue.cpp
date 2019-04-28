#include "VKRenderer.h"


CVKQueue::CVKQueue(CVKDevice* pDevice, uint32_t queueFamilyIndex)
	: m_pDevice(pDevice)

	, m_vkQueue(VK_NULL_HANDLE)
	, m_queueFamilyIndex(queueFamilyIndex)
{
	vkGetDeviceQueue(m_pDevice->GetDevice(), m_queueFamilyIndex, 0, &m_vkQueue);
}

CVKQueue::~CVKQueue(void)
{

}

VkQueue CVKQueue::GetQueue(void) const
{
	return m_vkQueue;
}

uint32_t CVKQueue::GetQueueFamilyIndex(void) const
{
	return m_queueFamilyIndex;
}

bool CVKQueue::Submit(CGfxCommandBufferPtr ptrCommandBuffers, VkSemaphore vkWaitSemaphore, VkPipelineStageFlags waitStageFlags, VkSemaphore vkSignalSemaphore) const
{
	VkFence vkFence = ((CVKCommandBuffer*)ptrCommandBuffers.GetPointer())->GetFence();
	VkCommandBuffer vkCommandBuffer = ((CVKCommandBuffer*)ptrCommandBuffers.GetPointer())->GetCommandBuffer();

	CALL_VK_FUNCTION_RETURN_BOOL(vkWaitForFences(m_pDevice->GetDevice(), 1, &vkFence, VK_TRUE, UINT64_MAX));
	CALL_VK_FUNCTION_RETURN_BOOL(vkResetFences(m_pDevice->GetDevice(), 1, &vkFence));
	CALL_VK_FUNCTION_RETURN_BOOL(vkSubmitCommandBuffer(m_vkQueue, vkCommandBuffer, vkFence, vkWaitSemaphore, waitStageFlags, vkSignalSemaphore));

	return true;
}

void CVKQueue::WaitIdle(void) const
{
	vkQueueWaitIdle(m_vkQueue);
}
