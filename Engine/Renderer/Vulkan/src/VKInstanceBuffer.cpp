#include "VKRenderer.h"


static const int INSTANCE_BUFFER_SIZE = 64;

CVKInstanceBuffer::CVKInstanceBuffer(CVKDevice* pDevice, CVKInstanceBufferManager* pManager, uint32_t instanceFormat, int instanceBinding)
	: CGfxInstanceBuffer(instanceFormat, instanceBinding)
	, m_pDevice(pDevice)
	, m_pManager(pManager)
	, m_pBuffer(nullptr)

	, m_binding(instanceBinding)
	, m_format(instanceFormat)
	, m_count(0)
{
	size_t size;
	size = INSTANCE_BUFFER_SIZE;
	size = ALIGN_BYTE(size, m_pDevice->GetPhysicalDeviceLimits().nonCoherentAtomSize);

	m_pBuffer = new CVKBuffer(m_pDevice, size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
	CGfxProfiler::IncInstanceBufferSize(m_pBuffer->GetMemorySize());
}

CVKInstanceBuffer::~CVKInstanceBuffer(void)
{
	CGfxProfiler::DecInstanceBufferSize(m_pBuffer->GetMemorySize());
	delete m_pBuffer;
}

void CVKInstanceBuffer::Release(void)
{
	if (m_pManager) {
		m_pManager->Destroy(this);
	}
}

VkBuffer CVKInstanceBuffer::GetBuffer(void) const
{
	return m_pBuffer->GetBuffer();
}

VkBufferUsageFlags CVKInstanceBuffer::GetBufferUsageFlags(void) const
{
	return m_pBuffer->GetBufferUsageFlags();
}

uint32_t CVKInstanceBuffer::GetInstanceBinding(void) const
{
	return m_binding;
}

uint32_t CVKInstanceBuffer::GetInstanceFormat(void) const
{
	return m_format;
}

uint32_t CVKInstanceBuffer::GetInstanceCount(void) const
{
	return m_count;
}

uint32_t CVKInstanceBuffer::GetSize(void) const
{
	return m_pBuffer->GetSize();
}

bool CVKInstanceBuffer::BufferData(size_t size, const void* data)
{
	m_count = size / GetInstanceStride(m_format);

	if (m_pBuffer->GetSize() < size) {
		CGfxProfiler::DecInstanceBufferSize(m_pBuffer->GetMemorySize());
		{
			delete m_pBuffer;

			size_t newSize = INSTANCE_BUFFER_SIZE;
			while (newSize < size) newSize <<= 1;

			newSize = ALIGN_BYTE(newSize, m_pDevice->GetPhysicalDeviceLimits().nonCoherentAtomSize);
			m_pBuffer = new CVKBuffer(m_pDevice, newSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
		}
		CGfxProfiler::IncInstanceBufferSize(m_pBuffer->GetMemorySize());
	}

	return m_pBuffer->BufferData(0, size, data);
}

void CVKInstanceBuffer::Bind(VkCommandBuffer vkCommandBuffer, int offset) const
{
	ASSERT(vkCommandBuffer);
	vkCmdBindVertexBuffer(vkCommandBuffer, m_binding, m_pBuffer->GetBuffer(), offset);
}
