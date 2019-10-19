#pragma once
#include "VKRenderer.h"


class CVKStorageBuffer : public CGfxStorageBuffer
{
	friend class CVKStorageBufferManager;


private:
	CVKStorageBuffer(CVKDevice* pDevice, CVKStorageBufferManager* pManager, size_t size);
	virtual ~CVKStorageBuffer(void);
	virtual void Release(void);


public:
	VkBuffer GetBuffer(void) const;

public:
	uint32_t GetSize(void) const;
	uint32_t GetOffset(void) const;

public:
	bool BufferData(size_t offset, size_t size, const void* data);


private:
	uint32_t m_size;
	uint32_t m_offset;

private:
	CVKBuffer* m_pBuffer;

private:
	CVKDevice* m_pDevice;

private:
	CVKStorageBufferManager* m_pManager;
};
