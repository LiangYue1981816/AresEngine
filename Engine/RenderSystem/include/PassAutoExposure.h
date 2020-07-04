#pragma once
#include "PreHeader.h"


class CALL_API CPassAutoExposure : public CPassBlit
{
	friend class CCamera;
	friend class CRenderSystem;


private:
	static void Create(GfxPixelFormat colorPixelFormat);
	static void Destroy(void);


private:
	CPassAutoExposure(CRenderSystem* pRenderSystem);
	virtual ~CPassAutoExposure(void);


public:
	void SetCamera(CCamera* pCamera);
	void SetInputReferenceTexture(CGfxRenderTexturePtr ptrInputReferenceTexture);
	void SetInputTexture(CGfxRenderTexturePtr ptrColorTexture);
	void SetOutputTexture(CGfxRenderTexturePtr ptrColorTexture);

private:
	void Render(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer);


private:
	CGfxShader* m_pShaderCompute;
	CGfxPipelineCompute* m_pPipelineCompute;
	CGfxDescriptorSetPtr m_ptrDescriptorSetEyeHistogram;
	CGfxDescriptorSetPtr m_ptrDescriptorSetAutoExposure;
	CGfxRenderTexturePtr m_ptrInputColorTexture;
	CGfxRenderTexturePtr m_ptrInputReferenceTexture;

private:
	CGfxFrameBufferPtr m_ptrFrameBuffer;
	CGfxRenderTexturePtr m_ptrOutputColorTexture;
};
