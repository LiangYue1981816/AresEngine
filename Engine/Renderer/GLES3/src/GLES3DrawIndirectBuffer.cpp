#include "GfxHeader.h"


CGLES3DrawIndirectBuffer::CGLES3DrawIndirectBuffer(uint32_t count)
	: CGfxDrawIndirectBuffer(count)
	, m_buffer(0)
{
#if GLES_VER == 310
	glGenBuffers(1, &m_buffer);
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_buffer);
	glBufferData(GL_DRAW_INDIRECT_BUFFER, m_size, nullptr, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, 0);
#endif
}

CGLES3DrawIndirectBuffer::~CGLES3DrawIndirectBuffer(void)
{
#if GLES_VER == 310
	glDeleteBuffers(1, &m_buffer);
#endif
}

bool CGLES3DrawIndirectBuffer::BufferData(size_t offset, size_t size, const void *pBuffer)
{
#if GLES_VER == 310
	if (m_size < (uint32_t)(offset + size)) {
		return false;
	}

	GLBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_buffer);
	glBufferSubData(GL_DRAW_INDIRECT_BUFFER, (int)offset, (uint32_t)size, pBuffer);

	return true;
#else
	return true;
#endif
}

bool CGLES3DrawIndirectBuffer::BufferData(int indexDraw, int instanceCount)
{
#if GLES_VER == 310
	if (indexDraw < 0 || (uint32_t)indexDraw >= m_count) {
		return false;
	}

	GLBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_buffer);
	glBufferSubData(GL_DRAW_INDIRECT_BUFFER, indexDraw * sizeof(DrawCommand) + offsetof(DrawCommand, instanceCount), sizeof(instanceCount), &instanceCount);

	return true;
#else
	return true;
#endif
}

bool CGLES3DrawIndirectBuffer::BufferData(int indexDraw, int baseVertex, int firstIndex, int indexCount, int instanceCount)
{
#if GLES_VER == 310
	if (indexDraw < 0 || (uint32_t)indexDraw >= m_count) {
		return false;
	}

	DrawCommand drawCommand;
	drawCommand.baseVertex = baseVertex;
	drawCommand.firstIndex = firstIndex;
	drawCommand.indexCount = indexCount;
	drawCommand.instanceCount = instanceCount;
	drawCommand.reservedMustBeZero = 0;

	GLBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_buffer);
	glBufferSubData(GL_DRAW_INDIRECT_BUFFER, indexDraw * sizeof(DrawCommand), sizeof(drawCommand), &drawCommand);

	return true;
#else
	return true;
#endif
}

void CGLES3DrawIndirectBuffer::Bind(void *pParam)
{
#if GLES_VER == 310
	GLBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_buffer);
#endif
}
