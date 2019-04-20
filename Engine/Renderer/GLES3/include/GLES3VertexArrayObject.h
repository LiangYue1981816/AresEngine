#pragma once
#include "GLES3Renderer.h"


class CGLES3VertexArrayObject
{
	friend class CGLES3Mesh;
	friend class CGLES3MeshDraw;


private:
	CGLES3VertexArrayObject(const CGLES3IndexBuffer* pIndexBuffer, const CGLES3VertexBuffer* pVertexBuffer, const CGLES3InstanceBuffer* pInstanceBuffer);
	virtual ~CGLES3VertexArrayObject(void);


private:
	void Bind(void) const;


private:
	uint32_t m_vao;
};
