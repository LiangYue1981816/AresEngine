#include "GLES3Renderer.h"


CGLES3UniformBuffer::CGLES3UniformBuffer(CGLES3UniformBufferManager* pManager, size_t size)
	: CGfxUniformBuffer(size)
	, m_pManager(pManager)
	, m_pBuffer(nullptr)
{
	ASSERT(m_pManager);

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
	return m_pBuffer->BufferData(offset, size, data);
}

void CGLES3UniformBuffer::Bind(int binding, int offset, int size) const
{
	m_pBuffer->Bind(binding, offset, size);
}
