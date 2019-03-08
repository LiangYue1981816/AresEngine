#include "GLES3Renderer.h"


static const int INSTANCE_BUFFER_SIZE = 64;

CGLES3InstanceBuffer::CGLES3InstanceBuffer(uint32_t instanceFormat, uint32_t instanceBinding)
	: CGLES3Buffer(GL_ARRAY_BUFFER, INSTANCE_BUFFER_SIZE, true)
	, CGfxInstanceBuffer(instanceFormat, instanceBinding)

	, m_format(instanceFormat)
	, m_count(0)

	, m_hash(INVALID_HASHVALUE)
{
	CGfxProfiler::IncInstanceBufferSize(m_size);
}

CGLES3InstanceBuffer::~CGLES3InstanceBuffer(void)
{
	CGfxProfiler::DecInstanceBufferSize(m_size);
}

uint32_t CGLES3InstanceBuffer::GetInstanceFormat(void) const
{
	return m_format;
}

uint32_t CGLES3InstanceBuffer::GetInstanceCount(void) const
{
	return m_count;
}

uint32_t CGLES3InstanceBuffer::GetSize(void) const
{
	return m_size;
}

bool CGLES3InstanceBuffer::BufferData(size_t size, const void *pBuffer)
{
	uint32_t hash = HashValue((uint8_t *)pBuffer, size, 2);

	if (m_hash != hash) {
		m_hash  = hash;
		m_count = size / GetInstanceStride(m_format);

		GLBindBuffer(m_target, m_buffer);
		{
			if (m_size < size) {
				CGfxProfiler::DecInstanceBufferSize(m_size);
				{
					m_size = INSTANCE_BUFFER_SIZE;
					while (m_size < size) m_size <<= 1;

					glBufferData(m_target, m_size, nullptr, GL_DYNAMIC_DRAW);
				}
				CGfxProfiler::IncInstanceBufferSize(m_size);
			}

			glBufferSubData(m_target, 0, size, pBuffer);
		}
	}

	return true;
}

void CGLES3InstanceBuffer::Bind(void) const
{
	GLBindBuffer(m_target, m_buffer);

	for (uint32_t indexAttribute = 0; indexAttribute < GetInstanceAttributeCount(); indexAttribute++) {
		uint32_t attribute = (1 << indexAttribute);

		if (m_format & attribute) {
			uint32_t stride = GetInstanceStride(m_format);
			uint32_t location = GetInstanceAttributeLocation(attribute);
			uint32_t components = GetInstanceAttributeComponents(attribute);
			uintptr_t offset = GetInstanceAttributeOffset(m_format, attribute);

			glEnableVertexAttribArray(location);
			glVertexAttribPointer(location, components, GL_FLOAT, GL_FALSE, stride, (const void *)offset);
			glVertexAttribDivisor(location, 1);
		}
	}
}
