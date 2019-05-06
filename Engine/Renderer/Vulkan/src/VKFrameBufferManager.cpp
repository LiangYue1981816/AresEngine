#include "VKRenderer.h"


CVKFrameBufferManager::CVKFrameBufferManager(CVKDevice* pDevice)
	: m_pDevice(pDevice)
{

}

CVKFrameBufferManager::~CVKFrameBufferManager(void)
{
	for (const auto& itFrameBuffer : m_pFrameBuffers) {
		delete itFrameBuffer.second;
	}
}

CVKFrameBuffer* CVKFrameBufferManager::Create(int width, int height, int numAttachments)
{
	mutex_autolock autolock(&lock);
	{
		CVKFrameBuffer* pFrameBuffer = new CVKFrameBuffer(m_pDevice, this, width, height, numAttachments);
		m_pFrameBuffers[pFrameBuffer] = pFrameBuffer;
		return pFrameBuffer;
	}
}

void CVKFrameBufferManager::Destroy(CVKFrameBuffer* pFrameBuffer)
{
	mutex_autolock autolock(&lock);
	{
		if (pFrameBuffer) {
			if (m_pFrameBuffers.find(pFrameBuffer) != m_pFrameBuffers.end()) {
				m_pFrameBuffers.erase(pFrameBuffer);
				delete pFrameBuffer;
			}
		}
	}
}
