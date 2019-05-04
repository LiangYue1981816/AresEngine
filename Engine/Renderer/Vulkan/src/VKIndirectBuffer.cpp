#include "VKRenderer.h"


CVKIndirectBuffer::CVKIndirectBuffer(CVKDevice* pDevice, uint32_t numDrawCommands)
	: CGfxIndirectBuffer(numDrawCommands)
	, m_pDevice(pDevice)
	, m_draws(numDrawCommands)
{
	m_ptrBuffer = CVKBufferPtr(new CVKBuffer(pDevice, CGfxSwapChain::SWAPCHAIN_FRAME_COUNT * numDrawCommands * sizeof(DrawCommand), VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));
	CGfxProfiler::IncIndirectBufferSize(m_ptrBuffer->GetSize());
}

CVKIndirectBuffer::~CVKIndirectBuffer(void)
{
	CGfxProfiler::DecIndirectBufferSize(m_ptrBuffer->GetSize());
}

VkBuffer CVKIndirectBuffer::GetBuffer(void) const
{
	return m_ptrBuffer->GetBuffer();
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
	return m_draws.size() * sizeof(DrawCommand);
}

bool CVKIndirectBuffer::BufferData(int indexDraw, int baseVertex, int firstIndex, int indexCount, int instanceCount)
{
	if (indexDraw < 0 || (uint32_t)indexDraw >= m_draws.size()) {
		return false;
	}

	if (m_draws[indexDraw].baseVertex == baseVertex &&
		m_draws[indexDraw].firstIndex == firstIndex &&
		m_draws[indexDraw].indexCount == indexCount &&
		m_draws[indexDraw].instanceCount == instanceCount) {
		return true;
	}

	m_draws[indexDraw].baseVertex = baseVertex;
	m_draws[indexDraw].firstIndex = firstIndex;
	m_draws[indexDraw].indexCount = indexCount;
	m_draws[indexDraw].instanceCount = instanceCount;

	return m_ptrBuffer->BufferData(VKRenderer()->GetSwapChain()->GetFrameIndex() * m_size + indexDraw * sizeof(DrawCommand), sizeof(m_draws[indexDraw]), &m_draws[indexDraw]);
}
