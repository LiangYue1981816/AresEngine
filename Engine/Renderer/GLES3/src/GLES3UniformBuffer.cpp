#include "GfxHeader.h"


CGLES3UniformBuffer::CGLES3UniformBuffer(size_t size, bool bDynamic)
	: CGfxUniformBuffer(size, bDynamic)
	, m_buffer(0)
{
	glGenBuffers(1, &m_buffer);
	glBindBuffer(GL_UNIFORM_BUFFER, m_buffer);
	glBufferData(GL_UNIFORM_BUFFER, m_size, nullptr, bDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

CGLES3UniformBuffer::~CGLES3UniformBuffer(void)
{
	glDeleteBuffers(1, &m_buffer);
}

bool CGLES3UniformBuffer::BufferData(size_t offset, size_t size, const void *pBuffer)
{
	if (m_size < (uint32_t)(offset + size)) {
		return false;
	}

	glBindBuffer(GL_UNIFORM_BUFFER, m_buffer);
	glBufferSubData(GL_UNIFORM_BUFFER, (int)offset, (uint32_t)size, pBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	return true;
}

bool CGLES3UniformBuffer::Bind(int index, int offset, int size)
{
	if (m_size < (uint32_t)(offset + size)) {
		return false;
	}

	GLBindBufferRange(GL_UNIFORM_BUFFER, index, m_buffer, offset, size);

	return true;
}
