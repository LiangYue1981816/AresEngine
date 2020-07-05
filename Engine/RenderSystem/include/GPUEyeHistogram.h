#pragma once
#include "PreHeader.h"


class CALL_API CGPUEyeHistogram
{
	friend class CRenderSystem;


private:
	CGPUEyeHistogram(CRenderSystem* pRenderSystem);
	virtual ~CGPUEyeHistogram(void);


public:
	void SetInputTexture(CGfxRenderTexturePtr ptrColorTexture);

private:
	void Compute(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer);


private:
	CGfxShader* m_pShaderCompute;
	CGfxPipelineCompute* m_pPipelineCompute;
	CGfxDescriptorSetPtr m_ptrDescriptorSet;
	CGfxRenderTexturePtr m_ptrInputColorTexture;

private:
	CRenderSystem* m_pRenderSystem;
};
