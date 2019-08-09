#pragma once
#include "VKRenderer.h"


class CVKMesh : public CGfxMesh
{
	friend class CVKMeshManager;


private:
	CVKMesh(CVKDevice* pDevice, CVKMeshManager* pManager, uint32_t name);
	virtual ~CVKMesh(void);
	virtual void Release(void);


public:
	CGfxMesh::Draw* GetDraw(uint32_t name);
	CGfxIndexBuffer* GetIndexBuffer(void);
	CGfxVertexBuffer* GetVertexBuffer(void);

public:
	bool CreateDraw(uint32_t name, const glm::aabb& aabb, int baseVertex, int firstIndex, int indexCount);
	bool CreateIndexBuffer(GfxIndexType type, size_t size, bool bDynamic, const void* data);
	bool CreateVertexBuffer(uint32_t vertexFormat, int vertexBinding, size_t size, bool bDynamic, const void* data);
	void Destroy(void);

public:
	void Bind(VkCommandBuffer vkCommandBuffer);


private:
	CVKIndexBuffer* m_pIndexBuffer;
	CVKVertexBuffer* m_pVertexBuffer;
	eastl::unordered_map<uint32_t, CGfxMesh::Draw> m_draws;

private:
	CVKDevice* m_pDevice;

private:
	CVKMeshManager* m_pManager;
};
