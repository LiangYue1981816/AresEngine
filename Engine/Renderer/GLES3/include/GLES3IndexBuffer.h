#pragma once
#include "GLES3Renderer.h"


class CGLES3IndexBuffer : public CGLES3Buffer, public CGfxIndexBuffer
{
	friend class CGLES3Mesh;
	friend class CGLES3VertexArrayObject;


private:
	CGLES3IndexBuffer(GfxIndexType type, size_t size, bool bDynamic);
	virtual ~CGLES3IndexBuffer(void);


public:
	GfxIndexType GetIndexType(void) const;
	uint32_t GetIndexCount(void) const;
	uint32_t GetSize(void) const;

public:
	bool BufferData(size_t offset, size_t size, const void *pBuffer);

private:
	void Bind(void) const;


private:
	GfxIndexType m_type;
};
