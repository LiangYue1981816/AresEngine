#include "VKRenderer.h"


CVKVertexBuffer::CVKVertexBuffer(CVKDevice* pDevice, uint32_t vertexFormat, int vertexBinding, size_t size, bool bDynamic)
	: CGfxVertexBuffer(vertexFormat, vertexBinding, size, bDynamic)
	, m_pDevice(pDevice)
	, m_pBuffer(nullptr)

	, m_binding(vertexBinding)
	, m_format(vertexFormat)
	, m_count(size / GetVertexStride(vertexFormat))
	, m_size(0)
{
	m_size = size;
	m_size = ALIGN_BYTE(m_size, m_pDevice->GetPhysicalDeviceLimits().nonCoherentAtomSize);

	if (bDynamic) {
		m_pBuffer = new CVKBuffer(m_pDevice, m_size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
		CGfxProfiler::IncVertexBufferSize(m_pBuffer->GetMemorySize());
	}
	else {
		m_pBuffer = new CVKBuffer(m_pDevice, m_size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		CGfxProfiler::IncVertexBufferSize(m_pBuffer->GetMemorySize());
	}
}

CVKVertexBuffer::~CVKVertexBuffer(void)
{
	CGfxProfiler::DecVertexBufferSize(m_pBuffer->GetMemorySize());
	delete m_pBuffer;
}

void CVKVertexBuffer::Release(void)
{
	delete this;
}

uint32_t CVKVertexBuffer::GetVertexBinding(void) const
{
	return m_binding;
}

uint32_t CVKVertexBuffer::GetVertexFormat(void) const
{
	return m_format;
}

uint32_t CVKVertexBuffer::GetVertexCount(void) const
{
	return m_count;
}

uint32_t CVKVertexBuffer::GetSize(void) const
{
	return m_size;
}

bool CVKVertexBuffer::BufferData(size_t offset, size_t size, const void* data)
{
	return m_pBuffer->BufferData(offset, size, data);
}

void CVKVertexBuffer::Bind(VkCommandBuffer vkCommandBuffer) const
{
	ASSERT(vkCommandBuffer);
	vkCmdBindVertexBuffer(vkCommandBuffer, m_binding, m_pBuffer->GetBuffer(), 0);
}


CVKMultiVertexBuffer::CVKMultiVertexBuffer(CVKDevice* pDevice, uint32_t vertexFormat, int vertexBinding, size_t size, bool bDynamic, int count)
	: CGfxMultiVertexBuffer(vertexFormat, vertexBinding, size, bDynamic, count)
	, m_index(0)
	, m_pBuffers(std::max(1, count))
{
	for (int index = 0; index < m_pBuffers.size(); index++) {
		m_pBuffers[index] = new CVKVertexBuffer(pDevice, vertexFormat, vertexBinding, size, bDynamic);
	}
}

CVKMultiVertexBuffer::~CVKMultiVertexBuffer(void)
{
	for (auto& itBuffer : m_pBuffers) {
		delete itBuffer;
	}
}

void CVKMultiVertexBuffer::Release(void)
{
	delete this;
}

void CVKMultiVertexBuffer::SetBufferIndex(int index)
{
	m_index = index;
	m_index = std::min(m_index, (int)m_pBuffers.size() - 1);
	m_index = std::max(m_index, 0);
}

CVKVertexBuffer* CVKMultiVertexBuffer::GetBuffer(void) const
{
	return m_pBuffers[m_index];
}

uint32_t CVKMultiVertexBuffer::GetVertexBinding(void) const
{
	return m_pBuffers[m_index]->GetVertexBinding();
}

uint32_t CVKMultiVertexBuffer::GetVertexFormat(void) const
{
	return m_pBuffers[m_index]->GetVertexFormat();
}

uint32_t CVKMultiVertexBuffer::GetVertexCount(void) const
{
	return m_pBuffers[m_index]->GetVertexCount();
}

uint32_t CVKMultiVertexBuffer::GetSize(void) const
{
	return m_pBuffers[m_index]->GetSize();
}

bool CVKMultiVertexBuffer::BufferData(size_t offset, size_t size, const void* data)
{
	return m_pBuffers[m_index]->BufferData(offset, size, data);
}

void CVKMultiVertexBuffer::Bind(VkCommandBuffer vkCommandBuffer) const
{
	m_pBuffers[m_index]->Bind(vkCommandBuffer);
}
