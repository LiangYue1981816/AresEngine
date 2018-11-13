#include "GLES3Renderer.h"
#include "GLES3FrameBufferManager.h"


CGLES3FrameBufferManager::CGLES3FrameBufferManager(void)
{

}

CGLES3FrameBufferManager::~CGLES3FrameBufferManager(void)
{
	for (const auto &itFrameBuffer : m_pFrameBuffers) {
		delete itFrameBuffer.second;
	}

	m_pFrameBuffers.clear();
}

CGLES3FrameBuffer* CGLES3FrameBufferManager::CreateFrameBuffer(int width, int height)
{
	mutex_autolock mutex(&lock);
	{
		CGLES3FrameBuffer *pFrameBuffer = new CGLES3FrameBuffer(this, width, height);
		m_pFrameBuffers[pFrameBuffer] = pFrameBuffer;
		return pFrameBuffer;
	}
}

void CGLES3FrameBufferManager::DestroyFrameBuffer(CGLES3FrameBuffer *pFrameBuffer)
{
	mutex_autolock mutex(&lock);
	{
		if (pFrameBuffer) {
			m_pFrameBuffers.erase(pFrameBuffer);
			delete pFrameBuffer;
		}
	}
}
