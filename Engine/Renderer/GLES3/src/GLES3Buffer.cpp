#include "GLES3Renderer.h"


CGLES3Buffer::CGLES3Buffer(uint32_t target, size_t size, bool bDynamic)
	: m_size(size)
	, m_target(target)
{
	glGenBuffers(1, &m_buffer);
	glBindBuffer(m_target, m_buffer);
	glBufferData(m_target, m_size, nullptr, bDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
	glBindBuffer(m_target, 0);
}

CGLES3Buffer::~CGLES3Buffer(void)
{
	glDeleteBuffers(1, &m_buffer);
}

void CGLES3Buffer::Release(void)
{
	delete this;
}

uint32_t CGLES3Buffer::GetTarget(void) const
{
	return m_target;
}

uint32_t CGLES3Buffer::GetBuffer(void) const
{
	return m_buffer;
}

uint32_t CGLES3Buffer::GetSize(void) const
{
	return m_size;
}

void CGLES3Buffer::BufferData(size_t size, bool bDynamic)
{
	m_size = size;

	glBindBuffer(m_target, m_buffer);
	glBufferData(m_target, m_size, nullptr, bDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
	glBindBuffer(m_target, 0);
}

bool CGLES3Buffer::BufferData(size_t offset, size_t size, const void *pBuffer)
{
	if (m_size < (uint32_t)(offset + size)) {
		return false;
	}

	glBindBuffer(m_target, m_buffer);
	glBufferSubData(m_target, (int)offset, (uint32_t)size, pBuffer);
	glBindBuffer(m_target, 0);

	return true;
}

void CGLES3Buffer::Bind(void) const
{
	GLBindBuffer(m_target, m_buffer);
}

void CGLES3Buffer::Bind(int index, int offset, int size) const
{
	if (m_size < (uint32_t)(offset + size)) {
		return;
	}

	GLBindBufferRange(m_target, index, m_buffer, offset, size);
}
