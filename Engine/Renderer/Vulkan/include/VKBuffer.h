#pragma once
#include "VKRenderer.h"


class CVKBuffer
{
	friend class CVKDevice;


private:
	CVKBuffer(CVKDevice *pDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags memoryPropertyFlags);
	virtual ~CVKBuffer(void);


protected:
	bool BufferData(VkDeviceSize offset, VkDeviceSize size, const void *pBuffer) const;
	bool CopyData(VkDeviceSize offset, VkDeviceSize size, const void *pBuffer) const;
	bool TransferData(VkDeviceSize offset, VkDeviceSize size, const void *pBuffer) const;


private:
	VkBuffer m_vkBuffer;
	VkBufferUsageFlags m_usage;

private:
	CVKMemory *m_pMemory;

private:
	CVKDevice *m_pDevice;
};
