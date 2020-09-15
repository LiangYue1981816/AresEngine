#include "VKRenderer.h"


CVKIndirectBuffer::CVKIndirectBuffer(CVKDevice* pDevice, int numDrawCommands)
	: CGfxIndirectBuffer(numDrawCommands)
	, m_pDevice(pDevice)
	, m_pBuffer(nullptr)

	, m_draws(numDrawCommands)
{
	size_t size;
	size = sizeof(DrawCommand);
	size = ALIGN_BYTE(size, m_pDevice->GetPhysicalDeviceLimits().nonCoherentAtomSize) * numDrawCommands;

	m_pBuffer = new CVKBuffer(m_pDevice, size, VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
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

uint32_t CVKIndirectBuffer::GetDrawCommandCount(void) const
{
	return m_draws.size();
}

uint32_t CVKIndirectBuffer::GetDrawCommandOffset(int indexDraw) const
{
	return indexDraw * sizeof(DrawCommand);
}

uint32_t CVKIndirectBuffer::GetSize(void) const
{
	return m_pBuffer->GetSize();
}

uint32_t CVKIndirectBuffer::GetStride(void) const
{
	return sizeof(DrawCommand);
}

bool CVKIndirectBuffer::BufferData(int indexDraw, int firstIndex, int baseVertex, int baseInstance, int indexCount, int instanceCount)
{
	if (indexDraw < 0 || (uint32_t)indexDraw >= m_draws.size()) {
		return false;
	}

	if (m_draws[indexDraw].indexCount == indexCount &&
		m_draws[indexDraw].instanceCount == instanceCount &&
		m_draws[indexDraw].firstIndex == firstIndex &&
		m_draws[indexDraw].baseVertex == baseVertex &&
		m_draws[indexDraw].baseInstance == baseInstance) {
		return true;
	}
	else {
		m_draws[indexDraw].Set(indexCount, instanceCount, firstIndex, baseVertex, baseInstance);
		return m_pBuffer->BufferData(indexDraw * sizeof(DrawCommand), sizeof(m_draws[indexDraw]), &m_draws[indexDraw]);
	}
}
