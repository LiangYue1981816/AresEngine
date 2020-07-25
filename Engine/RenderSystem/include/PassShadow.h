#pragma once
#include "PreHeader.h"


class CPassShadow : public CPassBase
{
	friend class CCamera;
	friend class CRenderQueue;
	friend class CRenderSystem;


private:
	static void Create(GfxPixelFormat depthPixelFormat);
	static void Destroy(void);


private:
	CPassShadow(CRenderSystem* pRenderSystem);
	virtual ~CPassShadow(void);


public:
	void SetCamera(CCamera* pCamera);
	void SetOutputTexture(CGfxRenderTexturePtr ptrDepthTexture);

	void SetParamFactor(float factor);
	void SetParamSplitFactors(float f1, float f2, float f3, float f4);

private:
	void Render(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer);


private:
	float m_factor;
	float m_splitFactors[5];

private:
	CCamera* m_pShadowCamera[4];

private:
	CGfxDescriptorSetPtr m_ptrDescriptorSetPass[4];

private:
	CGfxFrameBufferPtr m_ptrFrameBuffer;
	CGfxRenderTexturePtr m_ptrOutputDepthTexture;
};
