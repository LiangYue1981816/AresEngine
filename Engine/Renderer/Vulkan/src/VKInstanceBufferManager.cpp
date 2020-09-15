#include "VKRenderer.h"
#include "ResourceLoader.h"


CVKInstanceBufferManager::CVKInstanceBufferManager(CVKDevice* pDevice)
	: m_pDevice(pDevice)
{

}

CVKInstanceBufferManager::~CVKInstanceBufferManager(void)
{
	for (const auto& itInstanceBuffer : m_pInstanceBuffers) {
		delete itInstanceBuffer.second;
	}
}

CVKInstanceBuffer* CVKInstanceBufferManager::Create(uint32_t instanceFormat, int instanceBinding)
{
	mutex_autolock autolock(&lock);
	{
		if (CVKInstanceBuffer* pInstanceBuffer = new CVKInstanceBuffer(m_pDevice, this, instanceFormat, instanceBinding)) {
			m_pInstanceBuffers[pInstanceBuffer] = pInstanceBuffer;
			return pInstanceBuffer;
		}
		else {
			return nullptr;
		}
	}
}

void CVKInstanceBufferManager::Destroy(CVKInstanceBuffer* pInstanceBuffer)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pInstanceBuffers.find(pInstanceBuffer) != m_pInstanceBuffers.end()) {
			m_pInstanceBuffers.erase(pInstanceBuffer);
			delete pInstanceBuffer;
		}
	}
}
