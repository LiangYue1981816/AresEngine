#include "VKRenderer.h"


CVKFrameBufferManager::CVKFrameBufferManager(CVKDevice *pDevice)
	: m_pDevice(pDevice)
{

}

CVKFrameBufferManager::~CVKFrameBufferManager(void)
{
	for (const auto &itFrameBuffer : m_pFrameBuffers) {
		delete itFrameBuffer.second;
	}

	m_pFrameBuffers.clear();
}

CVKFrameBuffer* CVKFrameBufferManager::Create(int width, int height)
{
	mutex_autolock autolock(&lock);
	{
		CVKFrameBuffer *pFrameBuffer = new CVKFrameBuffer(m_pDevice, this, width, height);
		m_pFrameBuffers[pFrameBuffer] = pFrameBuffer;
		return pFrameBuffer;
	}
}

void CVKFrameBufferManager::Destroy(CGfxFrameBuffer *pFrameBuffer)
{
	mutex_autolock autolock(&lock);
	{
		if (pFrameBuffer) {
			m_pFrameBuffers.erase(pFrameBuffer);
			delete pFrameBuffer;
		}
	}
}
