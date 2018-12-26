#include "VKRenderer.h"


CVKQueue::CVKQueue(CVKDevice *pDevice, uint32_t queueFamilyIndex)
	: m_pDevice(pDevice)

	, m_vkQueue(VK_NULL_HANDLE)
	, m_queueFamilyIndex(queueFamilyIndex)
{
	vkGetDeviceQueue(m_pDevice->GetDevice(), m_queueFamilyIndex, 0, &m_vkQueue);
}

CVKQueue::~CVKQueue(void)
{

}

bool CVKQueue::Submit(const CGfxCommandBufferPtr &ptrCommandBuffer) const
{
	return Submit(ptrCommandBuffer, VK_NULL_HANDLE, 0, VK_NULL_HANDLE);
}

bool CVKQueue::Submit(const CGfxCommandBufferPtr &ptrCommandBuffer, VkSemaphore vkWaitSemaphore, VkPipelineStageFlags waitStageFlags, VkSemaphore vkSignalSemaphore) const
{
	return true;
}

void CVKQueue::WaitIdle(void) const
{
	vkQueueWaitIdle(m_vkQueue);
}

VkQueue CVKQueue::GetQueue(void) const
{
	return m_vkQueue;
}

uint32_t CVKQueue::GetQueueFamilyIndex(void) const
{
	return m_queueFamilyIndex;
}
