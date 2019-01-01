#pragma once
#include "VKRenderer.h"


class CVKUniformBuffer : public CGfxUniformBuffer
{
	friend class CVKDevice;


private:
	CVKUniformBuffer(CVKDevice *pDevice, size_t size);
	virtual ~CVKUniformBuffer(void);
	virtual void Release(void);


public:
	bool BufferData(size_t offset, size_t size, const void *pBuffer);


private:
	CVKBufferPtr m_ptrBuffer;

private:
	CVKDevice *m_pDevice;
};
