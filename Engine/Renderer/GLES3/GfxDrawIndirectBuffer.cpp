#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxDrawIndirectBuffer.h"


typedef struct {
	int indexCount;
	int instanceCount;
	int firstIndex;
	int baseVertex;
	int reservedMustBeZero;
} DrawCommand;

CGfxDrawIndirectBuffer::CGfxDrawIndirectBuffer(uint32_t name, int baseVertex, int firstIndex, int indexCount)
	: m_name(name)
	, m_buffer(0)
	, m_baseVertex(baseVertex)
	, m_firstIndex(firstIndex)
	, m_indexCount(indexCount)
	, m_instanceCount(0)
{
	glGenBuffers(1, &m_buffer);
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_buffer);
	glBufferData(GL_DRAW_INDIRECT_BUFFER, sizeof(DrawCommand), nullptr, GL_DYNAMIC_DRAW);
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

int CGfxDrawIndirectBuffer::GetBaseVertex(void) const
{
	return m_baseVertex;
}

int CGfxDrawIndirectBuffer::GetFirstIndex(void) const
{
	return m_firstIndex;
}

int CGfxDrawIndirectBuffer::GetIndexCount(void) const
{
	return m_indexCount;
}

int CGfxDrawIndirectBuffer::GetInstanceCount(void) const
{
	return m_instanceCount;
}

void CGfxDrawIndirectBuffer::Bind(int instanceCount)
{
	GLBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_buffer);

	if (m_instanceCount != instanceCount) {
		m_instanceCount  = instanceCount;

		DrawCommand drawCommand;
		drawCommand.baseVertex = m_baseVertex;
		drawCommand.firstIndex = m_firstIndex;
		drawCommand.indexCount = m_indexCount;
		drawCommand.instanceCount = m_instanceCount;
		drawCommand.reservedMustBeZero = 0;

		glBufferSubData(GL_DRAW_INDIRECT_BUFFER, 0, sizeof(drawCommand), &drawCommand);
	}
}
