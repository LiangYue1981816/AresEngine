#include "GLES3Renderer.h"


CGLES3Buffer::CGLES3Buffer(uint32_t target, size_t size, bool bDynamic)
	: m_target(target)
	, m_size(size)
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
	ASSERT(m_buffer);
	return m_buffer;
}

uint32_t CGLES3Buffer::GetSize(void) const
{
	ASSERT(m_size);
	return m_size;
}

bool CGLES3Buffer::BufferSize(size_t size, bool bDynamic)
{
	ASSERT(size);

	m_size = size;
	glBindBuffer(m_target, m_buffer);
	glBufferData(m_target, m_size, nullptr, bDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
	glBindBuffer(m_target, 0);

	return true;
}

bool CGLES3Buffer::BufferData(size_t offset, size_t size, const void* data, bool bSync)
{
	ASSERT(data);
	ASSERT(size);
	ASSERT(m_size >= (uint32_t)(offset + size));

	void* addr = nullptr;

	glBindBuffer(m_target, m_buffer);
	glMapBufferRangeAddress(m_target, offset, size, bSync ? GL_MAP_WRITE_BIT : GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT, &addr);
	glMemcpy(addr, data, size);
	glUnmapBuffer(m_target);
	glBindBuffer(m_target, 0);

	return true;
}

void CGLES3Buffer::Bind(void) const
{
	GLBindBuffer(m_target, m_buffer);
}

void CGLES3Buffer::Bind(int binding, int offset, int size) const
{
	ASSERT(size);
	ASSERT(m_size >= (uint32_t)(offset + size));

	GLBindBufferRange(m_target, binding, m_buffer, offset, size);
}
