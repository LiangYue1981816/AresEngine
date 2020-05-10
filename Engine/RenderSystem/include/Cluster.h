#pragma once
#include "PreHeader.h"


class CALL_API CCluster
{
	friend class CRenderSystem;
	friend class CPassForwardLighting;
	friend class CPassDeferredLighting;


public:
	CCluster(int screenWidth, int screenHeight, int tileSize, int numSlices);
	virtual ~CCluster(void);


private:
	const CGfxStorageBufferPtr GetLightListBuffer(void) const;

public:
	void SetCamera(CCamera* pCamera);

private:
	void Update(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer);


private:
	CCamera* m_pCamera;

private:
	CGfxShader* m_pShaderCompute;
	CGfxPipelineCompute* m_pPipelineCompute;
	CGfxDescriptorSetPtr m_ptrDescriptorSet;

private:
	CGfxStorageBufferPtr m_ptrClusterBuffer;
	CGfxStorageBufferPtr m_ptrLightListBuffer;
};
