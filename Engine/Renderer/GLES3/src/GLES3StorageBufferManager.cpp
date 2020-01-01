#include "GLES3Renderer.h"


CGLES3StorageBufferManager::CGLES3StorageBufferManager(void)
{

}

CGLES3StorageBufferManager::~CGLES3StorageBufferManager(void)
{
	for (const auto& itStorageBuffer : m_pStorageBuffers) {
		delete itStorageBuffer.second;
	}
}

CGLES3StorageBuffer* CGLES3StorageBufferManager::Create(size_t size)
{
	mutex_autolock autolock(&lock);
	{
		if (CGLES3StorageBuffer* pStorageBuffer = new CGLES3StorageBuffer(this, size)) {
			m_pStorageBuffers[pStorageBuffer] = pStorageBuffer;
			return pStorageBuffer;
		}
		else {
			return nullptr;
		}
	}
}

void CGLES3StorageBufferManager::Destroy(CGLES3StorageBuffer* pStorageBuffer)
{
	ASSERT(pStorageBuffer);
	{
		mutex_autolock autolock(&lock);
		{
			if (m_pStorageBuffers.find(pStorageBuffer) != m_pStorageBuffers.end()) {
				m_pStorageBuffers.erase(pStorageBuffer);
			}
		}
	}
	delete pStorageBuffer;
}
