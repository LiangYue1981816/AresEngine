#include "VKRenderer.h"


CVKUniformBuffer::CVKUniformBuffer(CVKDevice *pDevice, size_t size, bool bDynamic)
	: CGfxUniformBuffer(size, bDynamic)
	, m_pDevice(pDevice)
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
