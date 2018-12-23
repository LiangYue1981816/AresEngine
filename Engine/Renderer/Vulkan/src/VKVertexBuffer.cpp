#include "VKRenderer.h"


CVKVertexBuffer::CVKVertexBuffer(CVKDevice *pDevice, uint32_t vertexFormat, uint32_t vertexBinding, size_t size, bool bDynamic)
	: CGfxVertexBuffer(vertexFormat, vertexBinding, size, bDynamic)
	, m_pDevice(pDevice)
	, m_memoryPropertyFlags(bDynamic ? VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT : VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
{

}

CVKVertexBuffer::~CVKVertexBuffer(void)
{

}

bool CVKVertexBuffer::BufferData(size_t offset, size_t size, const void *pBuffer)
{
	return true;
}

void CVKVertexBuffer::Bind(void *pParam)
{

}
