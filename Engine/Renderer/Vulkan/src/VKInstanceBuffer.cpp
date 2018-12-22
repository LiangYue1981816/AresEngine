#include "VKRenderer.h"


CVKInstanceBuffer::CVKInstanceBuffer(CVKDevice *pDevice, uint32_t instanceFormat, uint32_t instanceBinding)
	: CVKBufferBase(pDevice, INSTANCE_BUFFER_SIZE, VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
	, CGfxInstanceBuffer(instanceFormat, instanceBinding)

	, m_hash(INVALID_HASHVALUE)
{

}

CVKInstanceBuffer::~CVKInstanceBuffer(void)
{

}

bool CVKInstanceBuffer::BufferData(size_t offset, size_t size, const void *pBuffer)
{
	uint32_t hash = HashValue((uint8_t *)pBuffer, size, 2);

	if (m_hash != hash) {
		m_hash  = hash;
		m_count = size / GetInstanceStride(m_format);

		if (m_size < size) {
			CGfxProfiler::DecInstanceBufferSize(m_size);
			{
				m_size = INSTANCE_BUFFER_SIZE;
				while (m_size < size) m_size <<= 1;

				Destroy();
				Create(m_size, m_usage, m_flags);
			}
			CGfxProfiler::IncInstanceBufferSize(m_size);
		}

		return CVKBufferBase::BufferData(offset, size, pBuffer);
	}

	return true;
}

void CVKInstanceBuffer::Bind(void *pParam)
{

}
