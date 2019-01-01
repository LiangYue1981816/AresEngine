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
	void Bind(VkCommandBuffer vkCommandBuffer, VkDeviceSize offset, CVKBufferPtr &ptrBufferTransfer);


private:
	VkDeviceSize m_transferOffset;
	eastl::vector<uint8_t> m_transferBuffer;

private:
	uint32_t m_binding;
	CVKBufferPtr m_ptrBuffer;

private:
	CVKDevice *m_pDevice;
};
