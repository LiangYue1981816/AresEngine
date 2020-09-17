#pragma once
#include "VKRenderer.h"


class CVKTransferBuffer : public CGfxTransferBuffer
{
public:
	CVKTransferBuffer(CVKDevice* pDevice, size_t size);
	virtual ~CVKTransferBuffer(void);
	virtual void Release(void);


public:
	VkBuffer GetBuffer(void) const;
	VkBufferUsageFlags GetBufferUsageFlags(void) const;
	VkDeviceSize GetBufferSize(void) const;
	VkDeviceSize GetMemorySize(void) const;

public:
	bool BufferData(size_t offset, size_t size, const void* data);


private:
	CVKBuffer* m_pBuffer;

private:
	CVKDevice* m_pDevice;
};
