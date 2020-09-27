#include "VKRenderer.h"


CVKTransferBuffer::CVKTransferBuffer(CVKDevice* pDevice, size_t size)
	: m_pDevice(pDevice)
	, m_pBuffer(nullptr)
{
	size = ALIGN_BYTE(size, m_pDevice->GetPhysicalDeviceLimits().nonCoherentAtomSize);

	m_pBuffer = new CVKBuffer(m_pDevice, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, GFX_BUFFER_TRANSFER_BNUFER);
}

CVKTransferBuffer::~CVKTransferBuffer(void)
{
	delete m_pBuffer;
}

void CVKTransferBuffer::Release(void)
{
	delete this;
}

VkBuffer CVKTransferBuffer::GetBuffer(void) const
{
	return m_pBuffer->GetBuffer();
}

VkBufferUsageFlags CVKTransferBuffer::GetBufferUsageFlags(void) const
{
	return m_pBuffer->GetBufferUsageFlags();
}

VkDeviceSize CVKTransferBuffer::GetBufferSize(void) const
{
	return m_pBuffer->GetBufferSize();
}

bool CVKTransferBuffer::BufferData(size_t offset, size_t size, const void* data)
{
	return m_pBuffer->BufferData(offset, size, data);
}
