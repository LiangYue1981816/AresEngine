#include "GLES3Renderer.h"


CGLES3UniformBufferManager::CGLES3UniformBufferManager(void)
{

}

CGLES3UniformBufferManager::~CGLES3UniformBufferManager(void)
{
	for (const auto& UniformBuffer : m_pUniformBuffers) {
		delete UniformBuffer.second;
	}
}

CGLES3UniformBuffer* CGLES3UniformBufferManager::Create(size_t size)
{
	mutex_autolock autolock(&lock);
	{
		CGLES3UniformBuffer* pUniformBuffer = new CGLES3UniformBuffer(this, size);
		m_pUniformBuffers[pUniformBuffer] = pUniformBuffer;
		return pUniformBuffer;
	}
}

void CGLES3UniformBufferManager::Destroy(CGLES3UniformBuffer* pUniformBuffer)
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
