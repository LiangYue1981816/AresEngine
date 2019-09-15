#pragma once
#include "PreHeader.h"


class CALL_API CPassBlendAdd : public CPassBlit
{
	friend class CCamera;
	friend class CRenderSystem;


private:
	static void Create(GfxPixelFormat colorPixelFormat);
	static void Destroy(void);


private:
	CPassBlendAdd(CRenderSystem* pRenderSystem);
	virtual ~CPassBlendAdd(void);


public:
	void SetCamera(CCamera* pCamera);
	void SetInputTexture(CGfxRenderTexturePtr ptrColorTexture, CGfxRenderTexturePtr ptrAddTexture);
	void SetOutputTexture(CGfxRenderTexturePtr ptrColorTexture);

private:
	void Render(CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer);


private:
	CGfxDescriptorSetPtr m_ptrDescriptorSetPass;
	CGfxRenderTexturePtr m_ptrInputColorTexture;
	CGfxRenderTexturePtr m_ptrInputAddTexture;

private:
	CGfxFrameBufferPtr m_ptrFrameBuffer;
	CGfxRenderTexturePtr m_ptrOutputColorTexture;
};
