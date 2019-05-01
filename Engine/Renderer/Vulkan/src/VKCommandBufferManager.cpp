#include "VKRenderer.h"


CVKCommandBufferManager::CVKCommandBufferManager(CVKDevice* pDevice)
	: m_pDevice(pDevice)
{

}

CVKCommandBufferManager::~CVKCommandBufferManager(void)
{
	for (const auto& itCommandBuffers : m_pCommandBuffers) {
		for (const auto& itCommandBuffer : itCommandBuffers.second) {
			delete itCommandBuffer.second;
		}
	}

	for (const auto& itCommandPool : m_vkCommandPools) {
		vkDestroyCommandPool(m_pDevice->GetDevice(), itCommandPool.second, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
	}
}

CVKCommandBuffer* CVKCommandBufferManager::Create(uint32_t pool, bool bMainCommandBuffer)
{
	mutex_autolock autolock(&lock);
	{
		if (m_vkCommandPools[pool] == VK_NULL_HANDLE) {
			VkCommandPoolCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			createInfo.pNext = nullptr;
			createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
			createInfo.queueFamilyIndex = m_pDevice->GetQueue()->GetQueueFamilyIndex();
			vkCreateCommandPool(m_pDevice->GetDevice(), &createInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkCommandPools[pool]);
		}

		CVKCommandBuffer* pCommandBuffer = new CVKCommandBuffer(m_pDevice, this, m_vkCommandPools[pool], bMainCommandBuffer);
		m_pCommandBuffers[m_vkCommandPools[pool]][pCommandBuffer] = pCommandBuffer;

		return pCommandBuffer;
	}
}

void CVKCommandBufferManager::Destroy(CVKCommandBuffer* pCommandBuffer)
{
	mutex_autolock autolock(&lock);
	{
		if (pCommandBuffer) {
			if (m_pCommandBuffers[pCommandBuffer->GetCommandPool()].find(pCommandBuffer) != m_pCommandBuffers[pCommandBuffer->GetCommandPool()].end()) {
				m_pCommandBuffers[pCommandBuffer->GetCommandPool()].erase(pCommandBuffer);
				delete pCommandBuffer;
			}
		}
	}
}
