#include "GLES3Renderer.h"


CGLES3StorageBuffer::CGLES3StorageBuffer(CGLES3StorageBufferManager* pManager, size_t size)
	: CGfxStorageBuffer(size)
	, m_pManager(pManager)
	, m_pBuffer(nullptr)
{
	m_pBuffer = new CGLES3Buffer(GL_SHADER_STORAGE_BUFFER, size, true);
	CGfxProfiler::IncUniformBufferSize(m_pBuffer->GetSize());
}

CGLES3StorageBuffer::~CGLES3StorageBuffer(void)
{
	CGfxProfiler::DecUniformBufferSize(m_pBuffer->GetSize());
	delete m_pBuffer;
}

void CGLES3StorageBuffer::Release(void)
{
	m_pManager->Destroy(this);
}

uint32_t CGLES3StorageBuffer::GetSize(void) const
{
	return m_pBuffer->GetSize();
}

bool CGLES3StorageBuffer::BufferData(size_t offset, size_t size, const void* data)
{
	return m_pBuffer->BufferData(offset, size, data);
}

void CGLES3StorageBuffer::Bind(int binding, int offset, int size) const
{
	m_pBuffer->Bind(binding, offset, size);
}
