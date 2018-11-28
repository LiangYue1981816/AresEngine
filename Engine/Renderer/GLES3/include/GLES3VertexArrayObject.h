#pragma once
#include "GLES3Renderer.h"


class CGLES3VertexArrayObject
{
	friend class CGLES3Mesh;


private:
	CGLES3VertexArrayObject(void);
	virtual ~CGLES3VertexArrayObject(void);


public:
	bool Buffer(CGfxIndexBuffer *pIndexBuffer, CGfxVertexBuffer *pVertexBuffer, CGfxInstanceBuffer *pInstanceBuffer);
	void Bind(void *pParam);


private:
	uint32_t m_vao;
};
