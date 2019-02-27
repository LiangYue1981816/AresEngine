#include "GLES3Renderer.h"


CGLES3Buffer::CGLES3Buffer(uint32_t target, size_t size, bool bDynamic)
	: m_size(size)
	, m_target(target)
	, m_buffer(0)
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
