#include "VKRenderer.h"


CVKUniformBuffer::CVKUniformBuffer(CVKDevice *pDevice, size_t size, bool bDynamic)
	: CGfxUniformBuffer(size, bDynamic)
	, m_pDevice(pDevice)

	, m_vkBuffer(VK_NULL_HANDLE)
	, m_usage(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT)
	, m_memoryPropertyFlags(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
{

}

CVKUniformBuffer::~CVKUniformBuffer(void)
{

}

bool CVKUniformBuffer::BufferData(size_t offset, size_t size, const void *pBuffer)
{
	return true;
}

void CVKUniformBuffer::Bind(void *pParam)
{

}
