#include "GLES3Renderer.h"


CGLES3IndirectBuffer::CGLES3IndirectBuffer(uint32_t drawCommandCount)
	: CGfxIndirectBuffer(drawCommandCount)

	, m_size(drawCommandCount * sizeof(DrawCommand))
	, m_count(drawCommandCount)

	, m_buffer(0)
{
	m_draws.resize(m_count);

	glGenBuffers(1, &m_buffer);
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_buffer);
	glBufferData(GL_DRAW_INDIRECT_BUFFER, m_size, nullptr, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, 0);
	CGfxProfiler::IncIndirectBufferSize(m_size);
}

CGLES3IndirectBuffer::~CGLES3IndirectBuffer(void)
{
	glDeleteBuffers(1, &m_buffer);
	CGfxProfiler::DecIndirectBufferSize(m_size);
}

uint32_t CGLES3IndirectBuffer::GetDrawCommandCount(void) const
{
	return m_count;
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
	if (indexDraw < 0 || (uint32_t)indexDraw >= m_count) {
		return false;
	}

	if (m_draws[indexDraw].instanceCount != instanceCount) {
		m_draws[indexDraw].instanceCount  = instanceCount;

		GLBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_buffer);
		glBufferSubData(GL_DRAW_INDIRECT_BUFFER, indexDraw * sizeof(DrawCommand) + offsetof(DrawCommand, instanceCount), sizeof(instanceCount), &instanceCount);
	}

	return true;
}

bool CGLES3IndirectBuffer::BufferData(int indexDraw, int baseVertex, int firstIndex, int indexCount, int instanceCount)
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

		GLBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_buffer);
		glBufferSubData(GL_DRAW_INDIRECT_BUFFER, indexDraw * sizeof(DrawCommand), sizeof(m_draws[indexDraw]), &m_draws[indexDraw]);
	}

	return true;
}

void CGLES3IndirectBuffer::Bind(void)
{
	GLBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_buffer);
}
