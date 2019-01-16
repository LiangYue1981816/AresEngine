#include "GLES3Renderer.h"


CGLES3CommandBufferManager::CGLES3CommandBufferManager(void)
{

}

CGLES3CommandBufferManager::~CGLES3CommandBufferManager(void)
{
	for (const auto &itCommandBuffer : m_pCommandBuffers) {
		delete itCommandBuffer.second;
	}

	m_pCommandBuffers.clear();
}

CGLES3CommandBuffer* CGLES3CommandBufferManager::Create(bool bMainCommandBuffer)
{
	mutex_autolock autolock(&lock);
	{
		CGLES3CommandBuffer *pCommandBuffer = new CGLES3CommandBuffer(this, bMainCommandBuffer);
		m_pCommandBuffers[pCommandBuffer] = pCommandBuffer;
		return pCommandBuffer;
	}
}

void CGLES3CommandBufferManager::Destroy(CGLES3CommandBuffer *pCommandBuffer)
{
	mutex_autolock autolock(&lock);
	{
		if (pCommandBuffer) {
			m_pCommandBuffers.erase(pCommandBuffer);
			delete pCommandBuffer;
		}
	}
}
