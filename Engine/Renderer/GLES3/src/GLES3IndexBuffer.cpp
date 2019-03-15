#include "GLES3Renderer.h"


CGLES3IndexBuffer::CGLES3IndexBuffer(GfxIndexType type, size_t size, bool bDynamic)
	: CGfxIndexBuffer(type, size, bDynamic)
	, m_type(type)
{
	m_ptrBuffer = CGLES3BufferPtr(new CGLES3Buffer(GL_ELEMENT_ARRAY_BUFFER, size, bDynamic));
	CGfxProfiler::IncIndexBufferSize(m_ptrBuffer->GetSize());
}

CGLES3IndexBuffer::~CGLES3IndexBuffer(void)
{
	CGfxProfiler::DecIndexBufferSize(m_ptrBuffer->GetSize());
}

GfxIndexType CGLES3IndexBuffer::GetIndexType(void) const
{
	return m_type;
}

uint32_t CGLES3IndexBuffer::GetIndexCount(void) const
{
	switch (m_type) {
	case GFX_INDEX_UNSIGNED_SHORT: return m_ptrBuffer->GetSize() / 2;
	case GFX_INDEX_UNSIGNED_INT:   return m_ptrBuffer->GetSize() / 4;
	default:                       return 0;
	}
}

uint32_t CGLES3IndexBuffer::GetSize(void) const
{
	return m_ptrBuffer->GetSize();
}

bool CGLES3IndexBuffer::BufferData(size_t offset, size_t size, const void *pBuffer)
{
	return m_ptrBuffer->BufferData(offset, size, pBuffer);
}

void CGLES3IndexBuffer::Bind(void) const
{
	m_ptrBuffer->Bind();
}
