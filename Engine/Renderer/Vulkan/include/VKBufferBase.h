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
	bool Create(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags memoryPropertyFlags);
	void Destroy(void);

private:
	bool BufferData(VkDeviceSize offset, VkDeviceSize size, const void *pBuffer) const;


private:
	VkBuffer m_vkBuffer;
	VkBufferUsageFlags m_usage;
	VkMemoryPropertyFlags m_flags;

private:
	CVKMemory *m_pMemory;

private:
	CVKDevice *m_pDevice;
};
