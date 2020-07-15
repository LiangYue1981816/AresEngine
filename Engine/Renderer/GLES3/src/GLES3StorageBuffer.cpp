#include "GLES3Renderer.h"


CGLES3StorageBuffer::CGLES3StorageBuffer(CGLES3StorageBufferManager* pManager, size_t size)
	: CGfxStorageBuffer(size)
	, m_pManager(pManager)
	, m_pBuffer(nullptr)

	, m_range(0)
	, m_offset(0)
{
	GLint minOffsetAlign;
	glGetIntegerv(GL_SHADER_STORAGE_BUFFER_OFFSET_ALIGNMENT, &minOffsetAlign);

	size = ALIGN_BYTE(size, minOffsetAlign);

	m_range = size;
	m_pBuffer = new CGLES3Buffer(GL_SHADER_STORAGE_BUFFER, size, true);
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
	return m_pBuffer->GetSize();
}

uint32_t CGLES3StorageBuffer::GetRange(void) const
{
	return m_range;
}

uint32_t CGLES3StorageBuffer::GetOffset(void) const
{
	return m_offset;
}

bool CGLES3StorageBuffer::BufferRange(size_t offset, size_t range)
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

bool CGLES3StorageBuffer::BufferData(size_t offset, size_t size, const void* data)
{
	return m_pBuffer->BufferData(offset, size, data, false);
}

void CGLES3StorageBuffer::Bind(int binding) const
{
	m_pBuffer->Bind(binding, m_offset, m_range);
}
