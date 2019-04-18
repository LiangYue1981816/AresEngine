#pragma once
#include "GLES3Renderer.h"


class CGLES3VertexArrayObject
{
	friend class CGLES3Mesh;
	friend class CGLES3MeshDraw;


private:
	CGLES3VertexArrayObject(CGLES3IndexBuffer *pIndexBuffer, CGLES3VertexBuffer *pVertexBuffer, CGLES3InstanceBuffer *pInstanceBuffer);
	virtual ~CGLES3VertexArrayObject(void);


private:
	void Bind(void) const;


private:
	uint32_t m_vao;
};
