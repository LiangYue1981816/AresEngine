#include "VKRenderer.h"


CVKUniformBufferManager::CVKUniformBufferManager(CVKDevice *pDevice)
	: m_pDevice(pDevice)
{

}

CVKUniformBufferManager::~CVKUniformBufferManager(void)
{
	for (const auto &UniformBuffer : m_pUniformBuffers) {
		delete UniformBuffer.second;
	}

	m_pUniformBuffers.clear();
}

CVKUniformBuffer* CVKUniformBufferManager::Create(size_t size)
{
	mutex_autolock autolock(&lock);
	{
		CVKUniformBuffer *pUniformBuffer = new CVKUniformBuffer(m_pDevice, this, size);
		m_pUniformBuffers[pUniformBuffer] = pUniformBuffer;
		return pUniformBuffer;
	}
}

void CVKUniformBufferManager::Destroy(CVKUniformBuffer *pUniformBuffer)
{
	mutex_autolock autolock(&lock);
	{
		if (pUniformBuffer) {
			m_pUniformBuffers.erase(pUniformBuffer);
			delete pUniformBuffer;
		}
	}
}
