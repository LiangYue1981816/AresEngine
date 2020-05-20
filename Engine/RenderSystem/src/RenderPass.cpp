#include "EngineHeader.h"
#include "RenderHeader.h"
#include "VKRenderer.h"
#include "GLES3Renderer.h"


void CRenderSystem::InternalPassCopyColor(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer, CCamera* pCamera, uint32_t rtInColor, uint32_t rtOutColor)
{
	m_pPassCopyColor->SetCamera(pCamera);
	m_pPassCopyColor->SetInputTexture(GetRenderTexture(rtInColor));
	m_pPassCopyColor->SetOutputTexture(GetRenderTexture(rtOutColor));
	m_pPassCopyColor->Render(taskPool, taskGraph, ptrCommandBuffer);
}

void CRenderSystem::InternalPassCopyDepthStencil(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer, CCamera* pCamera, uint32_t rtInDepth, uint32_t rtOutDepth)
{
	m_pPassCopyDepthStencil->SetCamera(pCamera);
	m_pPassCopyDepthStencil->SetInputTexture(GetRenderTexture(rtInDepth));
	m_pPassCopyDepthStencil->SetOutputTexture(GetRenderTexture(rtOutDepth));
	m_pPassCopyDepthStencil->Render(taskPool, taskGraph, ptrCommandBuffer);
}

void CRenderSystem::InternalPassPreZ(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer, CCamera* pCamera, uint32_t rtOutDepth)
{
	m_pPassPreZ->SetCamera(pCamera);
	m_pPassPreZ->SetOutputTexture(GetRenderTexture(rtOutDepth));
	m_pPassPreZ->Render(taskPool, taskGraph, ptrCommandBuffer);
}

void CRenderSystem::InternalPassSSAO(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer, CCamera* pCamera, uint32_t rtInDepth, uint32_t rtOutSSAO, uint32_t rtTempBlur)
{
	m_pPassSSAO->SetCamera(pCamera);
	m_pPassSSAO->SetInputTexture(GetRenderTexture(rtInDepth));
	m_pPassSSAO->SetOutputTexture(GetRenderTexture(rtOutSSAO));
	m_pPassSSAO->SetParamSamples(Settings()->GetValue("RenderSystem.SSAO.SampleCount"));
	m_pPassSSAO->SetParamMinSampleRadius(Settings()->GetValue("RenderSystem.SSAO.MinSampleRadius"));
	m_pPassSSAO->SetParamMaxSampleRadius(Settings()->GetValue("RenderSystem.SSAO.MaxSampleRadius"));
	m_pPassSSAO->SetParamMinDepthRange(Settings()->GetValue("RenderSystem.SSAO.MinDepthRange"));
	m_pPassSSAO->SetParamMaxDepthRange(Settings()->GetValue("RenderSystem.SSAO.MaxDepthRange"));
	m_pPassSSAO->Render(taskPool, taskGraph, ptrCommandBuffer);

	m_pPassSSAOBlurHorizontal->SetCamera(pCamera);
	m_pPassSSAOBlurHorizontal->SetInputTexture(GetRenderTexture(rtOutSSAO));
	m_pPassSSAOBlurHorizontal->SetOutputTexture(GetRenderTexture(rtTempBlur));
	m_pPassSSAOBlurHorizontal->SetParamRange(Settings()->GetValue("RenderSystem.SSAO.BlurRange"));
	m_pPassSSAOBlurHorizontal->Render(taskPool, taskGraph, ptrCommandBuffer);

	m_pPassSSAOBlurVertical->SetCamera(pCamera);
	m_pPassSSAOBlurVertical->SetInputTexture(GetRenderTexture(rtTempBlur));
	m_pPassSSAOBlurVertical->SetOutputTexture(GetRenderTexture(rtOutSSAO));
	m_pPassSSAOBlurVertical->SetParamRange(Settings()->GetValue("RenderSystem.SSAO.BlurRange"));
	m_pPassSSAOBlurVertical->Render(taskPool, taskGraph, ptrCommandBuffer);
}

void CRenderSystem::InternalPassShadow(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer, CCamera* pCamera, uint32_t rtOutShadow)
{
	m_pPassShadow->SetCamera(pCamera);
	m_pPassShadow->SetOutputTexture(GetRenderTexture(rtOutShadow));
	m_pPassShadow->SetParamFactor(Settings()->GetValue("RenderSystem.Shadow.Factor"));
	m_pPassShadow->SetParamSplitFactors(Settings()->GetValue("RenderSystem.Shadow.SplitFactor0"), Settings()->GetValue("RenderSystem.Shadow.SplitFactor1"), Settings()->GetValue("RenderSystem.Shadow.SplitFactor2"), Settings()->GetValue("RenderSystem.Shadow.SplitFactor3"));
	m_pPassShadow->Render(taskPool, taskGraph, ptrCommandBuffer);
}

void CRenderSystem::InternalPassForwardLighting(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer, CCamera* pCamera, uint32_t rtInShadow, uint32_t rtInSSAO, uint32_t rtOutColor, uint32_t rtOutDepth)
{
	m_pPassForwardShading->SetCamera(pCamera);
	m_pPassForwardShading->SetInputTexture(GetRenderTexture(rtInShadow), GetRenderTexture(rtInSSAO));
	m_pPassForwardShading->SetOutputTexture(GetRenderTexture(rtOutColor), GetRenderTexture(rtOutDepth));
	m_pPassForwardShading->Render(taskPool, taskGraph, ptrCommandBuffer);
}

void CRenderSystem::InternalPassDeferredLighting(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer, CCamera* pCamera, uint32_t rtInDepth, uint32_t rtInShadow, uint32_t rtInSSAO, uint32_t rtOutColor, uint32_t rtOutDepth, uint32_t rtOutGBuffer0, uint32_t rtOutGBuffer1)
{
	m_pPassDeferredShading->SetCamera(pCamera);
	m_pPassDeferredShading->SetInputTexture(GetRenderTexture(rtInDepth), GetRenderTexture(rtInShadow), GetRenderTexture(rtInSSAO));
	m_pPassDeferredShading->SetOutputTexture(GetRenderTexture(rtOutColor), GetRenderTexture(rtOutGBuffer0), GetRenderTexture(rtOutGBuffer1), GetRenderTexture(rtOutDepth));
	m_pPassDeferredShading->Render(taskPool, taskGraph, ptrCommandBuffer);
}

void CRenderSystem::InternalPassTileDeferredLighting(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer, CCamera* pCamera, uint32_t rtInDepth, uint32_t rtInShadow, uint32_t rtInSSAO, uint32_t rtOutColor, uint32_t rtOutDepth, uint32_t rtOutGBuffer0, uint32_t rtOutGBuffer1)
{
	m_pPassTileDeferredShading->SetCamera(pCamera);
	m_pPassTileDeferredShading->SetInputTexture(GetRenderTexture(rtInDepth), GetRenderTexture(rtInShadow), GetRenderTexture(rtInSSAO));
	m_pPassTileDeferredShading->SetOutputTexture(GetRenderTexture(rtOutColor), GetRenderTexture(rtOutGBuffer0), GetRenderTexture(rtOutGBuffer1), GetRenderTexture(rtOutDepth));
	m_pPassTileDeferredShading->Render(taskPool, taskGraph, ptrCommandBuffer);
}

void CRenderSystem::InternalPassBloom(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer, CCamera* pCamera, uint32_t rtInColor, uint32_t rtOutBloom, uint32_t rtTempThreshold, uint32_t rtTempBlur0, uint32_t rtTempBlur1)
{
	m_pPassBloomLuminanceThreshold->SetCamera(pCamera);
	m_pPassBloomLuminanceThreshold->SetInputTexture(GetRenderTexture(rtInColor));
	m_pPassBloomLuminanceThreshold->SetOutputTexture(GetRenderTexture(rtTempThreshold));
	m_pPassBloomLuminanceThreshold->Render(taskPool, taskGraph, ptrCommandBuffer);

	m_pPassBloomBlurHorizontal->SetCamera(pCamera);
	m_pPassBloomBlurHorizontal->SetInputTexture(GetRenderTexture(rtTempThreshold));
	m_pPassBloomBlurHorizontal->SetOutputTexture(GetRenderTexture(rtTempBlur0));
	m_pPassBloomBlurHorizontal->SetParamRange(Settings()->GetValue("RenderSystem.Bloom.BlurRange.FirstTime"));
	m_pPassBloomBlurHorizontal->Render(taskPool, taskGraph, ptrCommandBuffer);

	m_pPassBloomBlurVertical->SetCamera(pCamera);
	m_pPassBloomBlurVertical->SetInputTexture(GetRenderTexture(rtTempBlur0));
	m_pPassBloomBlurVertical->SetOutputTexture(GetRenderTexture(rtTempBlur1));
	m_pPassBloomBlurVertical->SetParamRange(Settings()->GetValue("RenderSystem.Bloom.BlurRange.FirstTime"));
	m_pPassBloomBlurVertical->Render(taskPool, taskGraph, ptrCommandBuffer);

	m_pPassBloomBlurHorizontal->SetCamera(pCamera);
	m_pPassBloomBlurHorizontal->SetInputTexture(GetRenderTexture(rtTempBlur1));
	m_pPassBloomBlurHorizontal->SetOutputTexture(GetRenderTexture(rtTempBlur0));
	m_pPassBloomBlurHorizontal->SetParamRange(Settings()->GetValue("RenderSystem.Bloom.BlurRange.SecondTime"));
	m_pPassBloomBlurHorizontal->Render(taskPool, taskGraph, ptrCommandBuffer);

	m_pPassBloomBlurVertical->SetCamera(pCamera);
	m_pPassBloomBlurVertical->SetInputTexture(GetRenderTexture(rtTempBlur0));
	m_pPassBloomBlurVertical->SetOutputTexture(GetRenderTexture(rtTempBlur1));
	m_pPassBloomBlurVertical->SetParamRange(Settings()->GetValue("RenderSystem.Bloom.BlurRange.SecondTime"));
	m_pPassBloomBlurVertical->Render(taskPool, taskGraph, ptrCommandBuffer);

	m_pPassBloomBlendAdd->SetCamera(pCamera);
	m_pPassBloomBlendAdd->SetInputTexture(GetRenderTexture(rtInColor), GetRenderTexture(rtTempBlur1));
	m_pPassBloomBlendAdd->SetOutputTexture(GetRenderTexture(rtOutBloom));
	m_pPassBloomBlendAdd->Render(taskPool, taskGraph, ptrCommandBuffer);
}

void CRenderSystem::InternalPassColorGrading(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer, CCamera* pCamera, uint32_t rtInColor, uint32_t rtOutColor)
{
	m_pPassColorGrading->SetCamera(pCamera);
	m_pPassColorGrading->SetInputTexture(GetRenderTexture(rtInColor));
	m_pPassColorGrading->SetOutputTexture(GetRenderTexture(rtOutColor));
	m_pPassColorGrading->Render(taskPool, taskGraph, ptrCommandBuffer);
}

void CRenderSystem::InternalPassFinal(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer, CCamera* pCamera, uint32_t rtInColor, bool bPresent)
{
	m_pPassFinal->SetCamera(pCamera);
	m_pPassFinal->SetInputTexture(GetRenderTexture(rtInColor));
	m_pPassFinal->SetOutputTexture(GfxRenderer()->GetSwapChain()->GetFrameIndex(), GetRenderTexture(GfxRenderer()->GetSwapChain()->GetFrameIndex()));
	m_pPassFinal->Render(taskPool, taskGraph, ptrCommandBuffer, GfxRenderer()->GetSwapChain()->GetFrameIndex(), bPresent);
}
