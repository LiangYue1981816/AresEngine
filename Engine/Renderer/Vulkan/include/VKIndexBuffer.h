#pragma once
#include "VKRenderer.h"


class CVKIndexBuffer : public CGfxIndexBuffer
{
	friend class CVKDevice;


private:
	CVKIndexBuffer(CVKDevice *pDevice, GfxIndexType type, size_t size, bool bDynamic);
	virtual ~CVKIndexBuffer(void);


public:
	bool BufferData(size_t offset, size_t size, const void *pBuffer);
	void Bind(void *pParam);


private:
	VkBuffer m_vkBuffer;
	VkBufferUsageFlags m_usage;
	VkMemoryPropertyFlags m_memoryPropertyFlags;

private:
	CVKDevice *m_pDevice;
};
