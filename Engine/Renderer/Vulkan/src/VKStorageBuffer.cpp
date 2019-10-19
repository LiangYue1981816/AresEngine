#include "VKRenderer.h"


CVKStorageBuffer::CVKStorageBuffer(CVKDevice* pDevice, CVKStorageBufferManager* pManager, size_t size)
	: CGfxStorageBuffer(size)
	, m_pDevice(pDevice)
	, m_pManager(pManager)
	, m_pBuffer(nullptr)

	, m_size(0)
	, m_offset(0)
{
	m_size = ALIGN_BYTE(size, m_pDevice->GetPhysicalDeviceLimits().nonCoherentAtomSize);
	m_pBuffer = new CVKBuffer(m_pDevice, m_size, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
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
	return m_size;
}

uint32_t CVKStorageBuffer::GetOffset(void) const
{
	return m_offset;
}

bool CVKStorageBuffer::BufferData(size_t offset, size_t size, const void* data)
{
	return m_pBuffer->BufferData(m_offset + offset, size, data);
}
