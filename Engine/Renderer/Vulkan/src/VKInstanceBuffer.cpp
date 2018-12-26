#include "VKRenderer.h"


CVKInstanceBuffer::CVKInstanceBuffer(CVKDevice *pDevice, uint32_t instanceFormat, uint32_t instanceBinding)
	: CGfxInstanceBuffer(instanceFormat, instanceBinding)
	, m_pDevice(pDevice)

	, m_hash(INVALID_HASHVALUE)
	, m_binding(instanceBinding)
	, m_pBuffer(nullptr)
{
	m_pBuffer = new CVKBuffer(m_pDevice, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, CGfxSwapChain::SWAPCHAIN_IMAGE_COUNT * m_size);
}

CVKInstanceBuffer::~CVKInstanceBuffer(void)
{
	delete m_pBuffer;
}

bool CVKInstanceBuffer::BufferData(size_t offset, size_t size, const void *pBuffer)
{
	return true;
}

void CVKInstanceBuffer::Bind(void *pParam)
{

}
