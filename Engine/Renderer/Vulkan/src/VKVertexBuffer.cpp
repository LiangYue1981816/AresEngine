#include "VKRenderer.h"


CVKVertexBuffer::CVKVertexBuffer(CVKDevice* pDevice, uint32_t vertexFormat, int vertexBinding, size_t size, bool bDynamic)
	: CGfxVertexBuffer(vertexFormat, vertexBinding, size, bDynamic)
	, m_pDevice(pDevice)
	, m_pBuffer(nullptr)

	, m_binding(vertexBinding)
	, m_format(vertexFormat)
	, m_count(size / GetVertexStride(vertexFormat))
	, m_size(size)
	, m_offset(0)
{
	if (bDynamic) {
		m_pBuffer = new CVKBuffer(pDevice, CGfxSwapChain::SWAPCHAIN_FRAME_COUNT * size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
		CGfxProfiler::IncVertexBufferSize(m_pBuffer->GetSize());
	}
	else {
		m_pBuffer = new CVKBuffer(pDevice, size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		CGfxProfiler::IncVertexBufferSize(m_pBuffer->GetSize());
	}
}

CVKVertexBuffer::~CVKVertexBuffer(void)
{
	CGfxProfiler::DecVertexBufferSize(m_pBuffer->GetSize());
	delete m_pBuffer;
}

uint32_t CVKVertexBuffer::GetVertexBinding(void) const
{
	return m_binding;
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
	m_offset = m_pBuffer->IsHostVisible() ? VKRenderer()->GetSwapChain()->GetFrameIndex() * m_size : 0;
	return m_pBuffer->BufferData(m_offset + offset, size, data);
}

void CVKVertexBuffer::Bind(VkCommandBuffer vkCommandBuffer) const
{
	ASSERT(vkCommandBuffer);
	vkCmdBindVertexBuffer(vkCommandBuffer, m_binding, m_pBuffer->GetBuffer(), m_offset);
}
