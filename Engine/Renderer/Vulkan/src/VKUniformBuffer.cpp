#include "VKRenderer.h"


CVKUniformBuffer::CVKUniformBuffer(CVKDevice *pDevice, size_t size)
	: CGfxUniformBuffer(size)
	, m_pDevice(pDevice)
{
	m_ptrBuffer = CVKBufferPtr(new CVKBuffer(m_pDevice, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, CGfxSwapChain::SWAPCHAIN_IMAGE_COUNT * m_size));
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
