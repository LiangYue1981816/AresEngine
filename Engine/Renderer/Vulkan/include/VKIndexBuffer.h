#pragma once
#include "VKRenderer.h"


class CVKIndexBuffer : public CGfxIndexBuffer
{
private:
	CVKIndexBuffer(CVKDevice* pDevice, GfxIndexType type, size_t size, bool bDynamic);
	virtual ~CVKIndexBuffer(void);


public:
	GfxIndexType GetIndexType(void) const;
	uint32_t GetIndexCount(void) const;
	uint32_t GetSize(void) const;

public:
	bool BufferData(size_t offset, size_t size, const void* data);

public:
	void Bind(VkCommandBuffer vkCommandBuffer) const;


private:
	GfxIndexType m_type;
	uint32_t m_size;

private:
	CVKBufferPtr m_ptrBuffer;

private:
	CVKDevice* m_pDevice;
};
/*
class CVKIndexBuffer : public CGfxIndexBuffer
{
	friend class CVKMesh;
	friend class CVKMeshDraw;


private:
	CVKIndexBuffer(CVKDevice* pDevice, GfxIndexType type, size_t size, bool bDynamic);
	virtual ~CVKIndexBuffer(void);


public:
	GfxIndexType GetIndexType(void) const;
	uint32_t GetIndexCount(void) const;
	uint32_t GetSize(void) const;

public:
	bool BufferData(size_t offset, size_t size, const void* pBuffer);

private:
	void Bind(VkCommandBuffer vkCommandBuffer, VkDeviceSize offset) const;

private:
	CVKBufferPtr TransferBuffer(VkCommandBuffer vkCommandBuffer);


private:
	GfxIndexType m_type;
	uint32_t m_size;

private:
	VkDeviceSize m_transferOffset;
	eastl::vector<uint8_t> m_transferBuffer;

private:
	CVKBufferPtr m_ptrBuffer;

private:
	CVKDevice* m_pDevice;
};
*/