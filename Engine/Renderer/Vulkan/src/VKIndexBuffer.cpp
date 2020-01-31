#include "VKRenderer.h"


CVKIndexBuffer::CVKIndexBuffer(CVKDevice* pDevice, GfxIndexType type, size_t size, bool bDynamic)
	: CGfxIndexBuffer(type, size, bDynamic)
	, m_pDevice(pDevice)
	, m_pBuffer(nullptr)

	, m_type(type)
{
	size = ALIGN_BYTE(size, m_pDevice->GetPhysicalDeviceLimits().nonCoherentAtomSize);

	if (bDynamic) {
		m_pBuffer = new CVKBuffer(m_pDevice, size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
		CGfxProfiler::IncIndexBufferSize(m_pBuffer->GetMemorySize());
	}
	else {
		m_pBuffer = new CVKBuffer(m_pDevice, size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		CGfxProfiler::IncIndexBufferSize(m_pBuffer->GetMemorySize());
	}
}

CVKIndexBuffer::~CVKIndexBuffer(void)
{
	CGfxProfiler::DecIndexBufferSize(m_pBuffer->GetMemorySize());
	delete m_pBuffer;
}

void CVKIndexBuffer::Release(void)
{

}

GfxIndexType CVKIndexBuffer::GetIndexType(void) const
{
	return m_type;
}

uint32_t CVKIndexBuffer::GetIndexCount(void) const
{
	switch (m_type) {
	case GFX_INDEX_UNSIGNED_SHORT: return m_pBuffer->GetSize() / 2;
	case GFX_INDEX_UNSIGNED_INT:   return m_pBuffer->GetSize() / 4;
	default:                       return 0;
	}
}

uint32_t CVKIndexBuffer::GetSize(void) const
{
	return m_pBuffer->GetSize();
}

bool CVKIndexBuffer::BufferData(size_t offset, size_t size, const void* data)
{
	return m_pBuffer->BufferData(offset, size, data);
}

void CVKIndexBuffer::Bind(VkCommandBuffer vkCommandBuffer) const
{
	ASSERT(vkCommandBuffer);
	vkCmdBindIndexBuffer(vkCommandBuffer, m_pBuffer->GetBuffer(), 0, CVKHelper::TranslateIndexType(m_type));
}


CVKMultiIndexBuffer::CVKMultiIndexBuffer(CVKDevice* pDevice, GfxIndexType type, size_t size, bool bDynamic, int count)
	: CGfxMultiIndexBuffer(type, size, bDynamic, count)
	, m_pBuffers(std::max(1, count))
{
	for (int index = 0; index < m_pBuffers.size(); index++) {
		m_pBuffers[index] = new CVKIndexBuffer(pDevice, type, size, bDynamic);
	}
}

CVKMultiIndexBuffer::~CVKMultiIndexBuffer(void)
{
	for (auto& itBuffer : m_pBuffers) {
		delete itBuffer;
	}
}

void CVKMultiIndexBuffer::Release(void)
{

}

CGfxIndexBuffer* CVKMultiIndexBuffer::GetBuffer(int index) const
{
	if (index >= 0 && index < m_pBuffers.size()) {
		return m_pBuffers[index];
	}
	else {
		return nullptr;
	}
}
