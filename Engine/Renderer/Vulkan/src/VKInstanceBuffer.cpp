#include "VKRenderer.h"


static const int INSTANCE_BUFFER_SIZE = 64;

CVKInstanceBuffer::CVKInstanceBuffer(CVKDevice* pDevice, uint32_t instanceFormat, int instanceBinding)
	: CGfxInstanceBuffer(instanceFormat, instanceBinding)
	, m_pDevice(pDevice)
	, m_pBuffer(nullptr)

	, m_binding(instanceBinding)
	, m_format(instanceFormat)
	, m_count(0)
{
	m_pBuffer = new CVKBuffer(pDevice, INSTANCE_BUFFER_SIZE, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
	CGfxProfiler::IncInstanceBufferSize(m_pBuffer->GetSize());
}

CVKInstanceBuffer::~CVKInstanceBuffer(void)
{
	CGfxProfiler::DecInstanceBufferSize(m_pBuffer->GetSize());
	delete m_pBuffer;
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
		CGfxProfiler::DecInstanceBufferSize(m_pBuffer->GetSize());
		{
			size_t newSize = INSTANCE_BUFFER_SIZE;
			while (newSize < size) newSize <<= 1;

			delete m_pBuffer;
			m_pBuffer = new CVKBuffer(m_pDevice, newSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
		}
		CGfxProfiler::IncInstanceBufferSize(m_pBuffer->GetSize());
	}

	return m_pBuffer->BufferData(0, size, data);
}

void CVKInstanceBuffer::Bind(VkCommandBuffer vkCommandBuffer) const
{
	ASSERT(vkCommandBuffer);

	const VkBuffer vkBuffer = m_pBuffer->GetBuffer();
	const VkDeviceSize vkOffset = 0;
	vkCmdBindVertexBuffers(vkCommandBuffer, m_binding, 1, &vkBuffer, &vkOffset);
}
