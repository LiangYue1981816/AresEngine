#pragma once
#include "GLES3Renderer.h"


class CGLES3Mesh : public CGfxMesh
{
	friend class CGLES3MeshManager;


private:
	CGLES3Mesh(CGLES3MeshManager* pManager, uint32_t name);
	virtual ~CGLES3Mesh(void);
	virtual void Release(void);


public:
	CGfxMesh::Draw* GetDraw(uint32_t name);
	CGfxIndexBufferPtr GetIndexBufferPtr(void);
	CGfxVertexBufferPtr GetVertexBufferPtr(void);

public:
	bool CreateDraw(uint32_t name, const glm::aabb& aabb, int baseVertex, int firstIndex, int indexCount);
	bool CreateIndexBuffer(GfxIndexType type, size_t size, bool bDynamic, const void* data);
	bool CreateVertexBuffer(uint32_t vertexFormat, int vertexBinding, size_t size, bool bDynamic, const void* data);
	void Destroy(void);


private:
	CGfxIndexBufferPtr m_ptrIndexBuffer;
	CGfxVertexBufferPtr m_ptrVertexBuffer;
	eastl::unordered_map<uint32_t, CGfxMesh::Draw> m_draws;

private:
	CGLES3MeshManager* m_pManager;
};
