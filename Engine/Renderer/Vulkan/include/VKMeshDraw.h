#pragma once
#include "VKRenderer.h"


class CVKMeshDraw : public CGfxMeshDraw
{
	friend class CVKMeshDrawManager;


private:
	CVKMeshDraw(CVKDevice* pDevice, CVKMeshDrawManager* pManager, uint32_t name, const CGfxMeshPtr ptrMesh, uint32_t nameDraw, uint32_t instanceFormat, int instanceBinding);
	virtual ~CVKMeshDraw(void);
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
	void Bind(VkCommandBuffer vkCommandBuffer);


private:
	uint32_t m_mask;

private:
	CGfxMeshPtr m_ptrMesh;
	CGfxMesh::Draw* m_pMeshDraw;
	CVKInstanceBuffer* m_pInstanceBuffer[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];

private:
	RenderCallback m_pRenderCallback;
	void* m_pRenderCallbackParam;

private:
	CVKDevice* m_pDevice;

private:
	CVKMeshDrawManager* m_pManager;
};
