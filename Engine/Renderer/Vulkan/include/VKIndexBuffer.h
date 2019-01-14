#pragma once
#include "VKRenderer.h"


class CVKIndexBuffer : public CGfxIndexBuffer
{
	friend class CVKMesh;
	friend class CVKMeshDraw;


private:
	CVKIndexBuffer(CVKDevice *pDevice, GfxIndexType type, size_t size, bool bDynamic);
	virtual ~CVKIndexBuffer(void);


public:
	GfxIndexType GetIndexType(void) const;
	uint32_t GetIndexCount(void) const;
	uint32_t GetSize(void) const;

public:
	bool BufferData(size_t offset, size_t size, const void *pBuffer);
	void Bind(VkCommandBuffer vkCommandBuffer, VkDeviceSize offset, CVKBufferPtr &ptrBufferTransfer);


private:
	GfxIndexType m_type;
	uint32_t m_size;

private:
	VkDeviceSize m_transferOffset;
	eastl::vector<uint8_t> m_transferBuffer;

private:
	CVKBufferPtr m_ptrBuffer;

private:
	CVKDevice *m_pDevice;
};
