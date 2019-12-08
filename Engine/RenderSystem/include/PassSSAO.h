#pragma once
#include "PreHeader.h"


class CALL_API CPassSSAO : public CPassBlit
{
	friend class CCamera;
	friend class CRenderSystem;


private:
	static void Create(GfxPixelFormat colorPixelFormat);
	static void Destroy(void);


private:
	CPassSSAO(CRenderSystem* pRenderSystem);
	virtual ~CPassSSAO(void);


public:
	void SetCamera(CCamera* pCamera);
	void SetInputTexture(CGfxRenderTexturePtr ptrDepthTexture);
	void SetOutputTexture(CGfxRenderTexturePtr ptrColorTexture);

	void SetParamSamples(int samples);
	void SetParamMinRadius(float minRadius);
	void SetParamMaxRadius(float maxRadius);

private:
	void Render(CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer);
	void RenderCallback(CGfxCommandBufferPtr ptrCommandBuffer);


private:
	int m_samples;
	float m_minRadius;
	float m_maxRadius;

private:
	CGfxDescriptorSetPtr m_ptrDescriptorSetPass;
	CGfxRenderTexturePtr m_ptrInputDepthTexture;

private:
	CGfxFrameBufferPtr m_ptrFrameBuffer;
	CGfxRenderTexturePtr m_ptrOutputColorTexture;
};
