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
	glm::aabb GetLocalAABB(void) const;

	GfxIndexType GetIndexType(void) const;
	uint32_t GetIndexCount(void) const;
	uint32_t GetIndexFirst(void) const;
	uint32_t GetIndexOffset(void) const;

	uint32_t GetVertexFormat(void) const;
	uint32_t GetVertexCount(void) const;

	uint32_t GetInstanceFormat(void) const;
	uint32_t GetInstanceCount(void) const;

public:
	VkBuffer GetIndirectBuffer(void) const;
	uint32_t GetIndirectBufferStride(void) const;

public:
	bool InstanceBufferData(size_t size, const void* data);

public:
	void Bind(VkCommandBuffer vkCommandBuffer);


private:
	CGfxMeshPtr m_ptrMesh;
	CGfxMesh::Draw* m_pMeshDraw;
	CVKIndirectBuffer* m_pIndirectBuffer[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];
	CVKInstanceBuffer* m_pInstanceBuffer[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];

private:
	CVKDevice* m_pDevice;

private:
	CVKMeshDrawManager* m_pManager;
};
