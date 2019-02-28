#include "VKRenderer.h"


CVKCommandPool::CVKCommandPool(CVKDevice *pDevice, uint32_t queueFamilyIndex)
	: m_pDevice(pDevice)
	, m_vkCommandPool(VK_NULL_HANDLE)
{
	VkCommandPoolCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	createInfo.queueFamilyIndex = queueFamilyIndex;
	vkCreateCommandPool(m_pDevice->GetDevice(), &createInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkCommandPool);
}

CVKCommandPool::~CVKCommandPool(void)
{
	for (const auto &itCommandBuffer : m_pCommandBuffers) {
		delete itCommandBuffer.second;
	}

	if (m_vkCommandPool) {
		vkDestroyCommandPool(m_pDevice->GetDevice(), m_vkCommandPool, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
	}

	m_pCommandBuffers.clear();
	m_vkCommandPool = VK_NULL_HANDLE;
}

CVKCommandBuffer* CVKCommandPool::AllocCommandBuffer(bool bMainCommandBuffer)
{
	return nullptr;
}

void CVKCommandPool::FreeCommandBuffer(CVKCommandBuffer *pCommandBuffer)
{

}

void CVKCommandPool::ResetCommandPool(void)
{

}
