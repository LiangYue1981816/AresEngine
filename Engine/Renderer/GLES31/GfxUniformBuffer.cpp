#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxUniformBuffer.h"


CGfxUniformBuffer::CGfxUniformBuffer(size_t size, bool bDynamic)
	: m_buffer(0)
	, m_size(size)
{
	CGfxProfiler::IncUniformBufferSize(m_size);

	glGenBuffers(1, &m_buffer);
	glBindBuffer(GL_UNIFORM_BUFFER, m_buffer);
	glBufferData(GL_UNIFORM_BUFFER, m_size, NULL, bDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

CGfxUniformBuffer::~CGfxUniformBuffer(void)
{
	CGfxProfiler::DecUniformBufferSize(m_size);

	glDeleteBuffers(1, &m_buffer);
	m_buffer = 0;
	m_size = 0;
}

bool CGfxUniformBuffer::BufferData(size_t offset, size_t size, const void *pBuffer)
{
	if (m_size < (GLsizeiptr)(offset + size)) {
		return false;
	}

	glBindBuffer(GL_UNIFORM_BUFFER, m_buffer);
	glBufferSubData(GL_UNIFORM_BUFFER, (GLintptr)offset, (GLsizeiptr)size, pBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	return true;
}

GLuint CGfxUniformBuffer::GetBuffer(void) const
{
	return m_buffer;
}

GLsizeiptr CGfxUniformBuffer::GetSize(void) const
{
	return m_size;
}
