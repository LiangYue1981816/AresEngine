#pragma once
#include "GLES3Renderer.h"


class CGLES3MeshDraw : public CGfxMeshDraw
{
	friend class CGLES3MeshDrawManager;


private:
	CGLES3MeshDraw(CGLES3MeshDrawManager* pManager, uint32_t name, const CGfxMeshPtr ptrMesh, uint32_t nameDraw);
	virtual ~CGLES3MeshDraw(void);
	virtual void Release(void);


public:
	void SetMask(uint32_t mask);
	uint32_t GetMask(void) const;

public:
	glm::aabb GetAABB(void) const;
	CGfxMeshPtr GetMesh(void) const;

public:
	uint32_t GetIndexType(void) const;
	uint32_t GetIndexCount(void) const;
	uint32_t GetIndexFirst(void) const;
	uint32_t GetIndexOffset(void) const;

	uint32_t GetVertexBinding(void) const;
	uint32_t GetVertexFormat(void) const;
	uint32_t GetVertexCount(void) const;

public:
	void SetRenderCallback(RenderCallback callback, void* param);
	void OnRenderCallback(CGfxCommandBufferPtr ptrCommandBuffer) const;


private:
	uint32_t m_mask;

private:
	CGfxMeshPtr m_ptrMesh;
	CGfxMesh::Draw* m_pMeshDraw;

private:
	RenderCallback m_pRenderCallback;
	void* m_pRenderCallbackParam;

private:
	CGLES3MeshDrawManager* m_pManager;
};
