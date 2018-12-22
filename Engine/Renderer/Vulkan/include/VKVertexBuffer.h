#pragma once
#include "VKRenderer.h"


class CVKVertexBuffer : public CVKBuffer, public CGfxVertexBuffer
{
	friend class CVKDevice;


private:
	CVKVertexBuffer(CVKDevice *pDevice, uint32_t vertexFormat, uint32_t vertexBinding, size_t size, bool bDynamic);
	virtual ~CVKVertexBuffer(void);


public:
	bool BufferData(size_t offset, size_t size, const void *pBuffer);
	void Bind(void *pParam);


private:
	VkBuffer m_vkBuffer;

private:
	CVKMemory *m_pMemory;

private:
	CVKDevice *m_pDevice;
};
