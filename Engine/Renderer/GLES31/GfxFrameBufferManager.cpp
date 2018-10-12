#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxFrameBufferManager.h"


CGfxFrameBufferManager::CGfxFrameBufferManager(void)
{

}

CGfxFrameBufferManager::~CGfxFrameBufferManager(void)
{
	for (const auto &itFrameBuffer : m_pFrameBuffers) {
		delete itFrameBuffer.second;
	}

	m_pFrameBuffers.clear();
}

CGfxFrameBuffer* CGfxFrameBufferManager::CreateFrameBuffer(uint32_t width, uint32_t height)
{
	CGfxFrameBuffer *pFrameBuffer = new CGfxFrameBuffer(width, height);
	m_pFrameBuffers[pFrameBuffer] = pFrameBuffer;
	return m_pFrameBuffers[pFrameBuffer];
}

void CGfxFrameBufferManager::DestroyFrameBuffer(CGfxFrameBuffer *pFrameBuffer)
{
	if (pFrameBuffer) {
		m_pFrameBuffers.erase(pFrameBuffer);
		delete pFrameBuffer;
	}
}
