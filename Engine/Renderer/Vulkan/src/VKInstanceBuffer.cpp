#include "VKRenderer.h"


static const int INSTANCE_BUFFER_SIZE = 64;

CVKInstanceBuffer::CVKInstanceBuffer(CVKDevice* pDevice, uint32_t instanceFormat, int instanceBinding)
	: CGfxInstanceBuffer(instanceFormat, instanceBinding)
	, m_pDevice(pDevice)

	, m_binding(instanceBinding)
	, m_format(instanceFormat)
	, m_count(0)
	, m_size(INSTANCE_BUFFER_SIZE)
	, m_offset(0)
{
	m_ptrBuffer = CVKBufferPtr(new CVKBuffer(pDevice, CGfxSwapChain::SWAPCHAIN_FRAME_COUNT * INSTANCE_BUFFER_SIZE, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));
	CGfxProfiler::IncInstanceBufferSize(m_ptrBuffer->GetSize());
}

CVKInstanceBuffer::~CVKInstanceBuffer(void)
{
	CGfxProfiler::DecInstanceBufferSize(m_ptrBuffer->GetSize());
}

uint32_t CVKInstanceBuffer::GetInstanceFormat(void) const
{
	return m_format;
}

uint32_t CVKInstanceBuffer::GetInstanceCount(void) const
{
	return m_count;
}

uint32_t CVKInstanceBuffer::GetSize(void) const
{
	return m_size;
}

uint32_t CVKInstanceBuffer::GetOffset(void) const
{
	return m_offset;
}

bool CVKInstanceBuffer::BufferData(size_t size, const void* data)
{
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

	m_offset = VKRenderer()->GetSwapChain()->GetFrameIndex() * m_size;
	return m_ptrBuffer->BufferData(m_offset, size, data);
}

void CVKInstanceBuffer::Bind(VkCommandBuffer vkCommandBuffer) const
{
	const VkBuffer vkBuffer = m_ptrBuffer->GetBuffer();
	const VkDeviceSize vkOffset = m_offset;
	vkCmdBindVertexBuffers(vkCommandBuffer, m_binding, 1, &vkBuffer, &vkOffset);
}
