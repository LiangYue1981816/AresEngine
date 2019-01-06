#pragma once
#include "GLES3Renderer.h"


class CGLES3UniformBuffer : public CGfxUniformBuffer
{
	friend class CGLES3UniformBufferManager;


private:
	CGLES3UniformBuffer(CGLES3UniformBufferManager *pManager, size_t size);
	virtual ~CGLES3UniformBuffer(void);
	virtual void Release(void);


public:
	uint32_t GetSize(void) const;

public:
	bool BufferData(size_t offset, size_t size, const void *pBuffer);
	bool Bind(int index, int offset, int size);


private:
	uint32_t m_size;

private:
	uint32_t m_buffer;

private:
	CGLES3UniformBufferManager *m_pManager;
};
