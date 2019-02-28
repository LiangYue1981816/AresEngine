#include "VKRenderer.h"


CVKCommandPool::CVKCommandPool(CVKDevice *pDevice)
	: m_pDevice(pDevice)
	, m_vkCommandPool(VK_NULL_HANDLE)
{
	VkCommandPoolCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	createInfo.queueFamilyIndex = m_pDevice->GetQueue()->GetQueueFamilyIndex();
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

VkCommandPool CVKCommandPool::GetCommandPool(void) const
{
	return m_vkCommandPool;
}

CVKCommandBuffer* CVKCommandPool::AllocCommandBuffer(bool bMainCommandBuffer)
{
	CVKCommandBuffer *pCommandBuffer = new CVKCommandBuffer(m_pDevice, this, bMainCommandBuffer);
	m_pCommandBuffers[pCommandBuffer] = pCommandBuffer;
	return pCommandBuffer;
}

void CVKCommandPool::FreeCommandBuffer(CVKCommandBuffer *pCommandBuffer)
{
	m_pCommandBuffers.erase(pCommandBuffer);
	delete pCommandBuffer;
}

void CVKCommandPool::ResetCommandPool(void)
{
	vkResetCommandPool(m_pDevice->GetDevice(), m_vkCommandPool, VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT);
}
