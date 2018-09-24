#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxVertexBuffer.h"
#include "GfxVertexAttribute.h"


CGfxVertexBuffer::CGfxVertexBuffer(GLuint format, size_t size, bool bDynamic)
	: m_vertexFormat(format)
	, m_vertexBuffer(0)
	, m_size(size)
{
	CGfxProfiler::IncVertexBufferSize(m_size);

	glGenBuffers(1, &m_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_size, NULL, bDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
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
	if (m_size < (GLsizeiptr)(offset + size)) {
		return false;
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, (GLintptr)offset, (GLsizeiptr)size, pBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return true;
}

GLuint CGfxVertexBuffer::GetVertexCount(void) const
{
	return m_size / GetVertexStride(m_vertexFormat);
}

GLuint CGfxVertexBuffer::GetVertexFormat(void) const
{
	return m_vertexFormat;
}

GLuint CGfxVertexBuffer::GetVertexBuffer(void) const
{
	return m_vertexBuffer;
}

GLsizeiptr CGfxVertexBuffer::GetSize(void) const
{
	return m_size;
}

void CGfxVertexBuffer::Bind(void) const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	{
		GLuint vertexStride = GetVertexStride(m_vertexFormat);

		for (GLuint indexAttribute = 0; indexAttribute < VERTEX_ATTRIBUTE_COUNT; indexAttribute++) {
			GLuint attribute = (1 << indexAttribute);

			if (m_vertexFormat & attribute) {
				GLuint location = GetVertexAttributeLocation(attribute);
				GLuint components = GetVertexAttributeComponents(attribute);
				GLuint offset = GetVertexAttributeOffset(m_vertexFormat, attribute);

				glEnableVertexAttribArray(location);
				glVertexAttribPointer(location, components, GL_FLOAT, GL_FALSE, vertexStride, (const void *)offset);
				glVertexAttribDivisor(location, 0);
			}
		}
	}
}
