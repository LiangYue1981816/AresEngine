#include "VKRenderer.h"


CVKUniformBuffer::CVKUniformBuffer(CVKDevice* pDevice, CVKUniformBufferManager* pManager, size_t size)
	: CGfxUniformBuffer(size)
	, m_pDevice(pDevice)
	, m_pManager(pManager)
	, m_pBuffer(nullptr)

	, m_size(size)
	, m_offset(0)
{
	m_pBuffer = new CVKBuffer(pDevice, CGfxSwapChain::SWAPCHAIN_FRAME_COUNT * size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	CGfxProfiler::IncUniformBufferSize(m_pBuffer->GetSize());
}

CVKUniformBuffer::~CVKUniformBuffer(void)
{
	CGfxProfiler::DecUniformBufferSize(m_pBuffer->GetSize());
	delete m_pBuffer;
}

void CVKUniformBuffer::Release(void)
{
	m_pManager->Destroy(this);
}

VkBuffer CVKUniformBuffer::GetBuffer(void) const
{
	return m_pBuffer->GetBuffer();
}

uint32_t CVKUniformBuffer::GetSize(void) const
{
	return m_size;
}

uint32_t CVKUniformBuffer::GetOffset(void) const
{
	return m_offset;
}

bool CVKUniformBuffer::BufferData(size_t offset, size_t size, const void* data)
{
	m_offset = VKRenderer()->GetSwapChain()->GetFrameIndex() * m_size;
	return m_pBuffer->BufferData(m_offset + offset, size, data);
}
