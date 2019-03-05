#include "VKRenderer.h"


CVKIndirectBuffer::CVKIndirectBuffer(CVKDevice *pDevice, uint32_t drawCommandCount)
	: CGfxIndirectBuffer(drawCommandCount)
	, m_pDevice(pDevice)

	, m_size(drawCommandCount * sizeof(DrawCommand))
	, m_count(drawCommandCount)
{
	m_draws.resize(m_count);
	m_ptrBuffer = CVKBufferPtr(new CVKBuffer(m_pDevice, CGfxSwapChain::SWAPCHAIN_FRAME_COUNT * m_size, VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));
	CGfxProfiler::IncIndirectBufferSize(m_ptrBuffer->GetSize());
}

CVKIndirectBuffer::~CVKIndirectBuffer(void)
{
	CGfxProfiler::DecIndirectBufferSize(m_ptrBuffer->GetSize());
}

uint32_t CVKIndirectBuffer::GetDrawCommandCount(void) const
{
	return m_count;
}

uint32_t CVKIndirectBuffer::GetDrawCommandOffset(int indexDraw) const
{
	return indexDraw * sizeof(DrawCommand);
}

uint32_t CVKIndirectBuffer::GetSize(void) const
{
	return m_size;
}

bool CVKIndirectBuffer::BufferData(int indexDraw, int instanceCount)
{
	if (indexDraw < 0 || (uint32_t)indexDraw >= m_count) {
		return false;
	}

	if (m_draws[indexDraw].instanceCount != instanceCount) {
		m_draws[indexDraw].instanceCount  = instanceCount;
		return m_ptrBuffer->BufferData(VKRenderer()->GetSwapChain()->GetFrameIndex() * m_size + indexDraw * sizeof(DrawCommand) + offsetof(DrawCommand, instanceCount), sizeof(instanceCount), &instanceCount);
	}

	return true;
}

bool CVKIndirectBuffer::BufferData(int indexDraw, int baseVertex, int firstIndex, int indexCount, int instanceCount)
{
	if (indexDraw < 0 || (uint32_t)indexDraw >= m_count) {
		return false;
	}

	if (m_draws[indexDraw].baseVertex != baseVertex ||
		m_draws[indexDraw].firstIndex != firstIndex ||
		m_draws[indexDraw].indexCount != indexCount ||
		m_draws[indexDraw].instanceCount != instanceCount) {
		m_draws[indexDraw].baseVertex = baseVertex;
		m_draws[indexDraw].firstIndex = firstIndex;
		m_draws[indexDraw].indexCount = indexCount;
		m_draws[indexDraw].instanceCount = instanceCount;
		return m_ptrBuffer->BufferData(VKRenderer()->GetSwapChain()->GetFrameIndex() * m_size + indexDraw * sizeof(DrawCommand), sizeof(m_draws[indexDraw]), &m_draws[indexDraw]);
	}

	return true;
}
