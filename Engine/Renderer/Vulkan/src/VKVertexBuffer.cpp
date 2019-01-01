#include "VKRenderer.h"


CVKVertexBuffer::CVKVertexBuffer(CVKDevice *pDevice, uint32_t vertexFormat, uint32_t vertexBinding, size_t size, bool bDynamic)
	: CGfxVertexBuffer(vertexFormat, vertexBinding, size, bDynamic)
	, m_pDevice(pDevice)

	, m_binding(vertexBinding)
	, m_transferOffset(0)
{
	if (bDynamic) {
		m_ptrBuffer = CVKBufferPtr(new CVKBuffer(m_pDevice, CGfxSwapChain::SWAPCHAIN_FRAME_COUNT * m_size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));
	}
	else {
		m_ptrBuffer = CVKBufferPtr(new CVKBuffer(m_pDevice, m_size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT));
	}
}

CVKVertexBuffer::~CVKVertexBuffer(void)
{

}

bool CVKVertexBuffer::BufferData(size_t offset, size_t size, const void *pBuffer)
{
	if (m_size < (uint32_t)(offset + size)) {
		return false;
	}

	if (m_ptrBuffer->IsDeviceLocal()) {
		m_transferOffset = offset;
		m_transferBuffer.assign((uint8_t *)pBuffer, (uint8_t *)pBuffer + size);
		return true;
	}
	else {
		return m_ptrBuffer->BufferData(VKRenderer()->GetSwapChain()->GetFrameIndex() * m_size + offset, size, pBuffer);
	}
}

void CVKVertexBuffer::Bind(VkCommandBuffer vkCommandBuffer, VkDeviceSize offset, CVKBufferPtr &ptrBufferTransfer)
{
	if (m_transferBuffer.size()) {
		ptrBufferTransfer = CVKBufferPtr(new CVKBuffer(m_pDevice, m_transferBuffer.size(), VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));
		ptrBufferTransfer->BufferData(0, m_transferBuffer.size(), m_transferBuffer.data());
		vkCmdTransferBuffer(vkCommandBuffer, ptrBufferTransfer->GetBuffer(), m_ptrBuffer->GetBuffer(), VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT, 0, m_transferOffset, m_transferBuffer.size());

		m_transferOffset = 0;
		m_transferBuffer.clear();
	}

	vkCmdBindVertexBuffer(vkCommandBuffer, m_binding, m_ptrBuffer->GetBuffer(), offset);
}
