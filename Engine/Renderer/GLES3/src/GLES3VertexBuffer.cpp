#include "GLES3Renderer.h"


CGLES3VertexBuffer::CGLES3VertexBuffer(uint32_t vertexFormat, uint32_t vertexBinding, size_t size, bool bDynamic)
	: CGfxVertexBuffer(vertexFormat, vertexBinding, size, bDynamic)
	, m_format(vertexFormat)
	, m_count(size / GetVertexStride(vertexFormat))
{
	m_ptrBuffer = CGLES3BufferPtr(new CGLES3Buffer(GL_ARRAY_BUFFER, size, bDynamic));
	CGfxProfiler::IncVertexBufferSize(m_ptrBuffer->GetSize());
}

CGLES3VertexBuffer::~CGLES3VertexBuffer(void)
{
	CGfxProfiler::DecVertexBufferSize(m_ptrBuffer->GetSize());
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
	return m_ptrBuffer->GetSize();
}

bool CGLES3VertexBuffer::BufferData(size_t offset, size_t size, const void* pBuffer)
{
	return m_ptrBuffer->BufferData(offset, size, pBuffer);
}

void CGLES3VertexBuffer::Bind(void) const
{
	m_ptrBuffer->Bind();

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
}
