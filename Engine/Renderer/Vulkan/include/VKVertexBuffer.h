#pragma once
#include "VKRenderer.h"


class CVKVertexBuffer : public CGfxVertexBuffer
{
	friend class CVKDevice;


private:
	CVKVertexBuffer(CVKDevice *pDevice, uint32_t vertexFormat, uint32_t vertexBinding, size_t size, bool bDynamic);
	virtual ~CVKVertexBuffer(void);


public:
	bool BufferData(size_t offset, size_t size, const void *pBuffer);
	void Bind(void *pParam);


private:
	uint32_t m_binding;
	CVKBuffer *m_pBuffer;
	CVKBuffer *m_pBufferTransfer;

private:
	bool m_bNeedTransfer;
	VkDeviceSize m_transferOffset;
	VkDeviceSize m_transferSize;

private:
	CVKDevice *m_pDevice;
};
