#pragma once
#include "GLES3Renderer.h"


class CGLES3VertexArrayObject
{
	friend class CGLES3Mesh;
	friend class CGLES3MeshDraw;


private:
	CGLES3VertexArrayObject(void);
	virtual ~CGLES3VertexArrayObject(void);


public:
	bool Buffer(CGLES3IndexBuffer *pIndexBuffer, CGLES3VertexBuffer *pVertexBuffer, CGLES3InstanceBuffer *pInstanceBuffer);
	void Bind(void);


private:
	uint32_t m_vao;
};
