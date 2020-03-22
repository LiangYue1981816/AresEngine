#pragma once
#include "PreHeader.h"


class CALL_API CPassGBuffer : public CPassBase
{
	friend class CCamera;
	friend class CRenderSystem;


private:
	static void Create(GfxPixelFormat colorPixelFormat, GfxPixelFormat depthPixelFormat);
	static void Destroy(void);


private:
	CPassGBuffer(CRenderSystem* pRenderSystem);
	virtual ~CPassGBuffer(void);


public:
	void SetCamera(CCamera* pCamera);
	void SetOutputTexture(CGfxRenderTexturePtr ptrGBufferATexture, CGfxRenderTexturePtr ptrGBufferBTexture, CGfxRenderTexturePtr ptrGBufferCTexture, CGfxRenderTexturePtr ptrDepthStencilTexture);

private:
	void Render(CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer);


private:
	CGfxDescriptorSetPtr m_ptrDescriptorSetPass;

private:
	CGfxFrameBufferPtr m_ptrFrameBuffer;
	CGfxRenderTexturePtr m_ptrOutputGBufferATexture; // Albedo
	CGfxRenderTexturePtr m_ptrOutputGBufferBTexture; // Normal
	CGfxRenderTexturePtr m_ptrOutputGBufferCTexture; // Roughness Metallic Specular AO
	CGfxRenderTexturePtr m_ptrOutputDepthStencilTexture;
};
