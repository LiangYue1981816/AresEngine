#include "GfxHeader.h"


CGLES3VertexBuffer::CGLES3VertexBuffer(uint32_t binding, uint32_t format, size_t size, bool bDynamic)
	: CGfxVertexBuffer(binding, format, size, bDynamic)

	, m_buffer(0)
{
	glGenBuffers(1, &m_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
	glBufferData(GL_ARRAY_BUFFER, m_size, nullptr, bDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

CGLES3VertexBuffer::~CGLES3VertexBuffer(void)
{
	glDeleteBuffers(1, &m_buffer);
}

bool CGLES3VertexBuffer::BufferData(size_t offset, size_t size, const void *pBuffer)
{
	if (m_size < (uint32_t)(offset + size)) {
		return false;
	}

	GLBindBuffer(GL_ARRAY_BUFFER, m_buffer);
	glBufferSubData(GL_ARRAY_BUFFER, (int)offset, (uint32_t)size, pBuffer);

	return true;
}

void CGLES3VertexBuffer::Bind(void *pParam)
{
	GLBindBuffer(GL_ARRAY_BUFFER, m_buffer);

	for (uint32_t indexAttribute = 0; indexAttribute < GetVertexAttributeCount(); indexAttribute++) {
		uint32_t attribute = (1 << indexAttribute);

		if (m_format & attribute) {
			uint32_t stride = GetVertexStride(m_format);
			uint32_t location = GetVertexAttributeLocation(attribute);
			uint32_t components = GetVertexAttributeComponents(attribute);
			uintptr_t offset = GetVertexAttributeOffset(m_format, attribute);

			glEnableVertexAttribArray(location);
			glVertexAttribPointer(location, components, GL_FLOAT, GL_FALSE, stride, (const void *)offset);
			glVertexAttribDivisor(location, 0);
		}
	}
}
