#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxIndexBuffer.h"


CGfxIndexBuffer::CGfxIndexBuffer(GLenum type)
	: m_indexType(type)
	, m_indexBuffer(0)
	, m_size(0)
{
	glGenBuffers(1, &m_indexBuffer);
}

CGfxIndexBuffer::~CGfxIndexBuffer(void)
{
	glDeleteBuffers(1, &m_indexBuffer);
}

void CGfxIndexBuffer::Bind(void) const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
}

bool CGfxIndexBuffer::BufferData(size_t size, const void *pBuffer, bool bDynamic)
{
	m_size = size;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, pBuffer, bDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return true;
}

GLenum CGfxIndexBuffer::GetIndexType(void) const
{
	return m_indexType;
}

GLuint CGfxIndexBuffer::GetIndexCount(void) const
{
	return m_size / (m_indexType == GL_UNSIGNED_SHORT ? 2 : 4);
}

GLuint CGfxIndexBuffer::GetIndexBuffer(void) const
{
	return m_indexBuffer;
}

GLuint CGfxIndexBuffer::GetSize(void) const
{
	return m_size;
}
