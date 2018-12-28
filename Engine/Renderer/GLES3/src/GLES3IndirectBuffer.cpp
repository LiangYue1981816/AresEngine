#include "GLES3Renderer.h"


CGLES3IndirectBuffer::CGLES3IndirectBuffer(uint32_t drawCommandCount)
	: CGfxIndirectBuffer(drawCommandCount)

	, m_buffer(0)
{
#if GLES_VER == 310
	m_draws.resize(m_count);

	glGenBuffers(1, &m_buffer);
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_buffer);
	glBufferData(GL_DRAW_INDIRECT_BUFFER, m_size, nullptr, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, 0);
#endif
}

CGLES3IndirectBuffer::~CGLES3IndirectBuffer(void)
{
#if GLES_VER == 310
	glDeleteBuffers(1, &m_buffer);
#endif
}

bool CGLES3IndirectBuffer::BufferData(int indexDraw, int instanceCount)
{
#if GLES_VER == 310
	if (indexDraw < 0 || (uint32_t)indexDraw >= m_count) {
		return false;
	}

	if (m_draws[indexDraw].instanceCount != instanceCount) {
		m_draws[indexDraw].instanceCount  = instanceCount;

		GLBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_buffer);
		glBufferSubData(GL_DRAW_INDIRECT_BUFFER, indexDraw * sizeof(DrawCommand) + offsetof(DrawCommand, instanceCount), sizeof(instanceCount), &instanceCount);
	}

	return true;
#else
	return true;
#endif
}

bool CGLES3IndirectBuffer::BufferData(int indexDraw, int baseVertex, int firstIndex, int indexCount, int instanceCount)
{
#if GLES_VER == 310
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
#else
	return true;
#endif
}

void CGLES3IndirectBuffer::Bind(void)
{
#if GLES_VER == 310
	GLBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_buffer);
#endif
}
