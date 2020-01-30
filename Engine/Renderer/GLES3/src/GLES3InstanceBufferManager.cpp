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
}

CGLES3MultiInstanceBuffer* CGLES3InstanceBufferManager::Create(uint32_t instanceFormat, int instanceBinding, int count)
{
	mutex_autolock autolock(&lock);
	{
		if (CGLES3MultiInstanceBuffer* pInstanceBuffer = new CGLES3MultiInstanceBuffer(instanceFormat, instanceBinding, count)) {
			m_pInstanceBuffers[pInstanceBuffer] = pInstanceBuffer;
			return pInstanceBuffer;
		}
		else {
			return nullptr;
		}
	}
}

void CGLES3InstanceBufferManager::Destroy(CGLES3MultiInstanceBuffer* pInstanceBuffer)
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
