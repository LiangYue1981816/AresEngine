#include "VKRenderer.h"


CVKUniformBufferManager::CVKUniformBufferManager(CVKDevice* pDevice)
	: m_pDevice(pDevice)
{

}

CVKUniformBufferManager::~CVKUniformBufferManager(void)
{
	for (const auto& itUniformBuffer : m_pUniformBuffers) {
		delete itUniformBuffer.second;
	}
}

CVKUniformBuffer* CVKUniformBufferManager::Create(size_t size)
{
	mutex_autolock autolock(&lock);
	{
		if (CVKUniformBuffer* pUniformBuffer = new CVKUniformBuffer(m_pDevice, this, ALIGN_BYTE(size, m_pDevice->GetPhysicalDeviceLimits().minUniformBufferOffsetAlignment))) {
			m_pUniformBuffers[pUniformBuffer] = pUniformBuffer;
			return pUniformBuffer;
		}
		else {
			return nullptr;
		}
	}
}

void CVKUniformBufferManager::Destroy(CVKUniformBuffer* pUniformBuffer)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pUniformBuffers.find(pUniformBuffer) != m_pUniformBuffers.end()) {
			m_pUniformBuffers.erase(pUniformBuffer);
			delete pUniformBuffer;
		}
	}
}
