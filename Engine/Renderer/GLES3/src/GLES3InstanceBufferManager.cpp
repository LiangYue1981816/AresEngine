#include "GLES3Renderer.h"
#include "ResourceLoader.h"


CGLES3InstanceBufferManager::CGLES3InstanceBufferManager(void)
{

}

CGLES3InstanceBufferManager::~CGLES3InstanceBufferManager(void)
{
	for (const auto& itInstanceBuffer : m_pInstanceBuffers) {
		delete itInstanceBuffer.second;
	}

	for (const auto& itMultiInstanceBuffer : m_pMultiInstanceBuffers) {
		delete itMultiInstanceBuffer.second;
	}
}

CGLES3InstanceBuffer* CGLES3InstanceBufferManager::Create(uint32_t instanceFormat, int instanceBinding)
{
	mutex_autolock autolock(&lock);
	{
		if (CGLES3InstanceBuffer* pInstanceBuffer = new CGLES3InstanceBuffer(this, instanceFormat, instanceBinding)) {
			m_pInstanceBuffers[pInstanceBuffer] = pInstanceBuffer;
			return pInstanceBuffer;
		}
		else {
			return nullptr;
		}
	}
}

CGLES3MultiInstanceBuffer* CGLES3InstanceBufferManager::Create(uint32_t instanceFormat, int instanceBinding, int count)
{
	mutex_autolock autolock(&lock);
	{
		if (CGLES3MultiInstanceBuffer* pMultiInstanceBuffer = new CGLES3MultiInstanceBuffer(this, instanceFormat, instanceBinding, count)) {
			m_pMultiInstanceBuffers[pMultiInstanceBuffer] = pMultiInstanceBuffer;
			return pMultiInstanceBuffer;
		}
		else {
			return nullptr;
		}
	}
}

void CGLES3InstanceBufferManager::Destroy(CGLES3InstanceBuffer* pInstanceBuffer)
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

void CGLES3InstanceBufferManager::Destroy(CGLES3MultiInstanceBuffer* pMultiInstanceBuffer)
{
	ASSERT(pMultiInstanceBuffer);
	{
		mutex_autolock autolock(&lock);
		{
			if (m_pMultiInstanceBuffers.find(pMultiInstanceBuffer) != m_pMultiInstanceBuffers.end()) {
				m_pMultiInstanceBuffers.erase(pMultiInstanceBuffer);
			}
		}
	}
	delete pMultiInstanceBuffer;
}
