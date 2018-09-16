#pragma once
#include "GfxRenderer.h"


class CGfxIndexBuffer
{
	friend class CGfxMesh;
	friend class CGfxVertexArrayObject;


private:
	CGfxIndexBuffer(GLenum type);
	virtual ~CGfxIndexBuffer(void);


private:
	void Bind(void) const;
	bool BufferData(size_t size, const void *pBuffer, bool bDynamic);

private:
	GLenum GetIndexType(void) const;
	GLuint GetIndexCount(void) const;
	GLuint GetIndexBuffer(void) const;
	GLsizeiptr GetSize(void) const;


private:
	GLenum m_indexType;
	GLuint m_indexBuffer;
	GLsizeiptr m_size;
};
