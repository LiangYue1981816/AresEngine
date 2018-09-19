#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxInstanceBuffer.h"
#include "GfxVertexAttribute.h"


static const int INSTANCE_BUFFER_SIZE = 1 * 1024;

CGfxInstanceBuffer::CGfxInstanceBuffer(GLuint format)
	: m_instanceFormat(format)
	, m_instanceBuffer(0)
	, m_size(0)

	, m_bDirty(false)
{
	glGenBuffers(1, &m_instanceBuffer);
}

CGfxInstanceBuffer::~CGfxInstanceBuffer(void)
{
	glDeleteBuffers(1, &m_instanceBuffer);
}

void CGfxInstanceBuffer::Bind(void) const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_instanceBuffer);
	{
		GLuint instanceStride = GetInstanceStride(m_instanceFormat);

		for (GLuint indexAttribute = 0; indexAttribute < INSTANCE_ATTRIBUTE_COUNT; indexAttribute++) {
			GLuint attribute = (1 << indexAttribute);

			if (m_instanceFormat & attribute) {
				GLuint location = GetInstanceAttributeLocation(attribute);
				GLuint components = GetInstanceAttributeComponents(attribute);
				GLuint offset = GetInstanceAttributeOffset(m_instanceFormat, attribute);

				glEnableVertexAttribArray(location);
				glVertexAttribPointer(location, components, GL_FLOAT, GL_FALSE, instanceStride, (const void *)offset);
				glVertexAttribDivisor(location, 1);
			}
		}
	}
}

void CGfxInstanceBuffer::AddInstance(const glm::mat4 &mtxTransform)
{
	m_bDirty = true;
	m_instanceDatas.push_back(mtxTransform);
}

void CGfxInstanceBuffer::SetInstance(const eastl::vector<glm::mat4> &mtxTransforms)
{
	m_bDirty = true;
	m_instanceDatas = mtxTransforms;
}

void CGfxInstanceBuffer::ClearInstance(void)
{
	m_bDirty = true;
	m_instanceDatas.clear();
}

void CGfxInstanceBuffer::UpdateInstance(void)
{
	if (m_bDirty) {
		m_bDirty = false;

		GLsizeiptr size = (GLsizeiptr)m_instanceDatas.size() * sizeof(glm::mat4);

		if (m_size < size) {
			m_size = INSTANCE_BUFFER_SIZE;
			while (m_size < size) m_size <<= 1;

			glBindBuffer(GL_ARRAY_BUFFER, m_instanceBuffer);
			glBufferData(GL_ARRAY_BUFFER, m_size, NULL, GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		glBindBuffer(GL_ARRAY_BUFFER, m_instanceBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, m_instanceDatas.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

GLuint CGfxInstanceBuffer::GetInstanceCount(void) const
{
	return (GLuint)m_instanceDatas.size();
}

GLuint CGfxInstanceBuffer::GetInstanceFormat(void) const
{
	return m_instanceFormat;
}

GLuint CGfxInstanceBuffer::GetInstanceBuffer(void) const
{
	return m_instanceBuffer;
}

GLsizeiptr CGfxInstanceBuffer::GetSize(void) const
{
	return m_size;
}
