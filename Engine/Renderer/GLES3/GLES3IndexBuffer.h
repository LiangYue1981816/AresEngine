#pragma once
#include "GLES3Renderer.h"


class CGLES3IndexBuffer : public CGfxIndexBuffer
{
	friend class CGLES3Mesh;


private:
	CGLES3IndexBuffer(uint32_t type, size_t size, bool bDynamic);
	virtual ~CGLES3IndexBuffer(void);


public:
	bool BufferData(size_t offset, size_t size, const void *pBuffer);
	void Bind(void *pParam);


private:
	uint32_t m_buffer;
};
