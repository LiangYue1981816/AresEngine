#include "VKRenderer.h"


CVKIndirectBuffer::CVKIndirectBuffer(CVKDevice* pDevice)
	: m_pDevice(pDevice)
	, m_pBuffer(nullptr)

	, m_count(0)
{
	size_t size;
	size = sizeof(DrawCommand);
	size = ALIGN_BYTE(size, m_pDevice->GetPhysicalDeviceLimits().nonCoherentAtomSize);

	m_pBuffer = new CVKBuffer(m_pDevice, size, VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, GFX_BUFFER_INDIRECT_BUFFER);
}

CVKIndirectBuffer::~CVKIndirectBuffer(void)
{
	delete m_pBuffer;
}

void CVKIndirectBuffer::Release(void)
{
	delete this;
}

VkBuffer CVKIndirectBuffer::GetBuffer(void) const
{
	return m_pBuffer->GetBuffer();
}

VkBufferUsageFlags CVKIndirectBuffer::GetBufferUsageFlags(void) const
{
	return m_pBuffer->GetBufferUsageFlags();
}

VkDeviceSize CVKIndirectBuffer::GetBufferSize(void) const
{
	return m_pBuffer->GetBufferSize();
}

uint32_t CVKIndirectBuffer::GetOffset(int indexDraw) const
{
	return indexDraw * sizeof(DrawCommand);
}

uint32_t CVKIndirectBuffer::GetCount(void) const
{
	return m_count;
}

bool CVKIndirectBuffer::CopyDataToDevice(uint32_t count, const DrawCommand* data)
{
	m_count = count;

	if (m_pBuffer->GetBufferSize() < sizeof(DrawCommand) * count) {
		delete m_pBuffer;

		size_t newSize = 1;
		while (newSize < sizeof(DrawCommand) * count) newSize <<= 1;

		newSize = ALIGN_BYTE(newSize, m_pDevice->GetPhysicalDeviceLimits().nonCoherentAtomSize);
		m_pBuffer = new CVKBuffer(m_pDevice, newSize, VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, GFX_BUFFER_INDIRECT_BUFFER);
	}

	return m_pBuffer->CopyDataToDevice(0, sizeof(DrawCommand) * count, data);
}
