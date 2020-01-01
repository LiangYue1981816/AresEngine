#include "VKRenderer.h"


CVKStorageBufferManager::CVKStorageBufferManager(CVKDevice* pDevice)
	: m_pDevice(pDevice)
{

}

CVKStorageBufferManager::~CVKStorageBufferManager(void)
{
	for (const auto& itStorageBuffers : m_pStorageBuffers) {
		delete itStorageBuffers.second;
	}
}

CVKStorageBuffer* CVKStorageBufferManager::Create(size_t size)
{
	mutex_autolock autolock(&lock);
	{
		CVKStorageBuffer* pStorageBuffer = new CVKStorageBuffer(m_pDevice, this, ALIGN_BYTE(size, m_pDevice->GetPhysicalDeviceLimits().minStorageBufferOffsetAlignment));
		m_pStorageBuffers[pStorageBuffer] = pStorageBuffer;

		return pStorageBuffer;
	}
}

void CVKStorageBufferManager::Destroy(CVKStorageBuffer* pStorageBuffer)
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
