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
	void Bind(void) const;
	bool BufferData(size_t size, const void *pBuffer, bool bDynamic);

private:
	GLuint GetVertexCount(void) const;
	GLuint GetVertexFormat(void) const;
	GLuint GetVertexBuffer(void) const;
	GLuint GetSize(void) const;


private:
	GLuint m_vertexFormat;
	GLuint m_vertexBuffer;
	GLuint m_size;
};
