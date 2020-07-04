#pragma once
#include "PreHeader.h"

/*
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
	void SetInputTexture(CGfxRenderTexturePtr ptrDepthTexture);
	void SetOutputTexture(CGfxRenderTexturePtr ptrColorTexture);

	void SetParamSamples(int samples);
	void SetParamMinSampleRadius(float minRadius);
	void SetParamMaxSampleRadius(float maxRadius);
	void SetParamMinDepthRange(float minDepth);
	void SetParamMaxDepthRange(float maxDepth);

private:
	void Render(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer);
	void RenderCallback(CGfxCommandBufferPtr ptrCommandBuffer);


private:
	int m_samples;
	float m_minSampleRadius;
	float m_maxSampleRadius;
	float m_minDepthRange;
	float m_maxDepthRange;

private:
	CGfxDescriptorSetPtr m_ptrDescriptorSetPass;
	CGfxRenderTexturePtr m_ptrInputDepthTexture;

private:
	CGfxFrameBufferPtr m_ptrFrameBuffer;
	CGfxRenderTexturePtr m_ptrOutputColorTexture;
};
*/