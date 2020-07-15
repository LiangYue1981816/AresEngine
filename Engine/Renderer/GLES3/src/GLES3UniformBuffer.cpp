#include "GLES3Renderer.h"


CGLES3UniformBuffer::CGLES3UniformBuffer(CGLES3UniformBufferManager* pManager, size_t size)
	: CGfxUniformBuffer(size)
	, m_pManager(pManager)
	, m_pBuffer(nullptr)
{
	GLint minOffsetAlign;
	glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &minOffsetAlign);

	size = ALIGN_BYTE(size, minOffsetAlign);

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

bool CGLES3UniformBuffer::BufferData(size_t offset, size_t size, const void* data)
{
	return m_pBuffer->BufferData(offset, size, data, false);
}

void CGLES3UniformBuffer::Bind(int binding, size_t offset, size_t range) const
{
	m_pBuffer->BindRange(binding, offset, range);
}
