#include "GLES3Renderer.h"
#include "GLES3CommandBufferManager.h"


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

CGLES3CommandBuffer* CGLES3CommandBufferManager::CreateCommandBuffer(bool bMainCommandBuffer)
{
	CGLES3CommandBuffer *pCommandBuffer = new CGLES3CommandBuffer(this, bMainCommandBuffer);
	m_pCommandBuffers[pCommandBuffer] = pCommandBuffer;
	return pCommandBuffer;
}

void CGLES3CommandBufferManager::DestroyCommandBuffer(CGLES3CommandBuffer *pCommandBuffer)
{
	if (pCommandBuffer) {
		m_pCommandBuffers.erase(pCommandBuffer);
		delete pCommandBuffer;
	}
}
