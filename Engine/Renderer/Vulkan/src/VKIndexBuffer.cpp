#include "VKRenderer.h"


CVKIndexBuffer::CVKIndexBuffer(CVKDevice* pDevice, GfxIndexType type, size_t size, bool bDynamic)
	: CGfxIndexBuffer(type, size, bDynamic)
	, m_pDevice(pDevice)
	, m_pBuffer(nullptr)

	, m_type(type)
	, m_size(size)
	, m_offset(0)
{
	if (bDynamic) {
		m_pBuffer = new CVKBuffer(pDevice, CGfxSwapChain::SWAPCHAIN_FRAME_COUNT * size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
		CGfxProfiler::IncIndexBufferSize(m_pBuffer->GetSize());
	}
	else {
		m_pBuffer = new CVKBuffer(pDevice, size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		CGfxProfiler::IncIndexBufferSize(m_pBuffer->GetSize());
	}
}

CVKIndexBuffer::~CVKIndexBuffer(void)
{
	CGfxProfiler::DecIndexBufferSize(m_pBuffer->GetSize());
	delete m_pBuffer;
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

uint32_t CVKIndexBuffer::GetOffset(void) const
{
	return m_offset;
}

bool CVKIndexBuffer::BufferData(size_t offset, size_t size, const void* data)
{
	m_offset = m_pBuffer->IsHostVisible() ? VKRenderer()->GetSwapChain()->GetFrameIndex() * m_size : 0;
	return m_pBuffer->BufferData(m_offset + offset, size, data);
}

void CVKIndexBuffer::Bind(VkCommandBuffer vkCommandBuffer) const
{
	ASSERT(vkCommandBuffer);
	vkCmdBindIndexBuffer(vkCommandBuffer, m_pBuffer->GetBuffer(), m_offset, CVKHelper::TranslateIndexType(m_type));
}
