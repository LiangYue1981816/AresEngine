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

void CGLES3IndexBuffer::Release(void)
{
	delete this;
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
	return m_pBuffer->BufferData(offset, size, data, false);
}

void CGLES3IndexBuffer::Bind(void) const
{
	m_pBuffer->Bind();
}


CGLES3MultiIndexBuffer::CGLES3MultiIndexBuffer(GfxIndexType type, size_t size, bool bDynamic, int count)
	: CGfxMultiIndexBuffer(type, size, bDynamic, count)
	, m_index(0)
	, m_pBuffers(std::max(1, count))
{
	for (int index = 0; index < m_pBuffers.size(); index++) {
		m_pBuffers[index] = new CGLES3IndexBuffer(type, size, bDynamic);
	}
}

CGLES3MultiIndexBuffer::~CGLES3MultiIndexBuffer(void)
{
	for (auto& itBuffer : m_pBuffers) {
		delete itBuffer;
	}
}

void CGLES3MultiIndexBuffer::Release(void)
{
	delete this;
}

void CGLES3MultiIndexBuffer::SetBufferIndex(int index)
{
	m_index = index;
	m_index = std::min(m_index, (int)m_pBuffers.size() - 1);
	m_index = std::max(m_index, 0);
}

GfxIndexType CGLES3MultiIndexBuffer::GetIndexType(void) const
{
	return m_pBuffers[m_index]->GetIndexType();
}

uint32_t CGLES3MultiIndexBuffer::GetIndexCount(void) const
{
	return m_pBuffers[m_index]->GetIndexCount();
}

uint32_t CGLES3MultiIndexBuffer::GetSize(void) const
{
	return m_pBuffers[m_index]->GetSize();
}

bool CGLES3MultiIndexBuffer::BufferData(size_t offset, size_t size, const void* data)
{
	return m_pBuffers[m_index]->BufferData(offset, size, data);
}

void CGLES3MultiIndexBuffer::Bind(void) const
{
	m_pBuffers[m_index]->Bind();
}
