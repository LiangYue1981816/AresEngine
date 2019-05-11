#include "GLES3Renderer.h"


CGLES3IndexBuffer::CGLES3IndexBuffer(GfxIndexType type, size_t size, bool bDynamic)
	: CGfxIndexBuffer(type, size, bDynamic)
	, m_pBuffer(nullptr)
	, m_type(type)
{
	m_pBuffer = new CGLES3Buffer(GL_ELEMENT_ARRAY_BUFFER, size, bDynamic);
	CGfxProfiler::IncIndexBufferSize(m_pBuffer->GetSize());
}

CGLES3IndexBuffer::~CGLES3IndexBuffer(void)
{
	CGfxProfiler::DecIndexBufferSize(m_pBuffer->GetSize());
	delete m_pBuffer;
}

GfxIndexType CGLES3IndexBuffer::GetIndexType(void) const
{
	return m_type;
}

uint32_t CGLES3IndexBuffer::GetIndexCount(void) const
{
	switch (m_type) {
	case GFX_INDEX_UNSIGNED_SHORT: return m_pBuffer->GetSize() / 2;
	case GFX_INDEX_UNSIGNED_INT:   return m_pBuffer->GetSize() / 4;
	default:                       return 0;
	}
}

uint32_t CGLES3IndexBuffer::GetSize(void) const
{
	return m_pBuffer->GetSize();
}

bool CGLES3IndexBuffer::BufferData(size_t offset, size_t size, const void* data)
{
	return m_pBuffer->BufferData(offset, size, data);
}

void CGLES3IndexBuffer::Bind(void) const
{
	m_pBuffer->Bind();
}
