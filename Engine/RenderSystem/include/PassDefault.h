#pragma once
#include "PreHeader.h"


class CALL_API CPassDefault : public CPassBase
{
	friend class CCamera;
	friend class CRenderSystem;


private:
	static void Create(GfxPixelFormat colorPixelFormat, GfxPixelFormat depthPixelFormat);
	static void Destroy(void);


private:
	CPassDefault(CRenderSystem* pRenderSystem);
	virtual ~CPassDefault(void);


public:
	void SetCamera(CCamera* pCamera);
	void SetOutputTexture(CGfxRenderTexturePtr ptrColorTexture, CGfxRenderTexturePtr ptrDepthStencilTexture);

private:
	void Render(CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer);


private:
	CGfxDescriptorSetPtr m_ptrDescriptorSetPass;

private:
	CGfxFrameBufferPtr m_ptrFrameBuffer;
	CGfxRenderTexturePtr m_ptrOutputColorTexture;
	CGfxRenderTexturePtr m_ptrOutputDepthStencilTexture;
};
