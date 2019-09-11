#pragma once
#include "PreHeader.h"


class CALL_API CPassPreZ
{
	friend class CCamera;
	friend class CRenderSystem;


private:
	static void Create(GfxPixelFormat depthPixelFormat);
	static void Destroy(void);


private:
	CPassPreZ(CRenderSystem* pRenderSystem);
	virtual ~CPassPreZ(void);


public:
	void SetCamera(CCamera* pCamera);
	void SetOutputTexture(CGfxRenderTexturePtr ptrDepthTexture);

private:
	void Render(CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer);


private:
	CGfxFrameBufferPtr m_ptrFrameBuffer;
	CGfxRenderTexturePtr m_ptrDepthTexture;

private:
	CGfxDescriptorSetPtr m_ptrDescriptorSetPass;

private:
	CCamera* m_pCamera;
	CRenderSystem* m_pRenderSystem;
};
