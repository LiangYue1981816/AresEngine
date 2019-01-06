#include "GLES3Renderer.h"


CGLES3IndexBuffer::CGLES3IndexBuffer(GfxIndexType type, size_t size, bool bDynamic)
	: CGfxIndexBuffer(type, size, bDynamic)

	, m_type(type)
	, m_size(size)

	, m_buffer(0)
{
	glGenBuffers(1, &m_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_size, nullptr, bDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	CGfxProfiler::IncIndexBufferSize(m_size);
}

CGLES3IndexBuffer::~CGLES3IndexBuffer(void)
{
	glDeleteBuffers(1, &m_buffer);
	CGfxProfiler::DecIndexBufferSize(m_size);
}

GfxIndexType CGLES3IndexBuffer::GetIndexType(void) const
{
	return m_type;
}

uint32_t CGLES3IndexBuffer::GetIndexCount(void) const
{
	switch (m_type) {
	case GFX_INDEX_UNSIGNED_SHORT: return m_size / 2;
	case GFX_INDEX_UNSIGNED_INT:   return m_size / 4;
	default:                       return 0;
	}
}

uint32_t CGLES3IndexBuffer::GetSize(void) const
{
	return m_size;
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
