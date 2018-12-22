#pragma once
#include "VKRenderer.h"


class CVKInstanceBuffer : public CVKBufferBase, public CGfxInstanceBuffer
{
	friend class CVKDevice;


private:
	CVKInstanceBuffer(CVKDevice *pDevice, uint32_t instanceFormat, uint32_t instanceBinding);
	virtual ~CVKInstanceBuffer(void);


public:
	bool BufferData(size_t offset, size_t size, const void *pBuffer);
	void Bind(void *pParam);


private:
	uint32_t m_hash;
};
