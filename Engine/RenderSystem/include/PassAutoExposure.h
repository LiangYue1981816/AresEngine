#pragma once
#include "PreHeader.h"


class CALL_API CPassAutoExposure : public CPassBlit
{
	friend class CCamera;
	friend class CRenderSystem;


private:
	static void Create(GfxPixelFormat colorPixelFormat);
	static void Destroy(void);


private:
	CPassAutoExposure(CRenderSystem* pRenderSystem);
	virtual ~CPassAutoExposure(void);


public:
	void SetCamera(CCamera* pCamera);
	void SetInputTexture(CGfxRenderTexturePtr ptrColorTexture);
	void SetOutputTexture(CGfxRenderTexturePtr ptrColorTexture);

	void SetParamLower(float lower);
	void SetParamUpper(float upper);
	void SetParamLuminance(float luminance);

private:
	void Render(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer);
	void RenderCallback(CGfxCommandBufferPtr ptrCommandBuffer);


private:
	float m_lower;
	float m_upper;
	float m_luminance;

private:
	CGfxDescriptorSetPtr m_ptrDescriptorSetPass;
	CGfxRenderTexturePtr m_ptrInputColorTexture;

private:
	CGfxFrameBufferPtr m_ptrFrameBuffer;
	CGfxRenderTexturePtr m_ptrOutputColorTexture;
};
