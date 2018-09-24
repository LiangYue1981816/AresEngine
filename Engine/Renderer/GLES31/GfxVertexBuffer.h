#pragma once
#include "GfxRenderer.h"


class CGfxVertexBuffer
{
	friend class CGfxMesh;
	friend class CGfxVertexArrayObject;


private:
	CGfxVertexBuffer(GLuint format, size_t size, bool bDynamic);
	virtual ~CGfxVertexBuffer(void);


private:
	bool BufferData(size_t offset, size_t size, const void *pBuffer);

private:
	GLuint GetVertexCount(void) const;
	GLuint GetVertexFormat(void) const;
	GLuint GetVertexBuffer(void) const;
	GLsizeiptr GetSize(void) const;

private:
	void Bind(void) const;


private:
	GLuint m_vertexFormat;
	GLuint m_vertexBuffer;
	GLsizeiptr m_size;
};
