#include "GLES3Renderer.h"


CGLES3IndexBuffer::CGLES3IndexBuffer(GfxIndexType type, size_t size, bool bDynamic)
	: CGfxIndexBuffer(type, size, bDynamic)

	, m_buffer(0)
{
	glGenBuffers(1, &m_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_size, nullptr, bDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

CGLES3IndexBuffer::~CGLES3IndexBuffer(void)
{
	glDeleteBuffers(1, &m_buffer);
}

bool CGLES3IndexBuffer::BufferData(size_t offset, size_t size, const void *pBuffer)
{
	if (m_size < (uint32_t)(offset + size)) {
		return false;
	}

	GLBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, (int)offset, (uint32_t)size, pBuffer);

	return true;
}

void CGLES3IndexBuffer::Bind(void)
{
	GLBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer);
}
