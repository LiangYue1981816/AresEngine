#pragma once
#include "VKRenderer.h"


class CVKBufferBase
{
	friend class CVKDevice;
	friend class CVKIndexBuffer;
	friend class CVKVertexBuffer;
	friend class CVKInstanceBuffer;


private:
	CVKBufferBase(CVKDevice *pDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags memoryPropertyFlags);
	virtual ~CVKBufferBase(void);


private:
	bool BufferData(VkDeviceSize offset, VkDeviceSize size, const void *pBuffer) const;


private:
	VkBuffer m_vkBuffer;
	CVKMemory *m_pMemory;

private:
	CVKDevice *m_pDevice;
};
