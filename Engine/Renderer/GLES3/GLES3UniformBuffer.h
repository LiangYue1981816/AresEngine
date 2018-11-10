#pragma once
#include "GLES3Renderer.h"


class CGLES3UniformBuffer : public CGfxUniformBuffer
{
public:
	CGLES3UniformBuffer(size_t size, bool bDynamic);
	virtual ~CGLES3UniformBuffer(void);


public:
	bool BufferData(size_t offset, size_t size, const void *pBuffer);
	bool Bind(int index, int offset, int size);


private:
	uint32_t m_buffer;
};
