#include "GLES3Renderer.h"


CGLES3UniformBuffer::CGLES3UniformBuffer(CGLES3UniformBufferManager* pManager, size_t size)
	: CGfxUniformBuffer(size)
	, m_pManager(pManager)
	, m_pBuffer(nullptr)

	, m_range(0)
	, m_offset(0)
{
	GLint minOffsetAlign;
	glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &minOffsetAlign);

	size = ALIGN_BYTE(size, minOffsetAlign);

	m_range = size;
	m_pBuffer = new CGLES3Buffer(GL_UNIFORM_BUFFER, size, true);
	CGfxProfiler::IncUniformBufferSize(m_pBuffer->GetSize());
}

CGLES3UniformBuffer::~CGLES3UniformBuffer(void)
{
	CGfxProfiler::DecUniformBufferSize(m_pBuffer->GetSize());
	delete m_pBuffer;
}

void CGLES3UniformBuffer::Release(void)
{
	m_pManager->Destroy(this);
}

uint32_t CGLES3UniformBuffer::GetSize(void) const
{
	return m_pBuffer->GetSize();
}

uint32_t CGLES3UniformBuffer::GetRange(void) const
{
	return m_range;
}

uint32_t CGLES3UniformBuffer::GetOffset(void) const
{
	return m_offset;
}

bool CGLES3UniformBuffer::BufferRange(size_t offset, size_t range)
{
	if (offset + range <= m_pBuffer->GetSize()) {
		m_offset = offset;
		m_range = range;
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3UniformBuffer::BufferData(size_t offset, size_t size, const void* data)
{
	return m_pBuffer->BufferData(offset, size, data, false);
}

void CGLES3UniformBuffer::Bind(int binding) const
{
	m_pBuffer->Bind(binding, m_offset, m_range);
}
