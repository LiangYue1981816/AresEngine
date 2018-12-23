#include "VKRenderer.h"


CVKIndexBuffer::CVKIndexBuffer(CVKDevice *pDevice, GfxIndexType type, size_t size, bool bDynamic)
	: CGfxIndexBuffer(type, size, bDynamic)
	, m_pDevice(pDevice)
	, m_memoryPropertyFlags(bDynamic ? VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT : VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
{

}

CVKIndexBuffer::~CVKIndexBuffer(void)
{

}

bool CVKIndexBuffer::BufferData(size_t offset, size_t size, const void *pBuffer)
{
	return true;
}

void CVKIndexBuffer::Bind(void *pParam)
{

}
