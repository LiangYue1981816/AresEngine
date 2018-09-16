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

CGfxFrameBuffer* CGfxFrameBufferManager::CreateFrameBuffer(GLuint width, GLuint height)
{
	CGfxFrameBuffer *pFrameBuffer = new CGfxFrameBuffer(width, height);
	m_pFrameBuffers[pFrameBuffer] = pFrameBuffer;
	return m_pFrameBuffers[pFrameBuffer];
}

void CGfxFrameBufferManager::DestroyFrameBuffer(CGfxFrameBuffer *pFrameBuffer)
{
	if (pFrameBuffer && pFrameBuffer->refCount == 0) {
		m_pFrameBuffers.erase(pFrameBuffer);
		delete pFrameBuffer;
	}
}
