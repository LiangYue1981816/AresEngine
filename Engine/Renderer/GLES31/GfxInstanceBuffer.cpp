#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxInstanceBuffer.h"
#include "GfxVertexAttribute.h"


static const int INSTANCE_BUFFER_SIZE = 128;

CGfxInstanceBuffer::CGfxInstanceBuffer(uint32_t format)
	: m_instanceFormat(format)
	, m_instanceBuffer(0)
	, m_size(INSTANCE_BUFFER_SIZE)

	, m_bDirty(false)
	, m_hash(INVALID_VALUE)
{
	CGfxProfiler::IncInstanceBufferSize(m_size);

	glGenBuffers(1, &m_instanceBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_instanceBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_size, NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

CGfxInstanceBuffer::~CGfxInstanceBuffer(void)
{
	CGfxProfiler::DecInstanceBufferSize(m_size);

	glDeleteBuffers(1, &m_instanceBuffer);
	m_instanceFormat = 0;
	m_instanceBuffer = 0;
	m_size = 0;
}

void CGfxInstanceBuffer::SetInstance(const eastl::vector<glm::mat4> &mtxTransforms)
{
	m_bDirty = true;
	m_instances = mtxTransforms;
}

void CGfxInstanceBuffer::UpdateInstance(void)
{
	if (m_bDirty) {
		m_bDirty = false;

		uint32_t size = (uint32_t)m_instances.size() * sizeof(glm::mat4);
		uint32_t hash = HashValue((unsigned char *)m_instances.data(), size);

		if (m_hash != hash) {
			m_hash  = hash;

			GLBindBuffer(GL_ARRAY_BUFFER, m_instanceBuffer);
			{
				if (m_size < size) {
					CGfxProfiler::DecInstanceBufferSize(m_size);
					{
						m_size = INSTANCE_BUFFER_SIZE;
						while (m_size < size) m_size <<= 1;

						glBufferData(GL_ARRAY_BUFFER, m_size, NULL, GL_DYNAMIC_DRAW);
					}
					CGfxProfiler::IncInstanceBufferSize(m_size);
				}

				glBufferSubData(GL_ARRAY_BUFFER, 0, size, m_instances.data());
			}
			GLBindBuffer(GL_ARRAY_BUFFER, 0);
		}
	}
}

uint32_t CGfxInstanceBuffer::GetInstanceCount(void) const
{
	return (uint32_t)m_instances.size();
}

uint32_t CGfxInstanceBuffer::GetInstanceFormat(void) const
{
	return m_instanceFormat;
}

uint32_t CGfxInstanceBuffer::GetInstanceBuffer(void) const
{
	return m_instanceBuffer;
}

uint32_t CGfxInstanceBuffer::GetSize(void) const
{
	return m_size;
}

void CGfxInstanceBuffer::Bind(void) const
{
	GLBindBuffer(GL_ARRAY_BUFFER, m_instanceBuffer);
}

void CGfxInstanceBuffer::SetupFormat(void) const
{
	uint32_t instanceStride = GetInstanceStride(m_instanceFormat);

	for (uint32_t indexAttribute = 0; indexAttribute < INSTANCE_ATTRIBUTE_COUNT; indexAttribute++) {
		uint32_t attribute = (1 << indexAttribute);

		if (m_instanceFormat & attribute) {
			uint32_t location = GetInstanceAttributeLocation(attribute);
			uint32_t components = GetInstanceAttributeComponents(attribute);
			uint32_t offset = GetInstanceAttributeOffset(m_instanceFormat, attribute);

			glEnableVertexAttribArray(location);
			glVertexAttribPointer(location, components, GL_FLOAT, GL_FALSE, instanceStride, (const void *)offset);
			glVertexAttribDivisor(location, 1);
		}
	}
}
