#include "VKRenderer.h"


CVKQueue::CVKQueue(CVKDevice *pDevice, uint32_t queueFamilyIndex, uint32_t queueIndex)
	: m_vkQueue(VK_NULL_HANDLE)
{
	vkGetDeviceQueue(pDevice->GetDevice(), queueFamilyIndex, queueIndex, &m_vkQueue);
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
	// ...
	return true;
}

void CVKQueue::WaitIdle(void) const
{
	vkQueueWaitIdle(m_vkQueue);
}
