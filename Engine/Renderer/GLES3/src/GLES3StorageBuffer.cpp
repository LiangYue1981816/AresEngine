#include "GLES3Renderer.h"


CGLES3StorageBuffer::CGLES3StorageBuffer(CGLES3StorageBufferManager* pManager, size_t size)
	: CGfxStorageBuffer(size)
	, m_pManager(pManager)
	, m_pBuffer(nullptr)

	, m_size(0)
	, m_offset(0)
{
	m_size = size;
	m_size = ALIGN_BYTE(m_size, 256);

	m_pBuffer = new CGLES3Buffer(GL_SHADER_STORAGE_BUFFER, m_size * CGfxSwapChain::SWAPCHAIN_FRAME_COUNT, true);
	CGfxProfiler::IncStorageBufferSize(m_pBuffer->GetSize());
}

CGLES3StorageBuffer::~CGLES3StorageBuffer(void)
{
	CGfxProfiler::DecStorageBufferSize(m_pBuffer->GetSize());
	delete m_pBuffer;
}

void CGLES3StorageBuffer::Release(void)
{
	m_pManager->Destroy(this);
}

uint32_t CGLES3StorageBuffer::GetSize(void) const
{
	return m_size;
}

uint32_t CGLES3StorageBuffer::GetOffset(void) const
{
	return m_offset;
}

bool CGLES3StorageBuffer::BufferData(size_t offset, size_t size, const void* data)
{
	m_offset = GLES3Renderer()->GetSwapChain()->GetFrameIndex() * m_size;
	return m_pBuffer->BufferData(m_offset + offset, size, data, false);
}

void CGLES3StorageBuffer::Bind(int binding, int offset, int size) const
{
	m_pBuffer->Bind(binding, m_offset + offset, size);
}
