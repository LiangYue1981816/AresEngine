#pragma once
#include "GLES3Renderer.h"


class CGLES3IndexBuffer : public CGfxIndexBuffer
{
	friend class CGLES3Mesh;


private:
	CGLES3IndexBuffer(GfxIndexType type, size_t size, bool bDynamic);
	virtual ~CGLES3IndexBuffer(void);


public:
	GfxIndexType GetIndexType(void) const;
	uint32_t GetIndexCount(void) const;
	uint32_t GetSize(void) const;

public:
	bool BufferData(size_t offset, size_t size, const void *pBuffer);
	void Bind(void);


private:
	GfxIndexType m_type;
	uint32_t m_size;

private:
	uint32_t m_buffer;
};
