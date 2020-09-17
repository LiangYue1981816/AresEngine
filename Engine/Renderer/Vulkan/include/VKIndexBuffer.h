#pragma once
#include "VKRenderer.h"


class CVKIndexBuffer : public CGfxIndexBuffer
{
public:
	CVKIndexBuffer(CVKDevice* pDevice, GfxIndexType type, size_t size, bool bDynamic);
	virtual ~CVKIndexBuffer(void);
	virtual void Release(void);


public:
	VkBuffer GetBuffer(void) const;
	VkBufferUsageFlags GetBufferUsageFlags(void) const;
	VkDeviceSize GetBufferSize(void) const;

public:
	GfxIndexType GetType(void) const;
	uint32_t GetCount(void) const;

public:
	bool BufferData(size_t offset, size_t size, const void* data);


private:
	GfxIndexType m_type;
	uint32_t m_count;

private:
	CVKBuffer* m_pBuffer;

private:
	CVKDevice* m_pDevice;
};
