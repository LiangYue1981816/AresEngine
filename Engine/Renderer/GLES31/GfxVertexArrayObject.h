#pragma once
#include "GfxRenderer.h"


class CGfxVertexArrayObject
{
	friend class CGfxMesh;


private:
	CGfxVertexArrayObject(void);
	virtual ~CGfxVertexArrayObject(void);


private:
	bool Buffer(CGfxIndexBuffer *pIndexBuffer, CGfxVertexBuffer *pVertexBuffer, CGfxInstanceBuffer *pInstanceBuffer);

private:
	void Bind(void) const;


private:
	GLuint m_vao;
	CGfxIndexBuffer *m_pIndexBuffer;
	CGfxVertexBuffer *m_pVertexBuffer;
	CGfxInstanceBuffer *m_pInstanceBuffer;
};
