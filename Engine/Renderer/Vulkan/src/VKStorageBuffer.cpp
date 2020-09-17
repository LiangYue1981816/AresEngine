#include "VKRenderer.h"


CVKStorageBuffer::CVKStorageBuffer(CVKDevice* pDevice, CVKStorageBufferManager* pManager, size_t size)
	: CGfxStorageBuffer(size)
	, m_pManager(pManager)
	, m_pDevice(pDevice)
	, m_pBuffer(nullptr)
{
	size = ALIGN_BYTE(size, m_pDevice->GetPhysicalDeviceLimits().nonCoherentAtomSize);
	size = ALIGN_BYTE(size, m_pDevice->GetPhysicalDeviceLimits().minStorageBufferOffsetAlignment);

	m_pBuffer = new CVKBuffer(m_pDevice, size, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
	CGfxProfiler::IncStorageBufferSize(m_pBuffer->GetMemorySize());
}

CVKStorageBuffer::~CVKStorageBuffer(void)
{
	CGfxProfiler::DecStorageBufferSize(m_pBuffer->GetMemorySize());
	delete m_pBuffer;
}

void CVKStorageBuffer::Release(void)
{
	if (m_pManager) {
		m_pManager->Destroy(this);
	}
	else {
		delete this;
	}
}

VkBuffer CVKStorageBuffer::GetBuffer(void) const
{
	return m_pBuffer->GetBuffer();
}

VkBufferUsageFlags CVKStorageBuffer::GetBufferUsageFlags(void) const
{
	return m_pBuffer->GetBufferUsageFlags();
}

VkDeviceSize CVKStorageBuffer::GetBufferSize(void) const
{
	return m_pBuffer->GetBufferSize();
}

VkDeviceSize CVKStorageBuffer::GetMemorySize(void) const
{
	return m_pBuffer->GetMemorySize();
}

size_t CVKStorageBuffer::GetSize(void) const
{
	return m_pBuffer->GetBufferSize();
}

bool CVKStorageBuffer::BufferData(size_t offset, size_t size, const void* data)
{
	return m_pBuffer->BufferData(offset, size, data);
}
