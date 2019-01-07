#pragma once
#include "VKRenderer.h"


class CVKMeshDraw : public CGfxMeshDraw
{
	friend class CVKMeshDrawManager;


private:
	CVKMeshDraw(CVKMeshDrawManager *pManager, uint32_t name, const CGfxMeshPtr &ptrMesh, int indexDraw, uint32_t instanceFormat, uint32_t instanceBinding);
	virtual ~CVKMeshDraw(void);
	virtual void Release(void);


public:
	uint32_t GetName(void) const;

public:
	GfxIndexType GetIndexType(void) const;
	uint32_t GetVertexFormat(void) const;

	uint32_t GetIndexCount(void) const;
	uint32_t GetIndexOffset(void) const;

	uint32_t GetInstanceCount(void) const;
	uint32_t GetInstanceFormat(void) const;

	glm::aabb GetLocalAABB(void) const;

public:
	bool InstanceBufferData(size_t size, const void *pBuffer);
	void Bind(VkCommandBuffer vkCommandBuffer, CVKBufferPtr &ptrIndexBuffer, CVKBufferPtr &ptrVertexBuffer, CVKBufferPtr &ptrInstanceBuffer);


private:
	uint32_t m_name;

private:
	CGfxMeshPtr m_ptrMesh;
	CGfxMesh::Draw *m_pMeshDraw;
	CVKIndexBuffer *m_pIndexBuffer;
	CVKVertexBuffer *m_pVertexBuffer;
	CVKInstanceBuffer *m_pInstanceBuffer;
	CVKIndirectBuffer *m_pIndirectBuffer;

private:
	CVKMeshDrawManager *m_pManager;
};
