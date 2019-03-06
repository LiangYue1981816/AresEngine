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

VkQueue CVKQueue::GetQueue(void) const
{
	return m_vkQueue;
}

uint32_t CVKQueue::GetQueueFamilyIndex(void) const
{
	return m_queueFamilyIndex;
}

bool CVKQueue::Submit(const eastl::vector<CGfxCommandBufferPtr> &ptrCommandBuffers, VkSemaphore vkWaitSemaphore, VkPipelineStageFlags waitStageFlags, VkSemaphore vkSignalSemaphore, VkFence vkFence) const
{
	eastl::vector<VkCommandBuffer> vkCommandBuffers;
	{
		for (int index = 0; index < ptrCommandBuffers.size(); index++) {
			vkCommandBuffers.emplace_back((VkCommandBuffer)ptrCommandBuffers[index]->GetCommandBuffer());
		}
	}
	if (vkCommandBuffers.empty()) return false;

	vkWaitForFences(m_pDevice->GetDevice(), 1, &vkFence, VK_TRUE, UINT64_MAX);
	vkResetFences(m_pDevice->GetDevice(), 1, &vkFence);

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.pNext = nullptr;
	submitInfo.waitSemaphoreCount = vkWaitSemaphore != VK_NULL_HANDLE ? 1 : 0;
	submitInfo.pWaitSemaphores = vkWaitSemaphore != VK_NULL_HANDLE ? &vkWaitSemaphore : nullptr;
	submitInfo.pWaitDstStageMask = vkWaitSemaphore != VK_NULL_HANDLE ? &waitStageFlags : nullptr;
	submitInfo.commandBufferCount = vkCommandBuffers.size();
	submitInfo.pCommandBuffers = vkCommandBuffers.data();
	submitInfo.signalSemaphoreCount = vkSignalSemaphore != VK_NULL_HANDLE ? 1 : 0;
	submitInfo.pSignalSemaphores = vkSignalSemaphore != VK_NULL_HANDLE ? &vkSignalSemaphore : nullptr;
	CALL_VK_FUNCTION_RETURN_BOOL(vkQueueSubmit(m_vkQueue, 1, &submitInfo, vkFence));

	return true;
}

void CVKQueue::WaitIdle(void) const
{
	vkQueueWaitIdle(m_vkQueue);
}
