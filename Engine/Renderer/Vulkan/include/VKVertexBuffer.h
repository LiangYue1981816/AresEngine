#pragma once
#include "VKRenderer.h"


class CVKVertexBuffer : public CGfxVertexBuffer
{
	friend class CVKMesh;
	friend class CVKMeshDraw;


private:
	CVKVertexBuffer(CVKDevice *pDevice, uint32_t vertexFormat, uint32_t vertexBinding, size_t size, bool bDynamic);
	virtual ~CVKVertexBuffer(void);


public:
	uint32_t GetVertexFormat(void) const;
	uint32_t GetVertexCount(void) const;
	uint32_t GetSize(void) const;

public:
	bool BufferData(size_t offset, size_t size, const void *pBuffer);

private:
	void Bind(VkCommandBuffer vkCommandBuffer, VkDeviceSize offset) const;

private:
	CVKBufferPtr BufferTransfer(VkCommandBuffer vkCommandBuffer);


private:
	uint32_t m_format;
	uint32_t m_count;
	uint32_t m_size;

private:
	VkDeviceSize m_transferOffset;
	eastl::vector<uint8_t> m_transferBuffer;

private:
	uint32_t m_binding;
	CVKBufferPtr m_ptrBuffer;

private:
	CVKDevice *m_pDevice;
};
