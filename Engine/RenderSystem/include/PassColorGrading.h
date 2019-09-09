#pragma once
#include "RenderSystem.h"


class CALL_API CPassColorGrading : public CPassBlit
{
	friend class CCamera;
	friend class CRenderSystem;


private:
	static void Create(GfxPixelFormat colorPixelFormat);
	static void Destroy(void);


private:
	CPassColorGrading(CRenderSystem* pRenderSystem);
	virtual ~CPassColorGrading(void);


public:
	void SetCamera(CCamera* pCamera);
	void SetInputTexture(CGfxRenderTexturePtr ptrColorTexture);
	void SetOutputTexture(CGfxRenderTexturePtr ptrColorTexture);

private:
	void Render(CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer);


private:
	CGfxFrameBufferPtr m_ptrFrameBuffer;
	CGfxRenderTexturePtr m_ptrColorTexture;

private:
	CGfxDescriptorSetPtr m_ptrDescriptorSetPass;
};
