#include "VKRenderer.h"


static uint32_t IndexCount(GfxIndexType type, size_t size)
{
	switch (type) {
	case GFX_INDEX_UINT16: return size / 2;
	case GFX_INDEX_UINT32: return size / 4;
	default:               return 0;
	}
}

CVKIndexBuffer::CVKIndexBuffer(CVKDevice* pDevice, GfxIndexType type, size_t size, bool bDynamic)
	: CGfxIndexBuffer(type, size, bDynamic)
	, m_pDevice(pDevice)
	, m_pBuffer(nullptr)

	, m_type(type)
	, m_count(IndexCount(type, size))
{
	size = ALIGN_BYTE(size, m_pDevice->GetPhysicalDeviceLimits().nonCoherentAtomSize);

	if (bDynamic) {
		m_pBuffer = new CVKBuffer(m_pDevice, size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, CGfxProfiler::BufferType::BUFFER_TYPE_INDEX_BUFFER);
	}
	else {
		m_pBuffer = new CVKBuffer(m_pDevice, size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, CGfxProfiler::BufferType::BUFFER_TYPE_INDEX_BUFFER);
	}
}

CVKIndexBuffer::~CVKIndexBuffer(void)
{
	delete m_pBuffer;
}

void CVKIndexBuffer::Release(void)
{
	delete this;
}

VkBuffer CVKIndexBuffer::GetBuffer(void) const
{
	return m_pBuffer->GetBuffer();
}

VkBufferUsageFlags CVKIndexBuffer::GetBufferUsageFlags(void) const
{
	return m_pBuffer->GetBufferUsageFlags();
}

VkDeviceSize CVKIndexBuffer::GetBufferSize(void) const
{
	return m_pBuffer->GetBufferSize();
}

GfxIndexType CVKIndexBuffer::GetType(void) const
{
	return m_type;
}

uint32_t CVKIndexBuffer::GetCount(void) const
{
	return m_count;
}

bool CVKIndexBuffer::BufferData(size_t offset, size_t size, const void* data)
{
	return m_pBuffer->BufferData(offset, size, data);
}
