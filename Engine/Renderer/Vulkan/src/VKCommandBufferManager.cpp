#include "VKRenderer.h"


CVKCommandBufferManager::CVKCommandBufferManager(CVKDevice *pDevice)
	: m_pDevice(pDevice)
{

}

CVKCommandBufferManager::~CVKCommandBufferManager(void)
{
	for (const auto &itCommandBuffer : m_pCommandBuffers) {
		delete itCommandBuffer.second;
	}

	m_pCommandBuffers.clear();
}

CVKCommandBuffer* CVKCommandBufferManager::Create(bool bMainCommandBuffer)
{
	mutex_autolock autolock(&lock);
	{
		CVKCommandBuffer *pCommandBuffer = new CVKCommandBuffer(m_pDevice, this, bMainCommandBuffer);
		m_pCommandBuffers[pCommandBuffer] = pCommandBuffer;
		return pCommandBuffer;
	}
}

void CVKCommandBufferManager::Destroy(CVKCommandBuffer *pCommandBuffer)
{
	mutex_autolock autolock(&lock);
	{
		if (pCommandBuffer) {
			m_pCommandBuffers.erase(pCommandBuffer);
			delete pCommandBuffer;
		}
	}
}
