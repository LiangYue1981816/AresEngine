#include "VKRenderer.h"


CVKIndexBuffer::CVKIndexBuffer(CVKDevice *pDevice, GfxIndexType type, size_t size, bool bDynamic)
	: CGfxIndexBuffer(type, size, bDynamic)
	, m_pDevice(pDevice)

	, m_bNeedTransfer(false)
	, m_transferOffset(0)
	, m_transferSize(0)
{
	if (bDynamic) {
		m_ptrBuffer = CVKBufferPtr(new CVKBuffer(m_pDevice, CGfxSwapChain::SWAPCHAIN_FRAME_COUNT * m_size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));
	}
	else {
		m_ptrBuffer = CVKBufferPtr(new CVKBuffer(m_pDevice, m_size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT));
		m_ptrBufferTransfer = CVKBufferPtr(new CVKBuffer(m_pDevice, m_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));
	}
}

CVKIndexBuffer::~CVKIndexBuffer(void)
{

}

bool CVKIndexBuffer::BufferData(size_t offset, size_t size, const void *pBuffer)
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
		return m_ptrBuffer->BufferData(VKRenderer()->GetSwapChain()->GetFrameIndex() * m_size + offset, size, pBuffer);
	}
}

void CVKIndexBuffer::Bind(VkCommandBuffer vkCommandBuffer, VkDeviceSize offset)
{
	if (m_bNeedTransfer) {
		m_bNeedTransfer = false;
		vkCmdTransferBuffer(vkCommandBuffer, m_ptrBufferTransfer->m_vkBuffer, m_ptrBuffer->m_vkBuffer, VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT, m_transferOffset, m_transferOffset, m_transferSize);
	}

	switch ((int)GetIndexType()) {
	case GFX_INDEX_UNSIGNED_SHORT:
		vkCmdBindIndexBuffer(vkCommandBuffer, m_ptrBuffer->m_vkBuffer, offset, VK_INDEX_TYPE_UINT16);
		break;

	case GFX_INDEX_UNSIGNED_INT:
		vkCmdBindIndexBuffer(vkCommandBuffer, m_ptrBuffer->m_vkBuffer, offset, VK_INDEX_TYPE_UINT32);
		break;
	}
}

CVKBufferPtr CVKIndexBuffer::GetBuffer(void) const
{
	return m_ptrBuffer;
}

CVKBufferPtr CVKIndexBuffer::GetBufferTransfer(void) const
{
	return m_ptrBufferTransfer;
}
