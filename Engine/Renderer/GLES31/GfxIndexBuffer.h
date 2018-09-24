#pragma once
#include "GfxRenderer.h"


class CGfxIndexBuffer
{
	friend class CGfxMesh;
	friend class CGfxVertexArrayObject;


private:
	CGfxIndexBuffer(GLenum type, size_t size, bool bDynamic);
	virtual ~CGfxIndexBuffer(void);


private:
	bool BufferData(size_t offset, size_t size, const void *pBuffer);

private:
	GLenum GetIndexType(void) const;
	GLuint GetIndexCount(void) const;
	GLuint GetIndexBuffer(void) const;
	GLsizeiptr GetSize(void) const;

private:
	void Bind(void) const;


private:
	GLenum m_indexType;
	GLuint m_indexBuffer;
	GLsizeiptr m_size;
};
