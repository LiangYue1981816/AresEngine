#include "GLES3Renderer.h"


CGLES3UniformBuffer::CGLES3UniformBuffer(CGLES3UniformBufferManager* pManager, size_t size)
	: CGfxUniformBuffer(size)
	, m_pManager(pManager)
	, m_pBuffer(nullptr)

	, m_size(0)
	, m_offset(0)
{
	GLint minOffsetAlign;
	glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &minOffsetAlign);

	m_size = size;
	m_size = ALIGN_BYTE(m_size, minOffsetAlign);

	m_pBuffer = new CGLES3Buffer(GL_UNIFORM_BUFFER, m_size * CGfxSwapChain::SWAPCHAIN_FRAME_COUNT, true);
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
	return m_size;
}

uint32_t CGLES3UniformBuffer::GetOffset(void) const
{
	return m_offset;
}

bool CGLES3UniformBuffer::BufferData(size_t offset, size_t size, const void* data)
{
	m_offset = m_size * GLES3Renderer()->GetSwapChain()->GetFrameIndex();
	return m_pBuffer->BufferData(m_offset + offset, size, data, false);
}

void CGLES3UniformBuffer::Bind(int binding, int offset, int size) const
{
	m_pBuffer->Bind(binding, m_offset + offset, size);
}
