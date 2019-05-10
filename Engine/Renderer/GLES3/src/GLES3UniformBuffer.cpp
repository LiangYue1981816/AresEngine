#include "GLES3Renderer.h"


CGLES3UniformBuffer::CGLES3UniformBuffer(CGLES3UniformBufferManager* pManager, size_t size)
	: CGfxUniformBuffer(size)
	, m_pManager(pManager)
{
	ASSERT(m_pManager);

	m_ptrBuffer = CGLES3BufferPtr(new CGLES3Buffer(GL_UNIFORM_BUFFER, size, true));
	CGfxProfiler::IncUniformBufferSize(m_ptrBuffer->GetSize());
}

CGLES3UniformBuffer::~CGLES3UniformBuffer(void)
{
	CGfxProfiler::DecUniformBufferSize(m_ptrBuffer->GetSize());
}

void CGLES3UniformBuffer::Release(void)
{
	m_pManager->Destroy(this);
}

uint32_t CGLES3UniformBuffer::GetSize(void) const
{
	return m_ptrBuffer->GetSize();
}

bool CGLES3UniformBuffer::BufferData(size_t offset, size_t size, const void* data)
{
	return m_ptrBuffer->BufferData(offset, size, data);
}

void CGLES3UniformBuffer::Bind(int binding, int offset, int size) const
{
	m_ptrBuffer->Bind(binding, offset, size);
}
