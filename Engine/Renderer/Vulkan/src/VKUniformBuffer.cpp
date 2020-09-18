#include "VKRenderer.h"


CVKUniformBuffer::CVKUniformBuffer(CVKDevice* pDevice, size_t size, bool bDynamic)
	: CGfxUniformBuffer(size)
	, m_pDevice(pDevice)
	, m_pBuffer(nullptr)
{
	size = ALIGN_BYTE(size, m_pDevice->GetPhysicalDeviceLimits().nonCoherentAtomSize);
	size = ALIGN_BYTE(size, m_pDevice->GetPhysicalDeviceLimits().minUniformBufferOffsetAlignment);

	if (bDynamic) {
		m_pBuffer = new CVKBuffer(m_pDevice, size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, GFX_BUFFER_UNIFORM_BUFFER);
	}
	else {
		m_pBuffer = new CVKBuffer(m_pDevice, size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, GFX_BUFFER_UNIFORM_BUFFER);
	}
}

CVKUniformBuffer::~CVKUniformBuffer(void)
{
	delete m_pBuffer;
}

void CVKUniformBuffer::Release(void)
{
	delete this;
}

VkBuffer CVKUniformBuffer::GetBuffer(void) const
{
	return m_pBuffer->GetBuffer();
}

VkBufferUsageFlags CVKUniformBuffer::GetBufferUsageFlags(void) const
{
	return m_pBuffer->GetBufferUsageFlags();
}

VkDeviceSize CVKUniformBuffer::GetBufferSize(void) const
{
	return m_pBuffer->GetBufferSize();
}

size_t CVKUniformBuffer::GetSize(void) const
{
	return m_pBuffer->GetBufferSize();
}

bool CVKUniformBuffer::BufferData(size_t offset, size_t size, const void* data)
{
	return m_pBuffer->BufferData(offset, size, data);
}
