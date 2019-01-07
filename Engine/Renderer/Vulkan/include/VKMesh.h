#pragma once
#include "VKRenderer.h"


class CVKMesh : public CGfxMesh
{
	friend class CVKMeshManager;


private:
	CVKMesh(CVKMeshManager *pManager, uint32_t name);
	virtual ~CVKMesh(void);
	virtual void Release(void);


public:
	uint32_t GetName(void) const;

public:
	CGfxIndexBuffer* GetIndexBuffer(void);
	CGfxVertexBuffer* GetVertexBuffer(void);
	Draw* GetDraw(int indexDraw);

	GfxIndexType GetIndexType(void) const;
	uint32_t GetVertexFormat(void) const;

public:
	bool CreateIndexBuffer(GfxIndexType type, size_t size, bool bDynamic, const void *pBuffer);
	bool CreateVertexBuffer(uint32_t vertexFormat, uint32_t vertexBinding, size_t size, bool bDynamic, const void *pBuffer);
	bool CreateDraw(int indexDraw, const glm::aabb &aabb, int baseVertex, int firstIndex, int indexCount);
	void Destroy(void);


private:
	uint32_t m_name;

private:
	CVKIndexBuffer *m_pIndexBuffer;
	CVKVertexBuffer *m_pVertexBuffer;
	eastl::unordered_map<int, Draw> m_draws;

private:
	CVKMeshManager *m_pManager;
};
