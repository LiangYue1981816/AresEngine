#include "VKRenderer.h"


CVKUniformBuffer::CVKUniformBuffer(CVKDevice *pDevice, size_t size)
	: CGfxUniformBuffer(size)
	, m_pDevice(pDevice)
{
	m_ptrBuffer = CVKBufferPtr(new CVKBuffer(m_pDevice, CGfxSwapChain::SWAPCHAIN_FRAME_COUNT * m_size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));
}

CVKUniformBuffer::~CVKUniformBuffer(void)
{

}

void CVKUniformBuffer::Release(void)
{

}

bool CVKUniformBuffer::BufferData(size_t offset, size_t size, const void *pBuffer)
{
	return true;
}

CVKBufferPtr CVKUniformBuffer::GetBuffer(void) const
{
	return m_ptrBuffer;
}
