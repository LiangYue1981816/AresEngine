#include "VKRenderer.h"


CVKStorageBuffer::CVKStorageBuffer(CVKDevice* pDevice, CVKStorageBufferManager* pManager, size_t size)
	: CGfxStorageBuffer(size)
	, m_pDevice(pDevice)
	, m_pManager(pManager)
	, m_pBuffer(nullptr)
{
	size = ALIGN_BYTE(size, m_pDevice->GetPhysicalDeviceLimits().nonCoherentAtomSize);
	size = ALIGN_BYTE(size, m_pDevice->GetPhysicalDeviceLimits().minStorageBufferOffsetAlignment);

	m_pBuffer = new CVKBuffer(m_pDevice, size, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
	CGfxProfiler::IncStorageBufferSize(m_pBuffer->GetMemorySize());
}

CVKStorageBuffer::~CVKStorageBuffer(void)
{
	CGfxProfiler::DecStorageBufferSize(m_pBuffer->GetMemorySize());
	delete m_pBuffer;
}

void CVKStorageBuffer::Release(void)
{
	m_pManager->Destroy(this);
}

VkBuffer CVKStorageBuffer::GetBuffer(void) const
{
	return m_pBuffer->GetBuffer();
}

uint32_t CVKStorageBuffer::GetSize(void) const
{
	return m_pBuffer->GetSize();
}

bool CVKStorageBuffer::BufferData(size_t offset, size_t size, const void* data)
{
	return m_pBuffer->BufferData(offset, size, data);
}

bool CVKStorageBuffer::PipelineBarrier(VkCommandBuffer vkCommandBuffer, VkAccessFlags srcAccessFlags, VkAccessFlags dstAccessFlags, VkDeviceSize offset, VkDeviceSize size)
{
	return m_pBuffer->PipelineBarrier(vkCommandBuffer, srcAccessFlags, dstAccessFlags, offset, size);
}

bool CVKStorageBuffer::PipelineBarrier(VkCommandBuffer vkCommandBuffer, VkAccessFlags srcAccessFlags, VkAccessFlags dstAccessFlags, VkPipelineStageFlags srcPipelineStageFlags, VkPipelineStageFlags dstPipelineStageFlags, VkDeviceSize offset, VkDeviceSize size)
{
	return m_pBuffer->PipelineBarrier(vkCommandBuffer, srcAccessFlags, dstAccessFlags, srcPipelineStageFlags, dstPipelineStageFlags, offset, size);
}
