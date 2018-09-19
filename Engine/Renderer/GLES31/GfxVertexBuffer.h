#pragma once
#include "GfxRenderer.h"


class CGfxVertexBuffer
{
	friend class CGfxMesh;
	friend class CGfxVertexArrayObject;


private:
	CGfxVertexBuffer(GLuint format);
	virtual ~CGfxVertexBuffer(void);


private:
	bool BufferData(size_t size, const void *pBuffer, bool bDynamic);

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
