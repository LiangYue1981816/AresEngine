#include "GLES3Renderer.h"


CGLES3IndirectBuffer::CGLES3IndirectBuffer(uint32_t numDrawCommands)
	: CGfxIndirectBuffer(numDrawCommands)
{
	m_draws.resize(numDrawCommands);
	m_ptrBuffer = CGLES3BufferPtr(new CGLES3Buffer(GL_DRAW_INDIRECT_BUFFER, numDrawCommands * sizeof(DrawCommand), true));
	CGfxProfiler::IncIndirectBufferSize(m_ptrBuffer->GetSize());
}

CGLES3IndirectBuffer::~CGLES3IndirectBuffer(void)
{
	CGfxProfiler::DecIndirectBufferSize(m_ptrBuffer->GetSize());
}

uint32_t CGLES3IndirectBuffer::GetDrawCommandCount(void) const
{
	return m_draws.size();
}

uint32_t CGLES3IndirectBuffer::GetDrawCommandOffset(int indexDraw) const
{
	return indexDraw * sizeof(DrawCommand);
}

uint32_t CGLES3IndirectBuffer::GetSize(void) const
{
	return m_ptrBuffer->GetSize();
}

bool CGLES3IndirectBuffer::BufferData(int indexDraw, int instanceCount)
{
	if (indexDraw < 0 || (uint32_t)indexDraw >= m_draws.size()) {
		return false;
	}

	if (m_draws[indexDraw].instanceCount != instanceCount) {
		m_draws[indexDraw].instanceCount  = instanceCount;
		m_ptrBuffer->BufferData(indexDraw * sizeof(DrawCommand) + offsetof(DrawCommand, instanceCount), sizeof(instanceCount), &instanceCount);
	}

	return true;
}

bool CGLES3IndirectBuffer::BufferData(int indexDraw, int baseVertex, int firstIndex, int indexCount, int instanceCount)
{
	if (indexDraw < 0 || (uint32_t)indexDraw >= m_draws.size()) {
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
		m_ptrBuffer->BufferData(indexDraw * sizeof(DrawCommand), sizeof(m_draws[indexDraw]), &m_draws[indexDraw]);
	}

	return true;
}

void CGLES3IndirectBuffer::Bind(void) const
{
	m_ptrBuffer->Bind();
}
