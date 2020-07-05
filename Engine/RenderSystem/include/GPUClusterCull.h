#pragma once
#include "PreHeader.h"


class CALL_API CGPUClusterCull
{
	friend class CRenderSystem;


private:
	CGPUClusterCull(CRenderSystem* pRenderSystem);
	virtual ~CGPUClusterCull(void);


public:
	void SetCamera(CCamera* pCamera);

private:
	void Compute(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer);


private:
	CCamera* m_pCamera;

private:
	CGfxShader* m_pShaderCompute;
	CGfxPipelineCompute* m_pPipelineCompute;
	CGfxDescriptorSetPtr m_ptrDescriptorSet;

private:
	CRenderSystem* m_pRenderSystem;
};
