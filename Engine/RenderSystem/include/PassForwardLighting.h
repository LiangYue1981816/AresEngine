#pragma once
#include "RenderSystem.h"


class CALL_API CPassForwardLighting
{
	friend class CCamera;
	friend class CRenderSystem;


private:
	static void Create(GfxPixelFormat colorPixelFormat, GfxPixelFormat depthPixelFormat);
	static void Destroy(void);


private:
	CPassForwardLighting(CRenderSystem* pRenderSystem);
	virtual ~CPassForwardLighting(void);


public:
	void SetCamera(CCamera* pCamera);
	void SetInputTexture(CGfxRenderTexturePtr ptrShadowTexture, CGfxRenderTexturePtr ptrSSAOTexture);
	void SetOutputTexture(CGfxRenderTexturePtr ptrColorTexture, CGfxRenderTexturePtr ptrDepthStencilTexture);

private:
	void Render(CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer);


private:
	CGfxFrameBufferPtr m_ptrFrameBuffer;
	CGfxRenderTexturePtr m_ptrColorTexture;
	CGfxRenderTexturePtr m_ptrDepthStencilTexture;

private:
	CGfxDescriptorSetPtr m_ptrDescriptorSetPass;

private:
	CCamera* m_pCamera;
	CRenderSystem* m_pRenderSystem;
};
