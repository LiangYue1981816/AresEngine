#include "VKRenderer.h"


CVKUniformBufferManager::CVKUniformBufferManager(CVKDevice* pDevice)
	: m_pDevice(pDevice)
{
	ASSERT(m_pDevice);
}

CVKUniformBufferManager::~CVKUniformBufferManager(void)
{
	for (const auto& UniformBuffer : m_pUniformBuffers) {
		delete UniformBuffer.second;
	}
}

CVKUniformBuffer* CVKUniformBufferManager::Create(size_t size)
{
	mutex_autolock autolock(&lock);
	{
		CVKUniformBuffer* pUniformBuffer = new CVKUniformBuffer(m_pDevice, this, ALIGN_BYTE(size, m_pDevice->GetPhysicalDeviceLimits().minUniformBufferOffsetAlignment));
		m_pUniformBuffers[pUniformBuffer] = pUniformBuffer;
		return pUniformBuffer;
	}
}

void CVKUniformBufferManager::Destroy(CVKUniformBuffer* pUniformBuffer)
{
	mutex_autolock autolock(&lock);
	{
		ASSERT(pUniformBuffer);

		if (m_pUniformBuffers.find(pUniformBuffer) != m_pUniformBuffers.end()) {
			m_pUniformBuffers.erase(pUniformBuffer);
			delete pUniformBuffer;
		}
	}
}
