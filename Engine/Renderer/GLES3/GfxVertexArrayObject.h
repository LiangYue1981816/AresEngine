#pragma once
#include "GfxRenderer.h"


class CGfxVertexArrayObject
{
	friend class CGfxMesh;


private:
	CGfxVertexArrayObject(void);
	virtual ~CGfxVertexArrayObject(void);


private:
	bool Buffer(CGfxIndexBuffer *pIndexBuffer, CGfxVertexBuffer *pVertexBuffer, CGfxInstanceBuffer *pInstanceBuffer) const;

private:
	void Bind(void) const;


private:
	uint32_t m_vao;
};
