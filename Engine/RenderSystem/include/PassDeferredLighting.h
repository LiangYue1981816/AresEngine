#pragma once
#include "PreHeader.h"


class CALL_API CPassDeferredLighting : public CPassBase
{
	friend class CCamera;
	friend class CRenderSystem;


private:
	static void Create(GfxPixelFormat colorPixelFormat, GfxPixelFormat depthPixelFormat);
	static void Destroy(void);


private:
	CPassDeferredLighting(CRenderSystem* pRenderSystem);
	virtual ~CPassDeferredLighting(void);


public:
	void SetCamera(CCamera* pCamera);
	void SetInputTexture(CGfxRenderTexturePtr ptrShadowTexture, CGfxRenderTexturePtr ptrSSAOTexture);
	void SetOutputTexture(CGfxRenderTexturePtr ptrColorTexture, CGfxRenderTexturePtr ptrGBufferATexture, CGfxRenderTexturePtr ptrGBufferBTexture, CGfxRenderTexturePtr ptrGBufferCTexture, CGfxRenderTexturePtr ptrDepthStencilTexture);

private:
	void Render(CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer);


private:
	CGfxDescriptorSetPtr m_ptrDescriptorSetPass0;
	CGfxDescriptorSetPtr m_ptrDescriptorSetPass1;
	CGfxRenderTexturePtr m_ptrInputShadowTexture;
	CGfxRenderTexturePtr m_ptrInputSSAOTexture;

private:
	CGfxFrameBufferPtr m_ptrFrameBuffer;
	CGfxRenderTexturePtr m_ptrOutputColorTexture;
	CGfxRenderTexturePtr m_ptrOutputGBufferATexture; // Albedo
	CGfxRenderTexturePtr m_ptrOutputGBufferBTexture; // Normal
	CGfxRenderTexturePtr m_ptrOutputGBufferCTexture; // Roughness Metallic Specular AO
	CGfxRenderTexturePtr m_ptrOutputDepthStencilTexture;
};
