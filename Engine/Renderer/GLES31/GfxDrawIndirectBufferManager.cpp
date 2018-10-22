#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxDrawIndirectBufferManager.h"


CGfxDrawIndirectBufferManager::CGfxDrawIndirectBufferManager(void)
{

}

CGfxDrawIndirectBufferManager::~CGfxDrawIndirectBufferManager(void)
{
	for (const auto &itBuffer : m_pBuffers) {
		delete itBuffer.second;
	}

	m_pBuffers.clear();
}

CGfxDrawIndirectBuffer* CGfxDrawIndirectBufferManager::CreateDrawIndirectBuffer(int baseVertex, uint32_t firstIndex, uint32_t indexCount, uint32_t instanceCount)
{
	CGfxDrawIndirectBuffer *pBuffer = new CGfxDrawIndirectBuffer(baseVertex, firstIndex, indexCount, instanceCount);
	m_pBuffers[pBuffer] = pBuffer;
	return m_pBuffers[pBuffer];
}

void CGfxDrawIndirectBufferManager::DestroyDrawIndirectBuffer(CGfxDrawIndirectBuffer *pBuffer)
{
	if (pBuffer) {
		m_pBuffers.erase(pBuffer);
		delete pBuffer;
	}
}
