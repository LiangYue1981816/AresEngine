#include "VKRenderer.h"


CVKIndexBuffer::CVKIndexBuffer(CVKDevice *pDevice, GfxIndexType type, size_t size, bool bDynamic)
	: CGfxIndexBuffer(type, size, bDynamic)
	, m_pDevice(pDevice)

	, m_type(type)
	, m_size(size)

	, m_transferOffset(0)
{
	if (bDynamic) {
		m_ptrBuffer = CVKBufferPtr(new CVKBuffer(m_pDevice, CGfxSwapChain::SWAPCHAIN_FRAME_COUNT * m_size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));
		CGfxProfiler::IncIndexBufferSize(m_ptrBuffer->GetSize());
	}
	else {
		m_ptrBuffer = CVKBufferPtr(new CVKBuffer(m_pDevice, m_size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT));
		CGfxProfiler::IncIndexBufferSize(m_ptrBuffer->GetSize());
	}
}

CVKIndexBuffer::~CVKIndexBuffer(void)
{
	CGfxProfiler::DecIndexBufferSize(m_ptrBuffer->GetSize());
}

GfxIndexType CVKIndexBuffer::GetIndexType(void) const
{
	return m_type;
}

uint32_t CVKIndexBuffer::GetIndexCount(void) const
{
	switch (m_type) {
	case GFX_INDEX_UNSIGNED_SHORT: return m_size / 2;
	case GFX_INDEX_UNSIGNED_INT:   return m_size / 4;
	default:                       return 0;
	}
}

uint32_t CVKIndexBuffer::GetSize(void) const
{
	return m_size;
}

bool CVKIndexBuffer::BufferData(size_t offset, size_t size, const void *pBuffer)
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

void CVKIndexBuffer::Bind(VkCommandBuffer vkCommandBuffer, VkDeviceSize offset) const
{
	switch ((int)GetIndexType()) {
	case GFX_INDEX_UNSIGNED_SHORT:
		vkCmdBindIndexBuffer(vkCommandBuffer, m_ptrBuffer->GetBuffer(), offset, VK_INDEX_TYPE_UINT16);
		break;

	case GFX_INDEX_UNSIGNED_INT:
		vkCmdBindIndexBuffer(vkCommandBuffer, m_ptrBuffer->GetBuffer(), offset, VK_INDEX_TYPE_UINT32);
		break;
	}
}

CVKBufferPtr CVKIndexBuffer::BufferTransfer(VkCommandBuffer vkCommandBuffer)
{
	CVKBufferPtr ptrBufferTransfer;

	if (m_transferBuffer.size()) {
		ptrBufferTransfer = CVKBufferPtr(new CVKBuffer(m_pDevice, m_transferBuffer.size(), VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));
		ptrBufferTransfer->BufferData(0, m_transferBuffer.size(), m_transferBuffer.data());
		vkCmdTransferBuffer(vkCommandBuffer, ptrBufferTransfer->GetBuffer(), m_ptrBuffer->GetBuffer(), VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT, 0, m_transferOffset, m_transferBuffer.size());

		m_transferOffset = 0;
		m_transferBuffer.clear();
		m_transferBuffer.shrink_to_fit();
	}

	return ptrBufferTransfer;
}
