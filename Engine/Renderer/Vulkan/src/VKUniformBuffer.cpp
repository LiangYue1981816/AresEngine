#include "VKRenderer.h"


CVKUniformBuffer::CVKUniformBuffer(CVKDevice* pDevice, CVKUniformBufferManager* pManager, size_t size)
	: CGfxUniformBuffer(size)
	, m_pDevice(pDevice)
	, m_pManager(pManager)
	, m_pBuffer(nullptr)
{
	size = ALIGN_BYTE(size, m_pDevice->GetPhysicalDeviceLimits().nonCoherentAtomSize);
	size = ALIGN_BYTE(size, m_pDevice->GetPhysicalDeviceLimits().minUniformBufferOffsetAlignment);

	m_pBuffer = new CVKBuffer(m_pDevice, size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
	CGfxProfiler::IncUniformBufferSize(m_pBuffer->GetMemorySize());
}

CVKUniformBuffer::~CVKUniformBuffer(void)
{
	CGfxProfiler::DecUniformBufferSize(m_pBuffer->GetMemorySize());
	delete m_pBuffer;
}

void CVKUniformBuffer::Release(void)
{
	m_pManager->Destroy(this);
}

VkBuffer CVKUniformBuffer::GetBuffer(void) const
{
	return m_pBuffer->GetBuffer();
}

VkBufferUsageFlags CVKUniformBuffer::GetBufferUsageFlags(void) const
{
	return m_pBuffer->GetBufferUsageFlags();
}

uint32_t CVKUniformBuffer::GetSize(void) const
{
	return m_pBuffer->GetSize();
}

bool CVKUniformBuffer::BufferData(size_t offset, size_t size, const void* data)
{
	return m_pBuffer->BufferData(offset, size, data);
}
