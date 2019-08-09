#pragma once
#include "GLES3Renderer.h"


class CGLES3MeshDraw : public CGfxMeshDraw
{
	friend class CGLES3MeshDrawManager;


private:
	CGLES3MeshDraw(CGLES3MeshDrawManager* pManager, uint32_t name, const CGfxMeshPtr ptrMesh, uint32_t nameDraw, uint32_t instanceFormat, int instanceBinding);
	virtual ~CGLES3MeshDraw(void);
	virtual void Release(void);


public:
	void SetMask(uint32_t mask);
	uint32_t GetMask(void) const;

public:
	CGfxMeshPtr GetMesh(void) const;

public:
	glm::aabb GetLocalAABB(void) const;

	GfxIndexType GetIndexType(void) const;
	uint32_t GetIndexCount(void) const;
	uint32_t GetIndexFirst(void) const;
	uint32_t GetIndexOffset(void) const;

	uint32_t GetVertexBinding(void) const;
	uint32_t GetVertexFormat(void) const;
	uint32_t GetVertexCount(void) const;

	uint32_t GetInstanceBinding(void) const;
	uint32_t GetInstanceFormat(void) const;
	uint32_t GetInstanceCount(void) const;

public:
	bool InstanceBufferData(size_t size, const void* data);

public:
	void Bind(void) const;


private:
	uint32_t m_mask;

private:
	CGfxMeshPtr m_ptrMesh;
	CGfxMesh::Draw* m_pMeshDraw;
	CGLES3IndirectBuffer* m_pIndirectBuffer[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];
	CGLES3InstanceBuffer* m_pInstanceBuffer[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];
	CGLES3VertexArrayObject* m_pVertexArrayObject[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];

private:
	CGLES3MeshDrawManager* m_pManager;
};
