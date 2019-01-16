#include "GLES3Renderer.h"


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

CGLES3FrameBuffer* CGLES3FrameBufferManager::Create(int width, int height, int numAttachments)
{
	mutex_autolock autolock(&lock);
	{
		CGLES3FrameBuffer *pFrameBuffer = new CGLES3FrameBuffer(this, width, height, numAttachments);
		m_pFrameBuffers[pFrameBuffer] = pFrameBuffer;
		return pFrameBuffer;
	}
}

void CGLES3FrameBufferManager::Destroy(CGLES3FrameBuffer *pFrameBuffer)
{
	mutex_autolock autolock(&lock);
	{
		if (pFrameBuffer) {
			m_pFrameBuffers.erase(pFrameBuffer);
			delete pFrameBuffer;
		}
	}
}
