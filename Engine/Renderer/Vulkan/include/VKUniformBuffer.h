#pragma once
#include "VKRenderer.h"


class CVKUniformBuffer : public CGfxUniformBuffer
{
private:
	CVKUniformBuffer(CVKDevice *pDevice, size_t size);
	virtual ~CVKUniformBuffer(void);
	virtual void Release(void);


public:
	uint32_t GetSize(void) const;

public:
	bool BufferData(size_t offset, size_t size, const void *pBuffer);


private:
	uint32_t m_size;

private:
	CVKBufferPtr m_ptrBuffer;

private:
	CVKDevice *m_pDevice;
};
