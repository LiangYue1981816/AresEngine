#include "VKRenderer.h"


static uint32_t VertexCount(uint32_t vertexFormat, size_t size)
{
	return size / GetVertexStride(vertexFormat);
}

CVKVertexBuffer::CVKVertexBuffer(CVKDevice* pDevice, uint32_t vertexFormat, int vertexBinding, size_t size, bool bDynamic)
	: CGfxVertexBuffer(vertexFormat, vertexBinding, size, bDynamic)
	, m_pDevice(pDevice)
	, m_pBuffer(nullptr)

	, m_binding(vertexBinding)
	, m_format(vertexFormat)
	, m_count(VertexCount(vertexFormat, size))
{
	size = ALIGN_BYTE(size, m_pDevice->GetPhysicalDeviceLimits().nonCoherentAtomSize);

	if (bDynamic) {
		m_pBuffer = new CVKBuffer(m_pDevice, size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
	}
	else {
		m_pBuffer = new CVKBuffer(m_pDevice, size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	}

	CGfxProfiler::IncVertexBufferSize(m_pBuffer->GetMemorySize());
}

CVKVertexBuffer::~CVKVertexBuffer(void)
{
	CGfxProfiler::DecVertexBufferSize(m_pBuffer->GetMemorySize());
	delete m_pBuffer;
}

void CVKVertexBuffer::Release(void)
{
	delete this;
}

VkBuffer CVKVertexBuffer::GetBuffer(void) const
{
	return m_pBuffer->GetBuffer();
}

VkBufferUsageFlags CVKVertexBuffer::GetBufferUsageFlags(void) const
{
	return m_pBuffer->GetBufferUsageFlags();
}

VkDeviceSize CVKVertexBuffer::GetBufferSize(void) const
{
	return m_pBuffer->GetBufferSize();
}

VkDeviceSize CVKVertexBuffer::GetMemorySize(void) const
{
	return m_pBuffer->GetMemorySize();
}

uint32_t CVKVertexBuffer::GetVertexBinding(void) const
{
	return m_binding;
}

uint32_t CVKVertexBuffer::GetVertexFormat(void) const
{
	return m_format;
}

uint32_t CVKVertexBuffer::GetVertexCount(void) const
{
	return m_count;
}

bool CVKVertexBuffer::BufferData(size_t offset, size_t size, const void* data)
{
	return m_pBuffer->BufferData(offset, size, data);
}
