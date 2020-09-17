#include "VKRenderer.h"


CVKIndirectBuffer::CVKIndirectBuffer(CVKDevice* pDevice, int numDrawCommands, bool bDynamic)
	: CGfxIndirectBuffer(numDrawCommands)
	, m_pDevice(pDevice)
	, m_pBuffer(nullptr)

	, m_count(numDrawCommands)
{
	size_t size;
	size = sizeof(DrawCommand);
	size = ALIGN_BYTE(size, m_pDevice->GetPhysicalDeviceLimits().nonCoherentAtomSize) * numDrawCommands;

	if (bDynamic) {
		m_pBuffer = new CVKBuffer(m_pDevice, size, VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
	}
	else {
		m_pBuffer = new CVKBuffer(m_pDevice, size, VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	}

	CGfxProfiler::IncIndirectBufferSize(m_pBuffer->GetMemorySize());
}

CVKIndirectBuffer::~CVKIndirectBuffer(void)
{
	CGfxProfiler::DecIndirectBufferSize(m_pBuffer->GetMemorySize());
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

VkDeviceSize CVKIndirectBuffer::GetMemorySize(void) const
{
	return m_pBuffer->GetMemorySize();
}

uint32_t CVKIndirectBuffer::GetDrawCommandCount(void) const
{
	return m_count;
}

uint32_t CVKIndirectBuffer::GetDrawCommandOffset(int indexDraw) const
{
	return indexDraw * sizeof(DrawCommand);
}

uint32_t CVKIndirectBuffer::GetStride(void) const
{
	return sizeof(DrawCommand);
}

bool CVKIndirectBuffer::BufferData(int indexDraw, int firstIndex, int baseVertex, int baseInstance, int indexCount, int instanceCount)
{
	if (indexDraw < 0 || (uint32_t)indexDraw >= m_count) {
		return false;
	}

	DrawCommand drawCommand;
	drawCommand.Set(indexCount, instanceCount, firstIndex, baseVertex, baseInstance);

	return m_pBuffer->BufferData(indexDraw * sizeof(DrawCommand), sizeof(DrawCommand), &drawCommand);
}
