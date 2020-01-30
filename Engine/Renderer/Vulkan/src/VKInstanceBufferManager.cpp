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

CVKMultiInstanceBuffer* CVKInstanceBufferManager::Create(uint32_t instanceFormat, int instanceBinding, int count)
{
	mutex_autolock autolock(&lock);
	{
		if (CVKMultiInstanceBuffer* pInstanceBuffer = new CVKMultiInstanceBuffer(m_pDevice, instanceFormat, instanceBinding, count)) {
			m_pInstanceBuffers[pInstanceBuffer] = pInstanceBuffer;
			return pInstanceBuffer;
		}
		else {
			return nullptr;
		}
	}
}

void CVKInstanceBufferManager::Destroy(CVKMultiInstanceBuffer* pInstanceBuffer)
{
	ASSERT(pInstanceBuffer);
	{
		mutex_autolock autolock(&lock);
		{
			if (m_pInstanceBuffers.find(pInstanceBuffer) != m_pInstanceBuffers.end()) {
				m_pInstanceBuffers.erase(pInstanceBuffer);
			}
		}
	}
	delete pInstanceBuffer;
}
