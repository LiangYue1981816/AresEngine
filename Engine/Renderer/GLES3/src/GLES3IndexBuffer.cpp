#include "GLES3Renderer.h"


CGLES3IndexBuffer::CGLES3IndexBuffer(GfxIndexType type, size_t size, bool bDynamic)
	: CGLES3Buffer(GL_ELEMENT_ARRAY_BUFFER, size, bDynamic)
	, CGfxIndexBuffer(type, size, bDynamic)

	, m_type(type)
{
	CGfxProfiler::IncIndexBufferSize(m_size);
}

CGLES3IndexBuffer::~CGLES3IndexBuffer(void)
{
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
	return CGLES3Buffer::GetSize();
}

bool CGLES3IndexBuffer::BufferData(size_t offset, size_t size, const void *pBuffer)
{
	return CGLES3Buffer::BufferData(offset, size, pBuffer);
}
