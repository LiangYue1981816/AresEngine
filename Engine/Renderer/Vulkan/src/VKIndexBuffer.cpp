#include "VKRenderer.h"


CVKIndexBuffer::CVKIndexBuffer(CVKDevice* pDevice, GfxIndexType type, size_t size, bool bDynamic)
	: CGfxIndexBuffer(type, size, bDynamic)
	, m_pDevice(nullptr)

	, m_type(type)
	, m_size(size)
{
	if (bDynamic) {
		m_ptrBuffer = CVKBufferPtr(new CVKBuffer(pDevice, CGfxSwapChain::SWAPCHAIN_FRAME_COUNT * size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));
		CGfxProfiler::IncIndexBufferSize(m_ptrBuffer->GetSize());
	}
	else {
		m_ptrBuffer = CVKBufferPtr(new CVKBuffer(pDevice, size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT));
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

bool CVKIndexBuffer::BufferData(size_t offset, size_t size, const void* data)
{
	return true;
}

void CVKIndexBuffer::Bind(VkCommandBuffer vkCommandBuffer) const
{

}
