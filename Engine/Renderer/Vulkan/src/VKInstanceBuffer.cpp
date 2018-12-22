#include "VKRenderer.h"


CVKInstanceBuffer::CVKInstanceBuffer(CVKDevice *pDevice, uint32_t instanceFormat, uint32_t instanceBinding)
	: CVKBufferBase(pDevice, INSTANCE_BUFFER_SIZE, VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
	, CGfxInstanceBuffer(instanceFormat, instanceBinding)

	, m_hash(INVALID_HASHVALUE)
{

}

CVKInstanceBuffer::~CVKInstanceBuffer(void)
{

}

bool CVKInstanceBuffer::BufferData(size_t offset, size_t size, const void *pBuffer)
{
	return CVKBufferBase::BufferData(offset, size, pBuffer);
}

void CVKInstanceBuffer::Bind(void *pParam)
{

}
