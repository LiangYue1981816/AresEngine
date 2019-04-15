#pragma once
#include "GLES3Renderer.h"


class CGLES3VertexBuffer : public CGfxVertexBuffer
{
	friend class CGLES3Mesh;
	friend class CGLES3VertexArrayObject;


private:
	CGLES3VertexBuffer(uint32_t vertexFormat, uint32_t vertexBinding, size_t size, bool bDynamic);
	virtual ~CGLES3VertexBuffer(void);


public:
	uint32_t GetVertexFormat(void) const;
	uint32_t GetVertexCount(void) const;
	uint32_t GetSize(void) const;

public:
	bool BufferData(size_t offset, size_t size, const void* pBuffer);

private:
	void Bind(void) const;


private:
	uint32_t m_format;
	uint32_t m_count;

private:
	CGLES3BufferPtr m_ptrBuffer;
};
