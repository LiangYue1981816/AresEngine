#pragma once
#include "GLES3Renderer.h"


class CGLES3MeshDraw : public CGfxMeshDraw
{
	friend class CGLES3MeshDrawManager;


private:
	CGLES3MeshDraw(CGLES3MeshDrawManager *pManager, uint32_t name, const CGfxMeshPtr ptrMesh, int indexDraw, uint32_t instanceFormat, uint32_t instanceBinding);
	virtual ~CGLES3MeshDraw(void);
	virtual void Release(void);


public:
	uint32_t GetName(void) const;

public:
	uint32_t GetVertexFormat(void) const;

	uint32_t GetIndexCount(void) const;
	uint32_t GetIndexOffset(void) const;
	GfxIndexType GetIndexType(void) const;

	uint32_t GetInstanceCount(void) const;
	uint32_t GetInstanceFormat(void) const;

public:
	glm::aabb GetLocalAABB(void) const;

public:
	bool InstanceBufferData(size_t size, const void *pBuffer);

public:
	void Bind(void) const;


private:
	uint32_t m_name;

private:
	CGfxMeshPtr m_ptrMesh;
	CGfxMesh::Draw *m_pMeshDraw;
	CGLES3InstanceBuffer *m_pInstanceBuffer;
	CGLES3IndirectBuffer *m_pIndirectBuffer;
	CGLES3VertexArrayObject *m_pVertexArrayObject;

private:
	CGLES3MeshDrawManager *m_pManager;
};
