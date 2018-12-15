#pragma once
#include "GLES3Renderer.h"


class CGLES3MeshDraw : public CGfxMeshDraw
{
	friend class CGLES3MeshDrawManager;


private:
	CGLES3MeshDraw(CGLES3MeshDrawManager *pManager, uint32_t name, const CGfxMeshPtr &ptrMesh, int indexDraw, uint32_t instanceFormat, uint32_t instanceBinding);
	virtual ~CGLES3MeshDraw(void);
	virtual void Release(void);


public:
	bool InstanceBufferData(size_t size, const void *pBuffer);

public:
	GfxIndexType GetIndexType(void) const;
	uint32_t GetVertexFormat(void) const;

	uint32_t GetIndexCount(void) const;
	uint32_t GetIndexOffset(void) const;

	uint32_t GetInstanceCount(void) const;
	uint32_t GetInstanceFormat(void) const;

	glm::aabb GetLocalAABB(void) const;

public:
	void Bind(void *pParam);


private:
	CGfxMeshPtr m_ptrMesh;
	CGfxMesh::Draw *m_pMeshDraw;
	CGLES3InstanceBuffer *m_pInstanceBuffer;
	CGLES3VertexArrayObject *m_pVertexArrayObject;
	CGLES3DrawIndirectBuffer *m_pDrawIndirectBuffer;

private:
	CGLES3MeshDrawManager *m_pManager;
};
