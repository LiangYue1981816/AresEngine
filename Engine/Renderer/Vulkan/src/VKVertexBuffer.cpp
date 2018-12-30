#include "VKRenderer.h"


CVKVertexBuffer::CVKVertexBuffer(CVKDevice *pDevice, uint32_t vertexFormat, uint32_t vertexBinding, size_t size, bool bDynamic)
	: CGfxVertexBuffer(vertexFormat, vertexBinding, size, bDynamic)
	, m_pDevice(pDevice)

	, m_binding(vertexBinding)

	, m_bNeedTransfer(false)
	, m_transferOffset(0)
	, m_transferSize(0)
{
	if (bDynamic) {
		m_ptrBuffer = CVKBufferPtr(new CVKBuffer(m_pDevice, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, CGfxSwapChain::SWAPCHAIN_IMAGE_COUNT * m_size));
	}
	else {
		m_ptrBuffer = CVKBufferPtr(new CVKBuffer(m_pDevice, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_size));
		m_ptrBufferTransfer = CVKBufferPtr(new CVKBuffer(m_pDevice, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_size));
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

	if (m_ptrBufferTransfer.IsValid()) {
		m_bNeedTransfer = true;
		m_transferOffset = offset;
		m_transferSize = size;
		return m_ptrBufferTransfer->BufferData(offset, size, pBuffer);
	}
	else {
		return m_ptrBuffer->BufferData(VKRenderer()->GetSwapChain()->GetRenderTextureIndex() * m_size + offset, size, pBuffer);
	}
}

void CVKVertexBuffer::Bind(VkCommandBuffer vkCommandBuffer, VkDeviceSize offset)
{
	if (m_bNeedTransfer) {
		m_bNeedTransfer = false;
		vkCmdTransferBuffer(vkCommandBuffer, m_ptrBufferTransfer->m_vkBuffer, m_ptrBuffer->m_vkBuffer, VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT, m_transferOffset, m_transferOffset, m_transferSize);
	}

	vkCmdBindVertexBuffers(vkCommandBuffer, m_binding, 1, &m_ptrBuffer->m_vkBuffer, &offset);
}

CVKBufferPtr CVKVertexBuffer::GetBuffer(void) const
{
	return m_ptrBuffer;
}

CVKBufferPtr CVKVertexBuffer::GetBufferTransfer(void) const
{
	return m_ptrBufferTransfer;
}
