#pragma once
#include "VKRenderer.h"


class CVKBuffer : public CGfxResource
{
	friend class CVKDevice;
	friend class CVKIndexBuffer;
	friend class CVKVertexBuffer;
	friend class CVKInstanceBuffer;
	friend class CVKIndirectBuffer;
	friend class CVKUniformBuffer;


private:
	CVKBuffer(CVKDevice *pDevice, VkBufferUsageFlags usage, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize size);
	virtual ~CVKBuffer(void);
	virtual void Release(void);


private:
	bool BufferData(size_t offset, size_t size, const void *pBuffer);


private:
	VkDeviceSize m_size;
	VkBufferUsageFlags m_usage;
	VkMemoryPropertyFlags m_memoryPropertyFlags;

private:
	VkBuffer m_vkBuffer;
	CVKMemory *m_pMemory;

private:
	CVKDevice *m_pDevice;
};

typedef CGfxResourcePtr<CVKBuffer> CVKBufferPtr;
