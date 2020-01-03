#pragma once
#include "VKRenderer.h"


class CVKIndexBuffer : public CGfxIndexBuffer
{
public:
	CVKIndexBuffer(CVKDevice* pDevice, GfxIndexType type, size_t size, bool bDynamic);
	virtual ~CVKIndexBuffer(void);
	virtual void Release(void);


public:
	GfxIndexType GetIndexType(void) const;
	uint32_t GetIndexCount(void) const;
	uint32_t GetSize(void) const;
	uint32_t GetOffset(void) const;

public:
	bool BufferData(size_t offset, size_t size, const void* data);

public:
	void Bind(VkCommandBuffer vkCommandBuffer) const;


private:
	GfxIndexType m_type;
	uint32_t m_size;
	uint32_t m_offset;

private:
	CVKBuffer* m_pBuffer;

private:
	CVKDevice* m_pDevice;
};


class CVKMultiIndexBuffer : public CGfxMultiIndexBuffer
{
public:
	CVKMultiIndexBuffer(CVKDevice* pDevice, GfxIndexType type, size_t size, bool bDynamic, int count);
	virtual ~CVKMultiIndexBuffer(void);
	virtual void Release(void);


public:
	void SetBufferIndex(int index);

public:
	GfxIndexType GetIndexType(void) const;
	uint32_t GetIndexCount(void) const;
	uint32_t GetSize(void) const;
	uint32_t GetOffset(void) const;

public:
	bool BufferData(size_t offset, size_t size, const void* data);

public:
	void Bind(VkCommandBuffer vkCommandBuffer) const;


private:
	int m_index;
	eastl::vector<CVKIndexBuffer*> m_pBuffers;
};
