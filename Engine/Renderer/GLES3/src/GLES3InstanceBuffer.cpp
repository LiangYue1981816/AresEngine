#include "GLES3Renderer.h"


static const int INSTANCE_BUFFER_SIZE = 64;

CGLES3InstanceBuffer::CGLES3InstanceBuffer(uint32_t instanceFormat, int instanceBinding)
	: CGfxInstanceBuffer(instanceFormat, instanceBinding)
	, m_pBuffer(nullptr)

	, m_binding(instanceBinding)
	, m_format(instanceFormat)
	, m_count(0)
{
	m_pBuffer = new CGLES3Buffer(GL_ARRAY_BUFFER, INSTANCE_BUFFER_SIZE, true);
	CGfxProfiler::IncInstanceBufferSize(m_pBuffer->GetSize());
}

CGLES3InstanceBuffer::~CGLES3InstanceBuffer(void)
{
	CGfxProfiler::DecInstanceBufferSize(m_pBuffer->GetSize());
	delete m_pBuffer;
}

void CGLES3InstanceBuffer::Release(void)
{
	delete this;
}

uint32_t CGLES3InstanceBuffer::GetInstanceBinding(void) const
{
	return m_binding;
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
	return m_pBuffer->GetSize();
}

bool CGLES3InstanceBuffer::BufferData(size_t size, const void* data)
{
	m_count = size / GetInstanceStride(m_format);

	if (m_pBuffer->GetSize() < size) {
		CGfxProfiler::DecInstanceBufferSize(m_pBuffer->GetSize());
		{
			size_t newSize = INSTANCE_BUFFER_SIZE;
			while (newSize < size) newSize <<= 1;

			m_pBuffer->BufferSize(newSize, true);
		}
		CGfxProfiler::IncInstanceBufferSize(m_pBuffer->GetSize());
	}

	return m_pBuffer->BufferData(0, size, data, false);
}

void CGLES3InstanceBuffer::Bind(void) const
{
	m_pBuffer->Bind();

	for (uint32_t indexAttribute = 0; indexAttribute < GetInstanceAttributeCount(); indexAttribute++) {
		uint32_t attribute = (1 << indexAttribute);

		if (m_format & attribute) {
			uint32_t stride = GetInstanceStride(m_format);
			uint32_t location = GetInstanceAttributeLocation(attribute);
			uint32_t components = GetInstanceAttributeComponents(attribute);
			uintptr_t offset = GetInstanceAttributeOffset(m_format, attribute);

			glEnableVertexAttribArray(location);
			glVertexAttribPointer(location, components, GL_FLOAT, GL_FALSE, stride, (const void*)offset);
			glVertexAttribDivisor(location, 1);
		}
	}

	CHECK_GL_ERROR_ASSERT();
}


CGLES3MultiInstanceBuffer::CGLES3MultiInstanceBuffer(uint32_t instanceFormat, int instanceBinding, int count)
	: CGfxMultiInstanceBuffer(instanceFormat, instanceBinding, count)
	, m_index(0)
	, m_pBuffers(std::max(1, count))
{
	for (int index = 0; index < m_pBuffers.size(); index++) {
		m_pBuffers[index] = new CGLES3InstanceBuffer(instanceFormat, instanceBinding);
	}
}

CGLES3MultiInstanceBuffer::~CGLES3MultiInstanceBuffer(void)
{
	for (auto& itBuffer : m_pBuffers) {
		delete itBuffer;
	}
}

void CGLES3MultiInstanceBuffer::Release(void)
{
	delete this;
}

void CGLES3MultiInstanceBuffer::SetBufferIndex(int index)
{
	m_index = index;
	m_index = std::min(m_index, (int)m_pBuffers.size() - 1);
	m_index = std::max(m_index, 0);
}

uint32_t CGLES3MultiInstanceBuffer::GetInstanceBinding(void) const
{
	return m_pBuffers[m_index]->GetInstanceBinding();
}

uint32_t CGLES3MultiInstanceBuffer::GetInstanceFormat(void) const
{
	return m_pBuffers[m_index]->GetInstanceFormat();
}

uint32_t CGLES3MultiInstanceBuffer::GetInstanceCount(void) const
{
	return m_pBuffers[m_index]->GetInstanceCount();
}

uint32_t CGLES3MultiInstanceBuffer::GetSize(void) const
{
	return m_pBuffers[m_index]->GetSize();
}

bool CGLES3MultiInstanceBuffer::BufferData(size_t size, const void* data)
{
	return m_pBuffers[m_index]->BufferData(size, data);
}

void CGLES3MultiInstanceBuffer::Bind(void) const
{
	return m_pBuffers[m_index]->Bind();
}
