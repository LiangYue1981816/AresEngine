#pragma once
#include "PreHeader.h"


class CPassCopyDepthStencil : public CPassBlit
{
	friend class CCamera;
	friend class CRenderSystem;


private:
	static void Create(GfxPixelFormat depthPixelFormat);
	static void Destroy(void);


private:
	CPassCopyDepthStencil(CRenderSystem* pRenderSystem);
	virtual ~CPassCopyDepthStencil(void);


public:
	void SetCamera(CCamera* pCamera);
	void SetInputTexture(CGfxRenderTexturePtr ptrDepthStencilTexture);
	void SetOutputTexture(CGfxRenderTexturePtr ptrDepthStencilTexture);

private:
	void Render(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer);


private:
	CGfxDescriptorSetPtr m_ptrDescriptorSetPass;
	CGfxRenderTexturePtr m_ptrInputDepthStencilTexture;

private:
	CGfxFrameBufferPtr m_ptrFrameBuffer;
	CGfxRenderTexturePtr m_ptrOutputDepthStencilTexture;
};
