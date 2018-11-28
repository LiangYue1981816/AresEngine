#include "GfxHeader.h"


CGLES3UniformBufferManager::CGLES3UniformBufferManager(void)
{

}

CGLES3UniformBufferManager::~CGLES3UniformBufferManager(void)
{
	for (const auto &UniformBuffer : m_pUniformBuffers) {
		delete UniformBuffer.second;
	}

	m_pUniformBuffers.clear();
}

CGLES3UniformBuffer* CGLES3UniformBufferManager::Create(size_t size, bool bDynamic)
{
	mutex_autolock autolock(&lock);
	{
		CGLES3UniformBuffer *pUniformBuffer = new CGLES3UniformBuffer(this, size, bDynamic);
		m_pUniformBuffers[pUniformBuffer] = pUniformBuffer;
		return pUniformBuffer;
	}
}

void CGLES3UniformBufferManager::Destroy(CGLES3UniformBuffer *pUniformBuffer)
{
	mutex_autolock autolock(&lock);
	{
		if (pUniformBuffer) {
			m_pUniformBuffers.erase(pUniformBuffer);
			delete pUniformBuffer;
		}
	}
}
