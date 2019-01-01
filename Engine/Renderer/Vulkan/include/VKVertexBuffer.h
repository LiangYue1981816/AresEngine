#pragma once
#include "VKRenderer.h"


class CVKVertexBuffer : public CGfxVertexBuffer
{
	friend class CVKDevice;
	friend class CVKMesh;
	friend class CVKMeshDraw;


private:
	CVKVertexBuffer(CVKDevice *pDevice, uint32_t vertexFormat, uint32_t vertexBinding, size_t size, bool bDynamic);
	virtual ~CVKVertexBuffer(void);


public:
	bool BufferData(size_t offset, size_t size, const void *pBuffer);
	void Bind(VkCommandBuffer vkCommandBuffer, VkDeviceSize offset);

private:
	CVKBufferPtr GetBuffer(void) const;
	CVKBufferPtr GetBufferTransfer(void) const;


private:
	uint32_t m_binding;

private:
	VkDeviceSize m_transferSize;
	VkDeviceSize m_transferOffset;

private:
	CVKBufferPtr m_ptrBuffer;
	CVKBufferPtr m_ptrBufferTransfer;

private:
	CVKDevice *m_pDevice;
};
