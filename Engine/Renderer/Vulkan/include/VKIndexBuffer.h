#pragma once
#include "VKRenderer.h"


class CVKIndexBuffer : public CGfxIndexBuffer
{
	friend class CVKDevice;
	friend class CVKMesh;
	friend class CVKMeshDraw;


private:
	CVKIndexBuffer(CVKDevice *pDevice, GfxIndexType type, size_t size, bool bDynamic);
	virtual ~CVKIndexBuffer(void);


public:
	bool BufferData(size_t offset, size_t size, const void *pBuffer);
	void Bind(VkCommandBuffer vkCommandBuffer, VkDeviceSize offset);


private:
	VkDeviceSize m_transferSize;
	VkDeviceSize m_transferOffset;

private:
	CVKBufferPtr m_ptrBuffer;
	CVKBufferPtr m_ptrBufferTransfer;

private:
	CVKDevice *m_pDevice;
};
