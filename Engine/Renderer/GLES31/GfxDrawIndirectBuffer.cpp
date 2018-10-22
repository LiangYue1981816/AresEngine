#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxDrawIndirectBuffer.h"


typedef struct {
	uint32_t indexCount;
	uint32_t instanceCount;
	uint32_t firstIndex;
	int      baseVertex;
	uint32_t reservedMustBeZero;
} DrawCommand;

CGfxDrawIndirectBuffer::CGfxDrawIndirectBuffer(uint32_t name, int baseVertex, uint32_t firstIndex, uint32_t indexCount)
	: m_name(name)
	, m_buffer(0)
	, m_baseVertex(baseVertex)
	, m_firstIndex(firstIndex)
	, m_indexCount(indexCount)
{
	glGenBuffers(1, &m_buffer);
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_buffer);
	glBufferData(GL_DRAW_INDIRECT_BUFFER, sizeof(DrawCommand), NULL, GL_DYNAMIC_DRAW);
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

uint32_t CGfxDrawIndirectBuffer::GetName(void) const
{
	return m_name;
}

void CGfxDrawIndirectBuffer::Bind(uint32_t instanceCount) const
{
	DrawCommand drawCommand;
	drawCommand.baseVertex = m_baseVertex;
	drawCommand.firstIndex = m_firstIndex;
	drawCommand.indexCount = m_indexCount;
	drawCommand.instanceCount = instanceCount;
	drawCommand.reservedMustBeZero = 0;

	GLBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_buffer);
	glBufferSubData(GL_DRAW_INDIRECT_BUFFER, 0, sizeof(drawCommand), &drawCommand);
}
