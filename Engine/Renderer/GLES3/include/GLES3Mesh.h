#pragma once
#include "GLES3Renderer.h"


class CGLES3Mesh : public CGfxMesh
{
	friend class CGLES3MeshManager;


private:
	CGLES3Mesh(CGLES3MeshManager *pManager, uint32_t name);
	virtual ~CGLES3Mesh(void);
	virtual void Release(void);


public:
	uint32_t GetName(void) const;

public:
	CGfxMesh::Draw* GetDraw(int indexDraw);
	CGfxIndexBuffer* GetIndexBuffer(void);
	CGfxVertexBuffer* GetVertexBuffer(void);

public:
	bool CreateIndexBuffer(GfxIndexType type, size_t size, bool bDynamic, const void *pBuffer);
	bool CreateVertexBuffer(uint32_t vertexFormat, uint32_t vertexBinding, size_t size, bool bDynamic, const void *pBuffer);
	bool CreateDraw(int indexDraw, const glm::aabb &aabb, int baseVertex, int firstIndex, int indexCount);
	void Destroy(void);


private:
	uint32_t m_name;

private:
	CGLES3IndexBuffer *m_pIndexBuffer;
	CGLES3VertexBuffer *m_pVertexBuffer;
	eastl::unordered_map<int, CGfxMesh::Draw> m_draws;

private:
	CGLES3MeshManager *m_pManager;
};
