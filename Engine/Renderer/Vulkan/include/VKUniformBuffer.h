#pragma once
#include "VKRenderer.h"


class CVKUniformBuffer : public CGfxUniformBuffer
{
private:
	CVKUniformBuffer(CVKDevice* pDevice, size_t size);
	virtual ~CVKUniformBuffer(void);


public:
	uint32_t GetSize(void) const;

public:
	bool BufferData(size_t offset, size_t size, const void* data);
};
/*
class CVKUniformBuffer : public CGfxUniformBuffer
{
	friend class CVKUniformBufferManager;


private:
	CVKUniformBuffer(CVKDevice* pDevice, CVKUniformBufferManager* pManager, size_t size);
	virtual ~CVKUniformBuffer(void);
	virtual void Release(void);


public:
	HANDLE GetBuffer(void) const;

public:
	uint32_t GetSize(void) const;
	uint32_t GetBaseOffset(void) const;

public:
	bool BufferData(size_t offset, size_t size, const void* pBuffer);


private:
	uint32_t m_size;
	uint32_t m_baseOffset;

private:
	CVKBufferPtr m_ptrBuffer;

private:
	CVKDevice* m_pDevice;
	CVKUniformBufferManager* m_pManager;
};
*/