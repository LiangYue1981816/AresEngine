#pragma once
#include "VKRenderer.h"


class CVKUniformBuffer : public CGfxUniformBuffer
{
	friend class CVKUniformBufferManager;


private:
	CVKUniformBuffer(CVKDevice* pDevice, CVKUniformBufferManager* pManager, size_t size);
	virtual ~CVKUniformBuffer(void);
	virtual void Release(void);


public:
	VkBuffer GetBuffer(void) const;

public:
	uint32_t GetSize(void) const;
	uint32_t GetRange(void) const;
	uint32_t GetOffset(void) const;

public:
	bool BufferData(size_t offset, size_t size, const void* data);
	bool BufferRange(size_t offset, size_t range);


private:
	CVKBuffer* m_pBuffer;

private:
	CVKDevice* m_pDevice;

private:
	CVKUniformBufferManager* m_pManager;
};
