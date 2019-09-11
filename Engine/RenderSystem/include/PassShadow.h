#pragma once
#include "PreHeader.h"


class CALL_API CPassShadow
{
	friend class CCamera;
	friend class CRenderSystem;


private:
	static void Create(GfxPixelFormat depthPixelFormat);
	static void Destroy(void);


private:
	CPassShadow(CRenderSystem* pRenderSystem);
	virtual ~CPassShadow(void);


public:
	void SetCamera(CCamera* pCamera);
	void SetSplitFactors(float f1, float f2, float f3, float f4);
	void SetOutputTexture(CGfxRenderTexturePtr ptrDepthTexture);

private:
	void Render(CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer);


private:
	float m_splitFactors[5];
	CGfxCamera* m_pShadowCamera[4];
	CGfxRenderQueue* m_pShadowRenderQueue[4];

private:
	CGfxFrameBufferPtr m_ptrFrameBuffer;
	CGfxRenderTexturePtr m_ptrDepthTexture;

private:
	CGfxUniformCamera* m_pShadowCameraUniform[4];
	CGfxDescriptorSetPtr m_ptrDescriptorSetPass[4];

private:
	CCamera* m_pCamera;
	CRenderSystem* m_pRenderSystem;
};
