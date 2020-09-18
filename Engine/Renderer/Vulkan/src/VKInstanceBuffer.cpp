#include "VKRenderer.h"


CVKInstanceBuffer::CVKInstanceBuffer(CVKDevice* pDevice, uint32_t instanceFormat, int instanceBinding)
	: CGfxInstanceBuffer(instanceFormat, instanceBinding)
	, m_pDevice(pDevice)
	, m_pBuffer(nullptr)

	, m_binding(instanceBinding)
	, m_format(instanceFormat)
	, m_count(0)
{
	size_t size;
	size = GetInstanceStride(instanceFormat);
	size = ALIGN_BYTE(size, m_pDevice->GetPhysicalDeviceLimits().nonCoherentAtomSize);

	m_pBuffer = new CVKBuffer(m_pDevice, size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, GFX_BUFFER_INSTANCE_BUFFER);
}

CVKInstanceBuffer::~CVKInstanceBuffer(void)
{
	delete m_pBuffer;
}

void CVKInstanceBuffer::Release(void)
{
	delete this;
}

VkBuffer CVKInstanceBuffer::GetBuffer(void) const
{
	return m_pBuffer->GetBuffer();
}

VkBufferUsageFlags CVKInstanceBuffer::GetBufferUsageFlags(void) const
{
	return m_pBuffer->GetBufferUsageFlags();
}

VkDeviceSize CVKInstanceBuffer::GetBufferSize(void) const
{
	return m_pBuffer->GetBufferSize();
}

uint32_t CVKInstanceBuffer::GetBinding(void) const
{
	return m_binding;
}

uint32_t CVKInstanceBuffer::GetFormat(void) const
{
	return m_format;
}

uint32_t CVKInstanceBuffer::GetCount(void) const
{
	return m_count;
}

bool CVKInstanceBuffer::BufferData(size_t size, const void* data)
{
	m_count = size / GetInstanceStride(m_format);

	if (m_pBuffer->GetBufferSize() < size) {
		delete m_pBuffer;

		size_t newSize = 1;
		while (newSize < size) newSize <<= 1;

		newSize = ALIGN_BYTE(newSize, m_pDevice->GetPhysicalDeviceLimits().nonCoherentAtomSize);
		m_pBuffer = new CVKBuffer(m_pDevice, newSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, GFX_BUFFER_INSTANCE_BUFFER);
	}

	return m_pBuffer->BufferData(0, size, data);
}
