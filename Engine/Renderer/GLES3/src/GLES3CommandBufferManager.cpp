#include "GLES3Renderer.h"


CGLES3CommandBufferManager::CGLES3CommandBufferManager(void)
{

}

CGLES3CommandBufferManager::~CGLES3CommandBufferManager(void)
{
	for (const auto& itCommandBuffer : m_pCommandBuffers) {
		delete itCommandBuffer.second;
	}
}

CGLES3CommandBuffer* CGLES3CommandBufferManager::Create(bool bMainCommandBuffer)
{
	mutex_autolock autolock(&lock);
	{
		if (CGLES3CommandBuffer* pCommandBuffer = new CGLES3CommandBuffer(this, bMainCommandBuffer)) {
			m_pCommandBuffers[pCommandBuffer] = pCommandBuffer;
			return pCommandBuffer;
		}
		else {
			return nullptr;
		}
	}
}

void CGLES3CommandBufferManager::Destroy(CGLES3CommandBuffer* pCommandBuffer)
{
	ASSERT(pCommandBuffer);
	{
		mutex_autolock autolock(&lock);
		{
			if (m_pCommandBuffers.find(pCommandBuffer) != m_pCommandBuffers.end()) {
				m_pCommandBuffers.erase(pCommandBuffer);
			}
		}
	}
	delete pCommandBuffer;
}
