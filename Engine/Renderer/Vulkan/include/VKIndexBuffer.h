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
	CVKBuffer *m_pBuffer;
	CVKBuffer *m_pBufferTransfer;

private:
	bool m_bNeedTransfer;
	VkDeviceSize m_transferOffset;
	VkDeviceSize m_transferSize;

private:
	CVKDevice *m_pDevice;
};
