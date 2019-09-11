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

private:
	void Render(CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer);
	void RenderCallback(CGfxCommandBufferPtr ptrCommandBuffer);


private:
	CGfxFrameBufferPtr m_ptrFrameBuffer;
	CGfxRenderTexturePtr m_ptrColorTexture;

private:
	CGfxDescriptorSetPtr m_ptrDescriptorSetPass;
};
