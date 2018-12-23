#include "VKRenderer.h"


CVKInstanceBuffer::CVKInstanceBuffer(CVKDevice *pDevice, uint32_t instanceFormat, uint32_t instanceBinding)
	: CGfxInstanceBuffer(instanceFormat, instanceBinding)
	, m_pDevice(pDevice)
	, m_memoryPropertyFlags(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)

	, m_hash(INVALID_HASHVALUE)
{

}

CVKInstanceBuffer::~CVKInstanceBuffer(void)
{

}

bool CVKInstanceBuffer::BufferData(size_t offset, size_t size, const void *pBuffer)
{
	return true;
}

void CVKInstanceBuffer::Bind(void *pParam)
{

}
