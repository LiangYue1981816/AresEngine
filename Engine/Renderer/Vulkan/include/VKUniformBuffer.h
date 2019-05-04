#pragma once
#include "VKRenderer.h"


class CVKUniformBuffer : public CGfxUniformBuffer
{
private:
	CVKUniformBuffer(CVKDevice* pDevice, size_t size);
	virtual ~CVKUniformBuffer(void);


public:
	VkBuffer GetBuffer(void) const;

public:
	uint32_t GetSize(void) const;
	uint32_t GetOffset(void) const;

public:
	bool BufferData(size_t offset, size_t size, const void* data);


private:
	uint32_t m_size;
	uint32_t m_offset;

private:
	CVKBufferPtr m_ptrBuffer;

private:
	CVKDevice* m_pDevice;
};
