#pragma once
#include "PreHeader.h"


class CALL_API CRenderSystem;
class CALL_API CPassLuminanceThreshold : public CPassBlit
{
	friend class CCamera;
	friend class CRenderSystem;


private:
	static void Create(GfxPixelFormat colorPixelFormat);
	static void Destroy(void);


private:
	CPassLuminanceThreshold(CRenderSystem* pRenderSystem);
	virtual ~CPassLuminanceThreshold(void);


public:
	void SetCamera(CCamera* pCamera);
	void SetInputTexture(CGfxRenderTexturePtr ptrColorTexture);
	void SetOutputTexture(CGfxRenderTexturePtr ptrColorTexture);

	void SetParamThreshold(float threshold);

private:
	void Render(CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer);
	void RenderCallback(CGfxCommandBufferPtr ptrCommandBuffer);


private:
	float m_threshold;

private:
	CGfxDescriptorSetPtr m_ptrDescriptorSetPass;
	CGfxRenderTexturePtr m_ptrInputColorTexture;

private:
	CGfxFrameBufferPtr m_ptrFrameBuffer;
	CGfxRenderTexturePtr m_ptrOutputColorTexture;
};
