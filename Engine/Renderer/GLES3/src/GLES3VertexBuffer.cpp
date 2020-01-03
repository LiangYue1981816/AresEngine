#include "GLES3Renderer.h"


CGLES3VertexBuffer::CGLES3VertexBuffer(uint32_t vertexFormat, int vertexBinding, size_t size, bool bDynamic)
	: CGfxVertexBuffer(vertexFormat, vertexBinding, size, bDynamic)
	, m_pBuffer(nullptr)

	, m_binding(vertexBinding)
	, m_format(vertexFormat)
	, m_count(size / GetVertexStride(vertexFormat))
{
	m_pBuffer = new CGLES3Buffer(GL_ARRAY_BUFFER, size, bDynamic);
	CGfxProfiler::IncVertexBufferSize(m_pBuffer->GetSize());
}

CGLES3VertexBuffer::~CGLES3VertexBuffer(void)
{
	CGfxProfiler::DecVertexBufferSize(m_pBuffer->GetSize());
	delete m_pBuffer;
}

void CGLES3VertexBuffer::Release(void)
{
	delete this;
}

uint32_t CGLES3VertexBuffer::GetVertexBinding(void) const
{
	return m_binding;
}

uint32_t CGLES3VertexBuffer::GetVertexFormat(void) const
{
	return m_format;
}

uint32_t CGLES3VertexBuffer::GetVertexCount(void) const
{
	return m_count;
}

uint32_t CGLES3VertexBuffer::GetSize(void) const
{
	return m_pBuffer->GetSize();
}

bool CGLES3VertexBuffer::BufferData(size_t offset, size_t size, const void* data)
{
	return m_pBuffer->BufferData(offset, size, data);
}

void CGLES3VertexBuffer::Bind(void) const
{
	m_pBuffer->Bind();

	for (uint32_t indexAttribute = 0; indexAttribute < GetVertexAttributeCount(); indexAttribute++) {
		uint32_t attribute = (1 << indexAttribute);

		if (m_format & attribute) {
			uint32_t stride = GetVertexStride(m_format);
			uint32_t location = GetVertexAttributeLocation(attribute);
			uint32_t components = GetVertexAttributeComponents(attribute);
			uintptr_t offset = GetVertexAttributeOffset(m_format, attribute);

			glEnableVertexAttribArray(location);
			glVertexAttribPointer(location, components, GL_FLOAT, GL_FALSE, stride, (const void*)offset);
			glVertexAttribDivisor(location, 0);
		}
	}

	CHECK_GL_ERROR_ASSERT();
}


CGLES3MultiVertexBuffer::CGLES3MultiVertexBuffer(uint32_t vertexFormat, int vertexBinding, size_t size, bool bDynamic, int count)
	: CGfxMultiVertexBuffer(vertexFormat, vertexBinding, size, bDynamic, count)
	, m_index(0)
	, m_pBuffers(std::max(1, count))
{
	for (int index = 0; index < m_pBuffers.size(); index++) {
		m_pBuffers[index] = new CGLES3VertexBuffer(vertexFormat, vertexBinding, size, bDynamic);
	}
}

CGLES3MultiVertexBuffer::~CGLES3MultiVertexBuffer(void)
{
	for (auto& itBuffer : m_pBuffers) {
		delete itBuffer;
	}
}

void CGLES3MultiVertexBuffer::Release(void)
{
	delete this;
}

void CGLES3MultiVertexBuffer::SetBufferIndex(int index)
{
	m_index = index;
	m_index = std::min(m_index, (int)m_pBuffers.size() - 1);
	m_index = std::max(m_index, 0);
}

uint32_t CGLES3MultiVertexBuffer::GetVertexBinding(void) const
{
	return m_pBuffers[m_index]->GetVertexBinding();
}

uint32_t CGLES3MultiVertexBuffer::GetVertexFormat(void) const
{
	return m_pBuffers[m_index]->GetVertexFormat();
}

uint32_t CGLES3MultiVertexBuffer::GetVertexCount(void) const
{
	return m_pBuffers[m_index]->GetVertexCount();
}

uint32_t CGLES3MultiVertexBuffer::GetSize(void) const
{
	return m_pBuffers[m_index]->GetSize();
}

bool CGLES3MultiVertexBuffer::BufferData(size_t offset, size_t size, const void* data)
{
	return m_pBuffers[m_index]->BufferData(offset, size, data);
}

void CGLES3MultiVertexBuffer::Bind(void) const
{
	m_pBuffers[m_index]->Bind();
}
