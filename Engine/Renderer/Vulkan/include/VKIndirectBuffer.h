#pragma once
#include "VKRenderer.h"


class CVKIndirectBuffer : public CGfxIndirectBuffer
{
public:
	CVKIndirectBuffer(CVKDevice* pDevice);
	virtual ~CVKIndirectBuffer(void);
	virtual void Release(void);


public:
	VkBuffer GetBuffer(void) const;
	VkBufferUsageFlags GetBufferUsageFlags(void) const;
	VkDeviceSize GetBufferSize(void) const;

public:
	uint32_t GetOffset(int indexDraw) const;
	uint32_t GetCount(void) const;

public:
	bool BufferData(uint32_t count, const DrawCommand* data);


private:
	uint32_t m_count;

private:
	CVKBuffer* m_pBuffer;

private:
	CVKDevice* m_pDevice;
};
