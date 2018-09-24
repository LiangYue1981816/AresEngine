#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxIndexBuffer.h"


CGfxIndexBuffer::CGfxIndexBuffer(GLenum type, size_t size, bool bDynamic)
	: m_indexType(type)
	, m_indexBuffer(0)
	, m_size(size)
{
	CGfxProfiler::IncIndexBufferSize(m_size);

	glGenBuffers(1, &m_indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_size, NULL, bDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

CGfxIndexBuffer::~CGfxIndexBuffer(void)
{
	CGfxProfiler::DecIndexBufferSize(m_size);

	glDeleteBuffers(1, &m_indexBuffer);
	m_indexType = GL_INVALID_ENUM;
	m_indexBuffer = 0;
	m_size = 0;
}

bool CGfxIndexBuffer::BufferData(size_t offset, size_t size, const void *pBuffer)
{
	if (m_size < (GLsizeiptr)(offset + size)) {
		return false;
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, (GLintptr)offset, (GLsizeiptr)size, pBuffer);
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

GLsizeiptr CGfxIndexBuffer::GetSize(void) const
{
	return m_size;
}

void CGfxIndexBuffer::Bind(void) const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
}
