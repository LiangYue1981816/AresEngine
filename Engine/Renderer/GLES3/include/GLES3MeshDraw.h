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
	glm::aabb GetAABB(void) const;

public:
	uint32_t GetIndexType(void) const;
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
	void SetRenderCallback(RenderCallback callback, void* param);
	void OnRenderCallback(CGfxCommandBufferPtr ptrCommandBuffer) const;

public:
	void Bind(void) const;


private:
	uint32_t m_mask;

private:
	CGfxMeshPtr m_ptrMesh;
	CGfxMesh::Draw* m_pMeshDraw;
	CGLES3MultiInstanceBuffer* m_pMultiInstanceBuffer;

private:
	RenderCallback m_pRenderCallback;
	void* m_pRenderCallbackParam;

private:
	CGLES3MeshDrawManager* m_pManager;
};
