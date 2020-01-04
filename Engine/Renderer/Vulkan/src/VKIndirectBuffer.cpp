#include "VKRenderer.h"


CVKIndirectBuffer::CVKIndirectBuffer(CVKDevice* pDevice, int numDrawCommands)
	: CGfxIndirectBuffer(numDrawCommands)
	, m_pDevice(pDevice)
	, m_pBuffer(nullptr)

	, m_size(0)
	, m_draws(numDrawCommands)
{
	m_size = sizeof(DrawCommand);
	m_size = ALIGN_BYTE(m_size, m_pDevice->GetPhysicalDeviceLimits().nonCoherentAtomSize) * numDrawCommands;

	m_pBuffer = new CVKBuffer(m_pDevice, m_size, VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
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

	m_draws[indexDraw].indexCount = indexCount;
	m_draws[indexDraw].instanceCount = instanceCount;
	m_draws[indexDraw].firstIndex = firstIndex;
	m_draws[indexDraw].baseVertex = baseVertex;
	m_draws[indexDraw].baseInstance = baseInstance;

	return m_pBuffer->BufferData(indexDraw * sizeof(DrawCommand), sizeof(m_draws[indexDraw]), &m_draws[indexDraw]);
}


CVKMultiIndirectBuffer::CVKMultiIndirectBuffer(CVKDevice* pDevice, int numDrawCommands, int count)
	: CGfxMultiIndirectBuffer(numDrawCommands, count)
	, m_index(0)
	, m_pBuffers(std::max(1, count))
{
	for (int index = 0; index < m_pBuffers.size(); index++) {
		m_pBuffers[index] = new CVKIndirectBuffer(pDevice, numDrawCommands);
	}
}

CVKMultiIndirectBuffer::~CVKMultiIndirectBuffer(void)
{
	for (auto& itBuffer : m_pBuffers) {
		delete itBuffer;
	}
}

void CVKMultiIndirectBuffer::Release(void)
{
	delete this;
}

void CVKMultiIndirectBuffer::SetIndex(int index)
{
	m_index = index;
	m_index = std::min(m_index, (int)m_pBuffers.size() - 1);
	m_index = std::max(m_index, 0);
}

CVKIndirectBuffer* CVKMultiIndirectBuffer::GetBuffer(void) const
{
	return m_pBuffers[m_index];
}

uint32_t CVKMultiIndirectBuffer::GetDrawCommandCount(void) const
{
	return m_pBuffers[m_index]->GetDrawCommandCount();
}

uint32_t CVKMultiIndirectBuffer::GetDrawCommandOffset(int indexDraw) const
{
	return m_pBuffers[m_index]->GetDrawCommandOffset(indexDraw);
}

uint32_t CVKMultiIndirectBuffer::GetSize(void) const
{
	return m_pBuffers[m_index]->GetSize();
}

uint32_t CVKMultiIndirectBuffer::GetStride(void) const
{
	return m_pBuffers[m_index]->GetStride();
}

bool CVKMultiIndirectBuffer::BufferData(int indexDraw, int firstIndex, int baseVertex, int baseInstance, int indexCount, int instanceCount)
{
	return m_pBuffers[m_index]->BufferData(indexDraw, firstIndex, baseVertex, baseInstance, indexCount, instanceCount);
}
