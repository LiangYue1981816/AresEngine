#include "GLES3Renderer.h"


static const int INSTANCE_BUFFER_SIZE = 64;

CGLES3InstanceBuffer::CGLES3InstanceBuffer(uint32_t instanceFormat, uint32_t instanceBinding)
	: CGfxInstanceBuffer(instanceFormat, instanceBinding)
	, m_format(instanceFormat)
	, m_count(0)
{
	m_ptrBuffer = CGLES3BufferPtr(new CGLES3Buffer(GL_ARRAY_BUFFER, INSTANCE_BUFFER_SIZE, true));
	CGfxProfiler::IncInstanceBufferSize(m_ptrBuffer->GetSize());
}

CGLES3InstanceBuffer::~CGLES3InstanceBuffer(void)
{
	CGfxProfiler::DecInstanceBufferSize(m_ptrBuffer->GetSize());
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
	return m_ptrBuffer->GetSize();
}

bool CGLES3InstanceBuffer::BufferData(size_t size, const void *pBuffer)
{
	m_count = size / GetInstanceStride(m_format);

	if (m_ptrBuffer->GetSize() < size) {
		CGfxProfiler::DecInstanceBufferSize(m_ptrBuffer->GetSize());
		{
			size_t newSize = INSTANCE_BUFFER_SIZE;
			while (newSize < size) newSize <<= 1;

			m_ptrBuffer->BufferSize(newSize, true);
		}
		CGfxProfiler::IncInstanceBufferSize(m_ptrBuffer->GetSize());
	}

	return m_ptrBuffer->BufferData(0, size, pBuffer);
}

void CGLES3InstanceBuffer::Bind(void) const
{
	m_ptrBuffer->Bind();

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
