#include "VKRenderer.h"


CVKIndexBuffer::CVKIndexBuffer(CVKDevice *pDevice, GfxIndexType type, size_t size, bool bDynamic)
	: CGfxIndexBuffer(type, size, bDynamic)
	, m_pDevice(pDevice)

	, m_pBuffer(nullptr)
	, m_pBufferTransfer(nullptr)

	, m_bNeedTransfer(false)
	, m_transferOffset(0)
	, m_transferSize(0)
{
	if (bDynamic) {
		m_pBuffer = new CVKBuffer(m_pDevice, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, CGfxSwapChain::SWAPCHAIN_IMAGE_COUNT * m_size);
	}
	else {
		m_pBuffer = new CVKBuffer(m_pDevice, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_size);
		m_pBufferTransfer = new CVKBuffer(m_pDevice, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_size);
	}
}

CVKIndexBuffer::~CVKIndexBuffer(void)
{
	delete m_pBuffer;
	delete m_pBufferTransfer;
}

bool CVKIndexBuffer::BufferData(size_t offset, size_t size, const void *pBuffer)
{
	if (m_size < (uint32_t)(offset + size)) {
		return false;
	}

	if (m_pBufferTransfer) {
		m_bNeedTransfer = true;
		m_transferOffset = offset;
		m_transferSize = size;
		return m_pBufferTransfer->BufferData(offset, size, pBuffer);
	}
	else {
		return m_pBuffer->BufferData(VKRenderer()->GetSwapChain()->GetTextureIndex() * m_size + offset, size, pBuffer);
	}
}

void CVKIndexBuffer::Bind(VkCommandBuffer vkCommandBuffer, VkDeviceSize offset)
{
	if (m_bNeedTransfer) {
		m_bNeedTransfer = false;
		vkCmdTransferBuffer(vkCommandBuffer, m_pBufferTransfer->m_vkBuffer, m_pBuffer->m_vkBuffer, VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT, m_transferOffset, m_transferOffset, m_transferSize);
	}

	switch ((int)GetIndexType()) {
	case GFX_INDEX_UNSIGNED_SHORT:
		vkCmdBindIndexBuffer(vkCommandBuffer, m_pBuffer->m_vkBuffer, offset, VK_INDEX_TYPE_UINT16);
		break;

	case GFX_INDEX_UNSIGNED_INT:
		vkCmdBindIndexBuffer(vkCommandBuffer, m_pBuffer->m_vkBuffer, offset, VK_INDEX_TYPE_UINT32);
		break;
	}
}
