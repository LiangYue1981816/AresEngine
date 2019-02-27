#include "GLES3Renderer.h"


CGLES3IndirectBuffer::CGLES3IndirectBuffer(uint32_t drawCommandCount)
	: CGLES3Buffer(GL_DRAW_INDIRECT_BUFFER, drawCommandCount * sizeof(DrawCommand), true)
	, CGfxIndirectBuffer(drawCommandCount)
{
	m_draws.resize(drawCommandCount);
	CGfxProfiler::IncIndirectBufferSize(m_size);
}

CGLES3IndirectBuffer::~CGLES3IndirectBuffer(void)
{
	CGfxProfiler::DecIndirectBufferSize(m_size);
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
	return m_size;
}

bool CGLES3IndirectBuffer::BufferData(int indexDraw, int instanceCount)
{
	if (indexDraw < 0 || (uint32_t)indexDraw >= m_draws.size()) {
		return false;
	}

	if (m_draws[indexDraw].instanceCount != instanceCount) {
		m_draws[indexDraw].instanceCount  = instanceCount;

		GLBindBuffer(m_target, m_buffer);
		glBufferSubData(m_target, indexDraw * sizeof(DrawCommand) + offsetof(DrawCommand, instanceCount), sizeof(instanceCount), &instanceCount);
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

		GLBindBuffer(m_target, m_buffer);
		glBufferSubData(m_target, indexDraw * sizeof(DrawCommand), sizeof(m_draws[indexDraw]), &m_draws[indexDraw]);
	}

	return true;
}

void CGLES3IndirectBuffer::Bind(void)
{
	GLBindBuffer(m_target, m_buffer);
}
