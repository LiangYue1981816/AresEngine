#include "GLES3Renderer.h"


CGLES3FrameBufferManager::CGLES3FrameBufferManager(void)
{

}

CGLES3FrameBufferManager::~CGLES3FrameBufferManager(void)
{
	for (const auto& itFrameBuffer : m_pFrameBuffers) {
		delete itFrameBuffer.second;
	}
}

CGLES3FrameBuffer* CGLES3FrameBufferManager::Create(int width, int height, int numAttachments)
{
	mutex_autolock autolock(&lock);
	{
		if (CGLES3FrameBuffer* pFrameBuffer = new CGLES3FrameBuffer(this, width, height, numAttachments)) {
			m_pFrameBuffers[pFrameBuffer] = pFrameBuffer;
			return pFrameBuffer;
		}
		else {
			return nullptr;
		}
	}
}

void CGLES3FrameBufferManager::Destroy(CGLES3FrameBuffer* pFrameBuffer)
{
	ASSERT(pFrameBuffer);
	{
		mutex_autolock autolock(&lock);
		{
			if (m_pFrameBuffers.find(pFrameBuffer) != m_pFrameBuffers.end()) {
				m_pFrameBuffers.erase(pFrameBuffer);
			}
		}
	}
	delete pFrameBuffer;
}
