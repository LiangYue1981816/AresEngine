#include "VKRenderer.h"


CVKUniformBuffer::CVKUniformBuffer(CVKDevice *pDevice, CVKUniformBufferManager *pManager, size_t size)
	: CGfxUniformBuffer(size)
	, m_pDevice(pDevice)
	, m_pManager(pManager)

	, m_size(size)
	, m_hash(INVALID_HASHVALUE)
{
	m_ptrBuffer = CVKBufferPtr(new CVKBuffer(m_pDevice, CGfxSwapChain::SWAPCHAIN_FRAME_COUNT * m_size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));
	CGfxProfiler::IncUniformBufferSize(m_ptrBuffer->GetSize());
}

CVKUniformBuffer::~CVKUniformBuffer(void)
{
	CGfxProfiler::DecUniformBufferSize(m_ptrBuffer->GetSize());
}

void CVKUniformBuffer::Release(void)
{
	m_pManager->Destroy(this);
}

HANDLE CVKUniformBuffer::GetBuffer(void) const
{
	return (HANDLE)m_ptrBuffer->GetBuffer();
}

uint32_t CVKUniformBuffer::GetSize(void) const
{
	return m_size;
}

bool CVKUniformBuffer::BufferData(size_t offset, size_t size, const void *pBuffer)
{
	if (m_size < (uint32_t)(offset + size)) {
		return false;
	}

	uint32_t hash = HashValue((uint8_t*)pBuffer, size);

	if (m_hash == hash) {
		return true;
	}

	m_hash = hash;
	return m_ptrBuffer->BufferData(VKRenderer()->GetSwapChain()->GetFrameIndex() * m_size + offset, size, pBuffer);
}
