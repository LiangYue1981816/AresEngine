#pragma once
#include "VKRenderer.h"


class CVKVertexBuffer : public CGfxVertexBuffer
{
	friend class CVKDevice;


private:
	CVKVertexBuffer(CVKDevice *pDevice, uint32_t vertexFormat, uint32_t vertexBinding, size_t size, bool bDynamic);
	virtual ~CVKVertexBuffer(void);


public:
	bool BufferData(size_t offset, size_t size, const void *pBuffer);
	void Bind(void *pParam);

private:
	bool CopyData(size_t offset, size_t size, const void *pBuffer);
	bool TransferData(size_t offset, size_t size, const void *pBuffer);


private:
	VkBuffer m_vkBuffer;
	VkBufferUsageFlags m_usage;
	VkMemoryPropertyFlags m_memoryPropertyFlags;

private:
	CVKMemory *m_pMemory;

private:
	CVKDevice *m_pDevice;
};
