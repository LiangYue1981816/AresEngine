#include "VKRenderer.h"


CVKVertexBuffer::CVKVertexBuffer(CVKDevice* pDevice, uint32_t vertexFormat, uint32_t vertexBinding, size_t size, bool bDynamic)
	: CGfxVertexBuffer(vertexFormat, vertexBinding, size, bDynamic)
	, m_pDevice(pDevice)

	, m_format(vertexFormat)
	, m_count(size / GetVertexStride(vertexFormat))
	, m_size(size)

	, m_binding(vertexBinding)
	, m_transferOffset(0)
{
	if (bDynamic) {
		m_ptrBuffer = CVKBufferPtr(new CVKBuffer(m_pDevice, CGfxSwapChain::SWAPCHAIN_FRAME_COUNT * m_size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));
		CGfxProfiler::IncVertexBufferSize(m_ptrBuffer->GetSize());
	}
	else {
		m_ptrBuffer = CVKBufferPtr(new CVKBuffer(m_pDevice, m_size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT));
		CGfxProfiler::IncVertexBufferSize(m_ptrBuffer->GetSize());
	}
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

bool CVKVertexBuffer::BufferData(size_t offset, size_t size, const void* pBuffer)
{
	if (m_size < (uint32_t)(offset + size)) {
		return false;
	}

	if (m_ptrBuffer->IsDeviceLocal()) {
		m_transferOffset = offset;
		m_transferBuffer.assign((uint8_t*)pBuffer, (uint8_t*)pBuffer + size);
		return true;
	}
	else {
		return m_ptrBuffer->BufferData(VKRenderer()->GetSwapChain()->GetFrameIndex() * m_size + offset, size, pBuffer);
	}
}

void CVKVertexBuffer::Bind(VkCommandBuffer vkCommandBuffer, VkDeviceSize offset) const
{
	vkCmdBindVertexBuffer(vkCommandBuffer, m_binding, m_ptrBuffer->GetBuffer(), offset);
}

CVKBufferPtr CVKVertexBuffer::TransferBuffer(VkCommandBuffer vkCommandBuffer)
{
	CVKBufferPtr ptrBuffer;

	if (m_transferBuffer.size()) {
		ptrBuffer = CVKBufferPtr(new CVKBuffer(m_pDevice, m_transferBuffer.size(), VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));
		ptrBuffer->BufferData(0, m_transferBuffer.size(), m_transferBuffer.data());
		vkCmdTransferBuffer(vkCommandBuffer, ptrBuffer->GetBuffer(), m_ptrBuffer->GetBuffer(), VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT, 0, m_transferOffset, m_transferBuffer.size());

		m_transferOffset = 0;
		m_transferBuffer.clear();
		m_transferBuffer.shrink_to_fit();
	}

	return ptrBuffer;
}
