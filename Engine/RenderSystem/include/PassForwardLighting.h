#pragma once
#include "PreHeader.h"


class CALL_API CPassForwardLighting : public CPassBase
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
	CGfxDescriptorSetPtr m_ptrDescriptorSetPass;
	CGfxRenderTexturePtr m_ptrInputShadowTexture;
	CGfxRenderTexturePtr m_ptrInputSSAOTexture;

private:
	CGfxFrameBufferPtr m_ptrFrameBuffer;
	CGfxRenderTexturePtr m_ptrOutputColorTexture;
	CGfxRenderTexturePtr m_ptrOutputDepthStencilTexture;
};
