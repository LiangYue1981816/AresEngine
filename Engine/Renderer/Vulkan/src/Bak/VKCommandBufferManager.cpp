#include "VKRenderer.h"


CVKCommandBufferManager::CVKCommandBufferManager(CVKDevice* pDevice)
	: m_pDevice(pDevice)
{

}

CVKCommandBufferManager::~CVKCommandBufferManager(void)
{
	for (const auto& itCommandPool : m_pCommandPools) {
		delete itCommandPool.second;
	}
}

CVKCommandBuffer* CVKCommandBufferManager::Create(uint32_t pool, bool bMainCommandBuffer)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pCommandPools[pool] == nullptr) {
			m_pCommandPools[pool] = new CVKCommandPool(m_pDevice);
		}

		return m_pCommandPools[pool]->AllocCommandBuffer(bMainCommandBuffer);
	}
}
