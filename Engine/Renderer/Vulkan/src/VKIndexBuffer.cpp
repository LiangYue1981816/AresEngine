#include "VKRenderer.h"


CVKIndexBuffer::CVKIndexBuffer(CVKDevice *pDevice, GfxIndexType type, size_t size, bool bDynamic)
	: CVKBufferBase(pDevice, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, bDynamic ? VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT : VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
	, CGfxIndexBuffer(type, size, bDynamic)
{

}

CVKIndexBuffer::~CVKIndexBuffer(void)
{

}

bool CVKIndexBuffer::BufferData(size_t offset, size_t size, const void *pBuffer)
{
	return CVKBufferBase::BufferData(offset, size, pBuffer);
}

void CVKIndexBuffer::Bind(void *pParam)
{

}
