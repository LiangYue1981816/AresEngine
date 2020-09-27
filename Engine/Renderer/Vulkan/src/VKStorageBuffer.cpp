#include "VKRenderer.h"


CVKStorageBuffer::CVKStorageBuffer(CVKDevice* pDevice, size_t size, bool bDynamic)
	: m_pDevice(pDevice)
	, m_pBuffer(nullptr)
{
	size = ALIGN_BYTE(size, m_pDevice->GetPhysicalDeviceLimits().nonCoherentAtomSize);
	size = ALIGN_BYTE(size, m_pDevice->GetPhysicalDeviceLimits().minStorageBufferOffsetAlignment);

	if (bDynamic) {
		m_pBuffer = new CVKBuffer(m_pDevice, size, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, GFX_BUFFER_STORAGE_BUFFER);
	}
	else {
		m_pBuffer = new CVKBuffer(m_pDevice, size, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, GFX_BUFFER_STORAGE_BUFFER);
	}
}

CVKStorageBuffer::~CVKStorageBuffer(void)
{
	delete m_pBuffer;
}

void CVKStorageBuffer::Release(void)
{
	delete this;
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

size_t CVKStorageBuffer::GetSize(void) const
{
	return m_pBuffer->GetBufferSize();
}

bool CVKStorageBuffer::BufferData(size_t offset, size_t size, const void* data)
{
	return m_pBuffer->BufferData(offset, size, data);
}
