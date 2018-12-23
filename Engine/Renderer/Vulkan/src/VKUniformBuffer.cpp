#include "VKRenderer.h"


CVKUniformBuffer::CVKUniformBuffer(CVKDevice *pDevice, size_t size, bool bDynamic)
	: CVKBufferBase(pDevice, bDynamic ? CGfxSwapChain::SWAPCHAIN_IMAGE_COUNT * size : size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, bDynamic ? VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT : VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
	, CGfxUniformBuffer(bDynamic ? CGfxSwapChain::SWAPCHAIN_IMAGE_COUNT * size : size, bDynamic)
{

}

CVKUniformBuffer::~CVKUniformBuffer(void)
{

}

bool CVKUniformBuffer::BufferData(size_t offset, size_t size, const void *pBuffer)
{
	return CVKBufferBase::BufferData(offset, size, pBuffer);
}

void CVKUniformBuffer::Bind(void *pParam)
{

}
