#pragma once
#include "PreHeader.h"


class CALL_API CCluster
{
	friend class CRenderSystem;
	friend class CPassForwardLighting;
	friend class CPassDeferredLighting;


public:
	CCluster(CRenderSystem* pRenderSystem, int screenWidth, int screenHeight, int tileSize, int numSlices, int maxInstanceCount);
	virtual ~CCluster(void);


private:
	const CGfxStorageBufferPtr GetClusterBuffer(void) const;
	const CGfxStorageBufferPtr GetCullLightListBuffer(void) const;

public:
	void SetCamera(CCamera* pCamera);

private:
	void Update(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer);


private:
	const int MAX_INSTANCE_COUNT;
	const int HORIZONTAL_TILE_COUNT;
	const int VERTICAL_TILE_COUNT;
	const int DEPTH_SLICE_COUNT;
	const int TILE_SIZE;

private:
	CCamera* m_pCamera;
	CRenderSystem* m_pRenderSystem;

private:
	CGfxShader* m_pShaderCompute;
	CGfxPipelineCompute* m_pPipelineCompute;
	CGfxDescriptorSetPtr m_ptrDescriptorSet;

private:
	CGfxStorageBufferPtr m_ptrClusterBuffer;
	CGfxStorageBufferPtr m_ptrFullLightListBuffer;
	CGfxStorageBufferPtr m_ptrCullLightListBuffer;
};
