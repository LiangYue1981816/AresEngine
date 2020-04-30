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

	for (const auto& itMultiInstanceBuffer : m_pMultiInstanceBuffers) {
		delete itMultiInstanceBuffer.second;
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

CVKMultiInstanceBuffer* CVKInstanceBufferManager::Create(uint32_t instanceFormat, int instanceBinding, int count)
{
	mutex_autolock autolock(&lock);
	{
		if (CVKMultiInstanceBuffer* pMultiInstanceBuffer = new CVKMultiInstanceBuffer(m_pDevice, this, instanceFormat, instanceBinding, count)) {
			m_pMultiInstanceBuffers[pMultiInstanceBuffer] = pMultiInstanceBuffer;
			return pMultiInstanceBuffer;
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

void CVKInstanceBufferManager::Destroy(CVKMultiInstanceBuffer* pMultiInstanceBuffer)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pMultiInstanceBuffers.find(pMultiInstanceBuffer) != m_pMultiInstanceBuffers.end()) {
			m_pMultiInstanceBuffers.erase(pMultiInstanceBuffer);
			delete pMultiInstanceBuffer;
		}
	}
}
