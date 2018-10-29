#pragma once
#include "GfxRenderer.h"


class CGfxVertexBuffer
{
	friend class CGfxMesh;
	friend class CGfxVertexArrayObject;


private:
	CGfxVertexBuffer(uint32_t format, size_t size, bool bDynamic);
	virtual ~CGfxVertexBuffer(void);


private:
	bool BufferData(size_t offset, size_t size, const void *pBuffer) const;

private:
	uint32_t GetVertexCount(void) const;
	uint32_t GetVertexFormat(void) const;
	uint32_t GetVertexBuffer(void) const;
	uint32_t GetSize(void) const;

private:
	void Bind(void) const;
	void SetupFormat(void) const;


private:
	uint32_t m_vertexFormat;
	uint32_t m_vertexBuffer;
	uint32_t m_size;
};
