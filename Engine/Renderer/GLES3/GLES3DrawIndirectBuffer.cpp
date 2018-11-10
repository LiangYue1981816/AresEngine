#include "GLES3Renderer.h"
#include "GLES3DrawIndirectBuffer.h"


CGLES3DrawIndirectBuffer::CGLES3DrawIndirectBuffer(size_t size)
	: CGfxDrawIndirectBuffer(size)
	, m_buffer(0)
{
	glGenBuffers(1, &m_buffer);
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_buffer);
	glBufferData(GL_DRAW_INDIRECT_BUFFER, m_size, nullptr, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, 0);
}

CGLES3DrawIndirectBuffer::~CGLES3DrawIndirectBuffer(void)
{
	glDeleteBuffers(1, &m_buffer);
}

bool CGLES3DrawIndirectBuffer::BufferData(int indexDraw, int baseVertex, int firstIndex, int indexCount, int instanceCount)
{
	if (indexDraw < 0) {
		return false;
	}

	if (m_size < (indexDraw + 1) * sizeof(DrawCommand)) {
		return false;
	}

	DrawCommand drawCommand;
	drawCommand.baseVertex = baseVertex;
	drawCommand.firstIndex = firstIndex;
	drawCommand.indexCount = indexCount;
	drawCommand.instanceCount = instanceCount;
	drawCommand.reservedMustBeZero = 0;

	GLBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_buffer);
	glBufferSubData(GL_DRAW_INDIRECT_BUFFER, indexDraw * sizeof(DrawCommand), sizeof(DrawCommand), &drawCommand);

	return true;
}

void CGLES3DrawIndirectBuffer::Bind(void *pParam)
{
	GLBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_buffer);
}
