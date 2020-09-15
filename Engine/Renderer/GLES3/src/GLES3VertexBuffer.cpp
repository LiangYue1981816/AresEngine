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
	return m_pBuffer->BufferData(offset, size, data, false);
}

void CGLES3VertexBuffer::Bind(void) const
{
	m_pBuffer->Bind();

	for (uint32_t indexAttribute = 0; indexAttribute < GetVertexAttributeCount(); indexAttribute++) {
		uint32_t attribute = (1 << indexAttribute);

		if (m_format & attribute) {
			uint32_t attributeStride = GetVertexStride(m_format);
			uint32_t attributeType = GetVertexAttributeType(attribute);
			uint32_t attributeLocation = GetVertexAttributeLocation(attribute);
			uint32_t attributeComponents = GetVertexAttributeComponents(attribute);
			uint32_t attributeNormalized = GetVertexAttributeNormalized(attribute);
			uint32_t attributeOffset = GetVertexAttributeOffset(m_format, attribute);

			glEnableVertexAttribArray(attributeLocation);
			glVertexAttribPointer(attributeLocation, attributeComponents, CGLES3Helper::TranslateDataType((GfxDataType)attributeType), attributeNormalized, attributeStride, (const void*)attributeOffset);
			glVertexAttribDivisor(attributeLocation, 0);
		}
	}

	CHECK_GL_ERROR_ASSERT();
}
