#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxUniformBuffer.h"


CGfxUniformBuffer::CGfxUniformBuffer(void)
	: m_buffer(0)
	, m_size(0)
{

}

CGfxUniformBuffer::~CGfxUniformBuffer(void)
{
	Destroy();
}

bool CGfxUniformBuffer::Create(const void *pBuffer, size_t size, bool bDynamic)
{
	Destroy();

	m_size = (GLsizeiptr)size;
	glGenBuffers(1, &m_buffer);
	glBindBuffer(GL_UNIFORM_BUFFER, m_buffer);
	glBufferData(GL_UNIFORM_BUFFER, m_size, NULL, bDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	return SetData(pBuffer, size);
}

void CGfxUniformBuffer::Destroy(void)
{
	if (m_buffer) {
		glDeleteBuffers(1, &m_buffer);
	}

	m_size = 0;
	m_buffer = 0;
}

bool CGfxUniformBuffer::SetData(const void *pBuffer, size_t size, size_t offset)
{
	if (m_size < (GLsizei)size) {
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
