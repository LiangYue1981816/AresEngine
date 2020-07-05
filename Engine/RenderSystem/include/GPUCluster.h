#pragma once
#include "PreHeader.h"


class CALL_API CGPUCluster : public CPassBase
{
	friend class CRenderSystem;


private:
	CGPUCluster(CRenderSystem* pRenderSystem);
	virtual ~CGPUCluster(void);


public:
	void SetCamera(CCamera* pCamera);

private:
	void Compute(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer);


private:
	CGfxShader* m_pShaderCompute;
	CGfxPipelineCompute* m_pPipelineCompute;
	CGfxDescriptorSetPtr m_ptrDescriptorSet;
};
