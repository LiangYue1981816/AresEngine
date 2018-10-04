#pragma once
#include "GfxRenderer.h"


class CGfxIndexBuffer
{
	friend class CGfxMesh;
	friend class CGfxVertexArrayObject;


private:
	CGfxIndexBuffer(uint32_t type, size_t size, bool bDynamic);
	virtual ~CGfxIndexBuffer(void);


private:
	bool BufferData(size_t offset, size_t size, const void *pBuffer);

private:
	uint32_t GetIndexType(void) const;
	uint32_t GetIndexCount(void) const;
	uint32_t GetIndexBuffer(void) const;
	uint32_t GetSize(void) const;

private:
	void Bind(void) const;


private:
	uint32_t m_indexType;
	uint32_t m_indexBuffer;
	uint32_t m_size;
};
