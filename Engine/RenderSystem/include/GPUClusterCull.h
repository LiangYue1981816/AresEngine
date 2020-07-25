#pragma once
#include "PreHeader.h"


class CALL_API CGPUClusterCull : public CPassBase
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
	CGfxMeshPtr m_ptrMesh;
	CGfxMeshDrawPtr m_ptrMeshDraw;
	CGfxMaterialPtr m_ptrMaterialCullFaceBack;
	CGfxMaterialPtr m_ptrMaterialCullFaceFront;

private:
	CGfxShader* m_pShaderCompute;
	CGfxPipelineCompute* m_pPipelineCompute;
	CGfxDescriptorSetPtr m_ptrDescriptorSet;
};
