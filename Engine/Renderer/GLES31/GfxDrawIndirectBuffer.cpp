#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxDrawIndirectBuffer.h"


CGfxDrawIndirectBuffer::CGfxDrawIndirectBuffer(int baseVertex, uint32_t firstIndex, uint32_t indexCount, uint32_t instanceCount)
	: m_buffer(0)
{
	typedef struct {
		uint32_t indexCount;
		uint32_t instanceCount;
		uint32_t firstIndex;
		int      baseVertex;
		uint32_t reservedMustBeZero;
	} DrawElementsIndirectCommand;

	DrawElementsIndirectCommand command;
	command.baseVertex = baseVertex;
	command.firstIndex = firstIndex;
	command.indexCount = indexCount;
	command.instanceCount = instanceCount;
	command.reservedMustBeZero = 0;

	glGenBuffers(1, &m_buffer);
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_buffer);
	glBufferData(GL_DRAW_INDIRECT_BUFFER, sizeof(command), &command, GL_STATIC_DRAW);
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, 0);
}

CGfxDrawIndirectBuffer::~CGfxDrawIndirectBuffer(void)
{
	glDeleteBuffers(1, &m_buffer);
}

void CGfxDrawIndirectBuffer::Release(void)
{
	Renderer()->DestroyDrawIndirectBuffer(this);
}

void CGfxDrawIndirectBuffer::Bind(void) const
{
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_buffer);
}
