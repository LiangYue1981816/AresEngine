#include "VKRenderer.h"


CVKCommandBufferManager::CVKCommandBufferManager(CVKDevice* pDevice)
	: m_pDevice(pDevice)
{

}

CVKCommandBufferManager::~CVKCommandBufferManager(void)
{
	for (const auto& itCommandBuffers : m_pCommandBuffers) {
		for (const auto& itCommandBufferList : itCommandBuffers.second) {
			for (const auto& itCommandBuffer : itCommandBufferList.second) {
				delete itCommandBuffer;
			}
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
			CALL_VK_FUNCTION_ASSERT(vkCreateCommandPool(m_pDevice->GetDevice(), &createInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkCommandPools[pool]));
		}

		CVKCommandBuffer* pCommandBuffer = nullptr;
		VkCommandPool vkCommandPool = m_vkCommandPools[pool];

		if (m_pCommandBuffers[vkCommandPool][bMainCommandBuffer].empty()) {
			pCommandBuffer = new CVKCommandBuffer(m_pDevice, this, vkCommandPool, bMainCommandBuffer);
		}
		else {
			pCommandBuffer = m_pCommandBuffers[vkCommandPool][bMainCommandBuffer].front();
			m_pCommandBuffers[vkCommandPool][bMainCommandBuffer].pop_front();
		}

		return pCommandBuffer;
	}
}

void CVKCommandBufferManager::Destroy(CVKCommandBuffer* pCommandBuffer)
{
	mutex_autolock autolock(&lock);
	{
		m_pCommandBuffers[pCommandBuffer->GetCommandPool()][pCommandBuffer->IsMainCommandBuffer()].emplace_back(pCommandBuffer);
	}
}
