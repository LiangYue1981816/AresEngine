#include "GLES3Renderer.h"


CGLES3UniformBuffer::CGLES3UniformBuffer(CGLES3UniformBufferManager *pManager, size_t size)
	: CGLES3Buffer(GL_UNIFORM_BUFFER, size, true)
	, CGfxUniformBuffer(size)
	, m_pManager(pManager)
{
	CGfxProfiler::IncUniformBufferSize(m_size);
}

CGLES3UniformBuffer::~CGLES3UniformBuffer(void)
{
	CGfxProfiler::DecUniformBufferSize(m_size);
}

void CGLES3UniformBuffer::Release(void)
{
	m_pManager->Destroy(this);
}

HANDLE CGLES3UniformBuffer::GetBuffer(void) const
{
	return (HANDLE)m_buffer;
}

uint32_t CGLES3UniformBuffer::GetSize(void) const
{
	return CGLES3Buffer::GetSize();
}

bool CGLES3UniformBuffer::BufferData(size_t offset, size_t size, const void *pBuffer)
{
	return CGLES3Buffer::BufferData(offset, size, pBuffer);
}

void CGLES3UniformBuffer::Bind(int index, int offset, int size)
{
	if (m_size < (uint32_t)(offset + size)) {
		return;
	}

	GLBindBufferRange(GL_UNIFORM_BUFFER, index, m_buffer, offset, size);
}
