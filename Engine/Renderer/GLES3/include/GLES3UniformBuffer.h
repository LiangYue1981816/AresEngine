#pragma once
#include "GLES3Renderer.h"


class CGLES3UniformBuffer : public CGfxUniformBuffer
{
	friend class CGLES3UniformBufferManager;


private:
	CGLES3UniformBuffer(CGLES3UniformBufferManager *pManager, size_t size, bool bDynamic);
	virtual ~CGLES3UniformBuffer(void);
	virtual void Release(void);


public:
	bool BufferData(size_t offset, size_t size, const void *pBuffer);
	bool Bind(int index, int offset, int size);


private:
	uint32_t m_buffer;

private:
	CGLES3UniformBufferManager *m_pManager;
};
