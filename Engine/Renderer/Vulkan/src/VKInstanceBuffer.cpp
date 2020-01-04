#include "VKRenderer.h"


static const int INSTANCE_BUFFER_SIZE = 64;

CVKInstanceBuffer::CVKInstanceBuffer(CVKDevice* pDevice, uint32_t instanceFormat, int instanceBinding)
	: CGfxInstanceBuffer(instanceFormat, instanceBinding)
	, m_pDevice(pDevice)
	, m_pBuffer(nullptr)

	, m_binding(instanceBinding)
	, m_format(instanceFormat)
	, m_count(0)
	, m_size(0)
{
	m_size = INSTANCE_BUFFER_SIZE;
	m_size = ALIGN_BYTE(m_size, m_pDevice->GetPhysicalDeviceLimits().nonCoherentAtomSize);

	m_pBuffer = new CVKBuffer(m_pDevice, m_size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
	CGfxProfiler::IncInstanceBufferSize(m_pBuffer->GetMemorySize());
}

CVKInstanceBuffer::~CVKInstanceBuffer(void)
{
	CGfxProfiler::DecInstanceBufferSize(m_pBuffer->GetMemorySize());
	delete m_pBuffer;
}

void CVKInstanceBuffer::Release(void)
{
	delete this;
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

			m_size = ALIGN_BYTE(newSize, m_pDevice->GetPhysicalDeviceLimits().nonCoherentAtomSize);
			m_pBuffer = new CVKBuffer(m_pDevice, m_size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
		}
		CGfxProfiler::IncInstanceBufferSize(m_pBuffer->GetMemorySize());
	}

	return m_pBuffer->BufferData(0, size, data);
}

void CVKInstanceBuffer::Bind(VkCommandBuffer vkCommandBuffer) const
{
	ASSERT(vkCommandBuffer);
	vkCmdBindVertexBuffer(vkCommandBuffer, m_binding, m_pBuffer->GetBuffer(), 0);
}


CVKMultiInstanceBuffer::CVKMultiInstanceBuffer(CVKDevice* pDevice, uint32_t instanceFormat, int instanceBinding, int count)
	: CGfxMultiInstanceBuffer(instanceFormat, instanceBinding, count)
	, m_index(0)
	, m_pBuffers(std::max(1, count))
{
	for (int index = 0; index < m_pBuffers.size(); index++) {
		m_pBuffers[index] = new CVKInstanceBuffer(pDevice, instanceFormat, instanceBinding);
	}
}

CVKMultiInstanceBuffer::~CVKMultiInstanceBuffer(void)
{
	for (auto& itBuffer : m_pBuffers) {
		delete itBuffer;
	}
}

void CVKMultiInstanceBuffer::Release(void)
{
	delete this;
}

bool CVKMultiInstanceBuffer::SetIndex(int index)
{
	if (index >= 0 && index < m_pBuffers.size()) {
		m_index = index;
		return true;
	}
	else {
		return false;
	}
}

CVKInstanceBuffer* CVKMultiInstanceBuffer::GetBuffer(void) const
{
	return m_pBuffers[m_index];
}

CVKInstanceBuffer* CVKMultiInstanceBuffer::GetBuffer(int index) const
{
	if (index >= 0 && index < m_pBuffers.size()) {
		return m_pBuffers[index];
	}
	else {
		return nullptr;
	}
}

uint32_t CVKMultiInstanceBuffer::GetInstanceBinding(void) const
{
	return m_pBuffers[m_index]->GetInstanceBinding();
}

uint32_t CVKMultiInstanceBuffer::GetInstanceFormat(void) const
{
	return m_pBuffers[m_index]->GetInstanceFormat();
}

uint32_t CVKMultiInstanceBuffer::GetInstanceCount(void) const
{
	return m_pBuffers[m_index]->GetInstanceCount();
}

uint32_t CVKMultiInstanceBuffer::GetSize(void) const
{
	return m_pBuffers[m_index]->GetSize();
}

bool CVKMultiInstanceBuffer::BufferData(size_t size, const void* data)
{
	return m_pBuffers[m_index]->BufferData(size, data);
}

void CVKMultiInstanceBuffer::Bind(VkCommandBuffer vkCommandBuffer) const
{
	m_pBuffers[m_index]->Bind(vkCommandBuffer);
}
