#pragma once
#include "GLES3Renderer.h"


class CGLES3VertexBuffer : public CGfxVertexBuffer
{
	friend class CGLES3Mesh;


private:
	CGLES3VertexBuffer(uint32_t vertexFormat, uint32_t vertexBinding, size_t size, bool bDynamic);
	virtual ~CGLES3VertexBuffer(void);


public:
	bool BufferData(size_t offset, size_t size, const void *pBuffer);
	void Bind(void *pParam);


private:
	uint32_t m_buffer;
};
