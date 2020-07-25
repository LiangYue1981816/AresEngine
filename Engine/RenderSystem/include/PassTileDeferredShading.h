#pragma once
#include "PreHeader.h"


class CPassTileDeferredShading : public CPassBlit
{
	friend class CCamera;
	friend class CRenderSystem;


private:
	static void Create(GfxPixelFormat colorPixelFormat, GfxPixelFormat depthPixelFormat);
	static void Destroy(void);


private:
	CPassTileDeferredShading(CRenderSystem* pRenderSystem);
	virtual ~CPassTileDeferredShading(void);


public:
	void SetCamera(CCamera* pCamera);
	void SetInputTexture(CGfxRenderTexturePtr ptrDepthTexture, CGfxRenderTexturePtr ptrShadowTexture, CGfxRenderTexturePtr ptrSSAOTexture);
	void SetOutputTexture(CGfxRenderTexturePtr ptrColorTexture, CGfxRenderTexturePtr ptrGBuffer0Texture, CGfxRenderTexturePtr ptrGBuffer1Texture, CGfxRenderTexturePtr ptrDepthStencilTexture);

private:
	void Render(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer);
	void RenderCallback(CGfxCommandBufferPtr ptrCommandBuffer);


private:
	CGfxDescriptorSetPtr m_ptrDescriptorSetPass_Subpass0;
	CGfxRenderTexturePtr m_ptrInputShadowTexture;
	CGfxRenderTexturePtr m_ptrInputSSAOTexture;

private:
	CGfxDescriptorSetPtr m_ptrDescriptorSetPass_Subpass1;
	CGfxRenderTexturePtr m_ptrInputDepthTexture;

private:
	CGfxFrameBufferPtr m_ptrFrameBuffer;
	CGfxRenderTexturePtr m_ptrOutputColorTexture;
	CGfxRenderTexturePtr m_ptrOutputGBuffer0Texture; // Albedo AO
	CGfxRenderTexturePtr m_ptrOutputGBuffer1Texture; // Normal Roughness Metallic
	CGfxRenderTexturePtr m_ptrOutputDepthStencilTexture;
};
