#pragma once
#include "GLES3Renderer.h"


class CGLES3StorageBuffer : public CGfxStorageBuffer
{
	friend class CGLES3StorageBufferManager;


private:
	CGLES3StorageBuffer(CGLES3StorageBufferManager* pManager, size_t size);
	virtual ~CGLES3StorageBuffer(void);
	virtual void Release(void);


public:
	uint32_t GetSize(void) const;
	uint32_t GetRange(void) const;
	uint32_t GetOffset(void) const;

public:
	bool BufferRange(size_t offset, size_t range);
	bool BufferData(size_t offset, size_t size, const void* data);

public:
	void Bind(int binding) const;


private:
	uint32_t m_range;
	uint32_t m_offset;

private:
	CGLES3Buffer* m_pBuffer;

private:
	CGLES3StorageBufferManager* m_pManager;
};
