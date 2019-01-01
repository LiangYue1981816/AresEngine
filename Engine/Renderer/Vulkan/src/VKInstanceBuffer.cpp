#include "VKRenderer.h"


CVKInstanceBuffer::CVKInstanceBuffer(CVKDevice *pDevice, uint32_t instanceFormat, uint32_t instanceBinding)
	: CGfxInstanceBuffer(instanceFormat, instanceBinding)
	, m_pDevice(pDevice)

	, m_hash(INVALID_HASHVALUE)
	, m_binding(instanceBinding)
{
	m_ptrBuffer = CVKBufferPtr(new CVKBuffer(m_pDevice, CGfxSwapChain::SWAPCHAIN_FRAME_COUNT * m_size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));
}

CVKInstanceBuffer::~CVKInstanceBuffer(void)
{

}

bool CVKInstanceBuffer::BufferData(size_t size, const void *pBuffer)
{
	uint32_t hash = HashValue((uint8_t *)pBuffer, size, 2);

	if (m_hash != hash) {
		m_hash  = hash;
		m_count = size / GetInstanceStride(m_format);

		if (m_size < size) {
			CGfxProfiler::DecInstanceBufferSize(m_size);
			{
				m_size = INSTANCE_BUFFER_SIZE;
				while (m_size < size) m_size <<= 1;

				m_ptrBuffer = CVKBufferPtr(new CVKBuffer(m_pDevice, CGfxSwapChain::SWAPCHAIN_FRAME_COUNT * m_size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));
			}
			CGfxProfiler::IncInstanceBufferSize(m_size);
		}

		return m_ptrBuffer->BufferData(0, size, pBuffer);
	}

	return true;
}

void CVKInstanceBuffer::Bind(VkCommandBuffer vkCommandBuffer, VkDeviceSize offset)
{
	vkCmdBindVertexBuffer(vkCommandBuffer, m_binding, m_ptrBuffer->GetBuffer(), offset);
}
