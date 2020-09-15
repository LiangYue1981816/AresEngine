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

void CGLES3InstanceBufferManager::Destroy(CGLES3InstanceBuffer* pInstanceBuffer)
{
	mutex_autolock autolock(&lock);
	{
		if (m_pInstanceBuffers.find(pInstanceBuffer) != m_pInstanceBuffers.end()) {
			m_pInstanceBuffers.erase(pInstanceBuffer);
			delete pInstanceBuffer;
		}
	}
}
