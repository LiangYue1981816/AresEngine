#pragma once
#include "GfxRenderer.h"


class CGfxVertexArrayObject
{
	friend class CGfxMesh;


private:
	CGfxVertexArrayObject(const CGfxIndexBuffer *pIndexBuffer, const CGfxVertexBuffer *pVertexBuffer, const CGfxInstanceBuffer *pInstanceBuffer);
	virtual ~CGfxVertexArrayObject(void);


private:
	void Bind(void) const;


private:
	GLuint m_vao;
};
