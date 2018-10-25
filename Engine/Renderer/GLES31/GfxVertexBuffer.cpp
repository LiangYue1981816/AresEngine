#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxVertexBuffer.h"
#include "GfxVertexAttribute.h"


CGfxVertexBuffer::CGfxVertexBuffer(uint32_t format, size_t size, bool bDynamic)
	: m_vertexFormat(format)
	, m_vertexBuffer(0)
	, m_size(size)
{
	CGfxProfiler::IncVertexBufferSize(m_size);

	glGenBuffers(1, &m_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_size, nullptr, bDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

CGfxVertexBuffer::~CGfxVertexBuffer(void)
{
	CGfxProfiler::DecVertexBufferSize(m_size);

	glDeleteBuffers(1, &m_vertexBuffer);
	m_vertexFormat = 0;
	m_vertexBuffer = 0;
	m_size = 0;
}

bool CGfxVertexBuffer::BufferData(size_t offset, size_t size, const void *pBuffer)
{
	if (m_size < (uint32_t)(offset + size)) {
		return false;
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, (int)offset, (uint32_t)size, pBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return true;
}

uint32_t CGfxVertexBuffer::GetVertexCount(void) const
{
	return m_size / GetVertexStride(m_vertexFormat);
}

uint32_t CGfxVertexBuffer::GetVertexFormat(void) const
{
	return m_vertexFormat;
}

uint32_t CGfxVertexBuffer::GetVertexBuffer(void) const
{
	return m_vertexBuffer;
}

uint32_t CGfxVertexBuffer::GetSize(void) const
{
	return m_size;
}

void CGfxVertexBuffer::Bind(void) const
{
	GLBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
}

void CGfxVertexBuffer::SetupFormat(void) const
{
	uint32_t vertexStride = GetVertexStride(m_vertexFormat);

	for (uint32_t indexAttribute = 0; indexAttribute < VERTEX_ATTRIBUTE_COUNT; indexAttribute++) {
		uint32_t attribute = (1 << indexAttribute);

		if (m_vertexFormat & attribute) {
			uint32_t location = GetVertexAttributeLocation(attribute);
			uint32_t components = GetVertexAttributeComponents(attribute);
			uint32_t offset = GetVertexAttributeOffset(m_vertexFormat, attribute);

			glEnableVertexAttribArray(location);
			glVertexAttribPointer(location, components, GL_FLOAT, GL_FALSE, vertexStride, (const void *)offset);
			glVertexAttribDivisor(location, 0);
		}
	}
}
