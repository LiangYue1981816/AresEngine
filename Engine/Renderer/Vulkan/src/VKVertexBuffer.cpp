#include "VKRenderer.h"


CVKVertexBuffer::CVKVertexBuffer(CVKDevice* pDevice, uint32_t vertexFormat, int vertexBinding, size_t size, bool bDynamic)
	: CGfxVertexBuffer(vertexFormat, vertexBinding, size, bDynamic)
	, m_pDevice(pDevice)

	, m_binding(vertexBinding)
	, m_format(vertexFormat)
	, m_count(size / GetVertexStride(vertexFormat))
	, m_size(size)
	, m_offset(0)
{
	if (bDynamic) {
		m_ptrBuffer = CVKBufferPtr(new CVKBuffer(pDevice, CGfxSwapChain::SWAPCHAIN_FRAME_COUNT * size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));
	}
	else {
		m_ptrBuffer = CVKBufferPtr(new CVKBuffer(pDevice, size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT));
	}

	CGfxProfiler::IncVertexBufferSize(m_ptrBuffer->GetSize());
}

CVKVertexBuffer::~CVKVertexBuffer(void)
{
	CGfxProfiler::DecVertexBufferSize(m_ptrBuffer->GetSize());
}

uint32_t CVKVertexBuffer::GetVertexFormat(void) const
{
	return m_format;
}

uint32_t CVKVertexBuffer::GetVertexCount(void) const
{
	return m_count;
}

uint32_t CVKVertexBuffer::GetSize(void) const
{
	return m_size;
}

uint32_t CVKVertexBuffer::GetOffset(void) const
{
	return m_offset;
}

bool CVKVertexBuffer::BufferData(size_t offset, size_t size, const void* data)
{
	if (m_ptrBuffer->IsHostVisible()) {
		m_offset = VKRenderer()->GetSwapChain()->GetFrameIndex() * m_size;
	}

	return m_ptrBuffer->BufferData(m_offset + offset, size, data);
}

void CVKVertexBuffer::Bind(VkCommandBuffer vkCommandBuffer) const
{
	const VkBuffer vkBuffer = m_ptrBuffer->GetBuffer();
	const VkDeviceSize vkOffset = m_offset;
	vkCmdBindVertexBuffers(vkCommandBuffer, m_binding, 1, &vkBuffer, &vkOffset);
}
