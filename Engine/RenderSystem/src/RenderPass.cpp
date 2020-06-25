#include "EngineHeader.h"
#include "RenderHeader.h"
#include "VKRenderer.h"
#include "GLES3Renderer.h"


void CRenderSystem::InternalPassCopyColor(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer, CCamera* pCamera, uint32_t rtInColor, uint32_t rtOutColor)
{
	m_pPassCopyColor->SetCamera(pCamera);
	m_pPassCopyColor->SetInputTexture(m_ptrRenderTextures[rtInColor]);
	m_pPassCopyColor->SetOutputTexture(m_ptrRenderTextures[rtOutColor]);
	m_pPassCopyColor->Render(taskPool, taskGraph, ptrCommandBuffer);
}

void CRenderSystem::InternalPassCopyDepthStencil(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer, CCamera* pCamera, uint32_t rtInDepth, uint32_t rtOutDepth)
{
	m_pPassCopyDepthStencil->SetCamera(pCamera);
	m_pPassCopyDepthStencil->SetInputTexture(m_ptrRenderTextures[rtInDepth]);
	m_pPassCopyDepthStencil->SetOutputTexture(m_ptrRenderTextures[rtOutDepth]);
	m_pPassCopyDepthStencil->Render(taskPool, taskGraph, ptrCommandBuffer);
}

void CRenderSystem::InternalPassPreZ(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer, CCamera* pCamera, uint32_t rtOutDepth)
{
	m_pPassPreZ->SetCamera(pCamera);
	m_pPassPreZ->SetOutputTexture(m_ptrRenderTextures[rtOutDepth]);
	m_pPassPreZ->Render(taskPool, taskGraph, ptrCommandBuffer);
}

void CRenderSystem::InternalPassSSAO(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer, CCamera* pCamera, uint32_t rtInDepth, uint32_t rtOutSSAO, uint32_t rtTempBlur)
{
	m_pPassSSAO->SetCamera(pCamera);
	m_pPassSSAO->SetInputTexture(m_ptrRenderTextures[rtInDepth]);
	m_pPassSSAO->SetOutputTexture(m_ptrRenderTextures[rtOutSSAO]);
	m_pPassSSAO->SetParamSamples(Settings()->GetValue("RenderSystem.SSAO.SampleCount"));
	m_pPassSSAO->SetParamMinSampleRadius(Settings()->GetValue("RenderSystem.SSAO.MinSampleRadius"));
	m_pPassSSAO->SetParamMaxSampleRadius(Settings()->GetValue("RenderSystem.SSAO.MaxSampleRadius"));
	m_pPassSSAO->SetParamMinDepthRange(Settings()->GetValue("RenderSystem.SSAO.MinDepthRange"));
	m_pPassSSAO->SetParamMaxDepthRange(Settings()->GetValue("RenderSystem.SSAO.MaxDepthRange"));
	m_pPassSSAO->Render(taskPool, taskGraph, ptrCommandBuffer);

	m_pPassSSAOBlurHorizontal->SetCamera(pCamera);
	m_pPassSSAOBlurHorizontal->SetInputTexture(m_ptrRenderTextures[rtOutSSAO]);
	m_pPassSSAOBlurHorizontal->SetOutputTexture(m_ptrRenderTextures[rtTempBlur]);
	m_pPassSSAOBlurHorizontal->SetParamRange(Settings()->GetValue("RenderSystem.SSAO.BlurRange"));
	m_pPassSSAOBlurHorizontal->Render(taskPool, taskGraph, ptrCommandBuffer);

	m_pPassSSAOBlurVertical->SetCamera(pCamera);
	m_pPassSSAOBlurVertical->SetInputTexture(m_ptrRenderTextures[rtTempBlur]);
	m_pPassSSAOBlurVertical->SetOutputTexture(m_ptrRenderTextures[rtOutSSAO]);
	m_pPassSSAOBlurVertical->SetParamRange(Settings()->GetValue("RenderSystem.SSAO.BlurRange"));
	m_pPassSSAOBlurVertical->Render(taskPool, taskGraph, ptrCommandBuffer);
}

void CRenderSystem::InternalPassShadow(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer, CCamera* pCamera, uint32_t rtOutShadow)
{
	m_pPassShadow->SetCamera(pCamera);
	m_pPassShadow->SetOutputTexture(m_ptrRenderTextures[rtOutShadow]);
	m_pPassShadow->SetParamFactor(Settings()->GetValue("RenderSystem.Shadow.Factor"));
	m_pPassShadow->SetParamSplitFactors(Settings()->GetValue("RenderSystem.Shadow.SplitFactor0"), Settings()->GetValue("RenderSystem.Shadow.SplitFactor1"), Settings()->GetValue("RenderSystem.Shadow.SplitFactor2"), Settings()->GetValue("RenderSystem.Shadow.SplitFactor3"));
	m_pPassShadow->Render(taskPool, taskGraph, ptrCommandBuffer);
}

void CRenderSystem::InternalPassUnlit(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer, CCamera* pCamera, uint32_t rtOutColor, uint32_t rtOutDepth)
{
	m_pPassUnlit->SetCamera(pCamera);
	m_pPassUnlit->SetOutputTexture(m_ptrRenderTextures[rtOutColor], m_ptrRenderTextures[rtOutDepth]);
	m_pPassUnlit->Render(taskPool, taskGraph, ptrCommandBuffer);
}

void CRenderSystem::InternalPassForwardLighting(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer, CCamera* pCamera, uint32_t rtInShadow, uint32_t rtInSSAO, uint32_t rtOutColor, uint32_t rtOutDepth)
{
	m_pPassForwardShading->SetCamera(pCamera);
	m_pPassForwardShading->SetInputTexture(m_ptrRenderTextures[rtInShadow], m_ptrRenderTextures[rtInSSAO]);
	m_pPassForwardShading->SetOutputTexture(m_ptrRenderTextures[rtOutColor], m_ptrRenderTextures[rtOutDepth]);
	m_pPassForwardShading->Render(taskPool, taskGraph, ptrCommandBuffer);
}

void CRenderSystem::InternalPassDeferredLighting(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer, CCamera* pCamera, uint32_t rtInDepth, uint32_t rtInShadow, uint32_t rtInSSAO, uint32_t rtOutColor, uint32_t rtOutDepth, uint32_t rtOutGBuffer0, uint32_t rtOutGBuffer1)
{
	m_pPassDeferredShading->SetCamera(pCamera);
	m_pPassDeferredShading->SetInputTexture(m_ptrRenderTextures[rtInDepth], m_ptrRenderTextures[rtInShadow], m_ptrRenderTextures[rtInSSAO]);
	m_pPassDeferredShading->SetOutputTexture(m_ptrRenderTextures[rtOutColor], m_ptrRenderTextures[rtOutGBuffer0], m_ptrRenderTextures[rtOutGBuffer1], m_ptrRenderTextures[rtOutDepth]);
	m_pPassDeferredShading->Render(taskPool, taskGraph, ptrCommandBuffer);
}

void CRenderSystem::InternalPassTileDeferredLighting(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer, CCamera* pCamera, uint32_t rtInDepth, uint32_t rtInShadow, uint32_t rtInSSAO, uint32_t rtOutColor, uint32_t rtOutDepth, uint32_t rtOutGBuffer0, uint32_t rtOutGBuffer1)
{
	m_pPassTileDeferredShading->SetCamera(pCamera);
	m_pPassTileDeferredShading->SetInputTexture(m_ptrRenderTextures[rtInDepth], m_ptrRenderTextures[rtInShadow], m_ptrRenderTextures[rtInSSAO]);
	m_pPassTileDeferredShading->SetOutputTexture(m_ptrRenderTextures[rtOutColor], m_ptrRenderTextures[rtOutGBuffer0], m_ptrRenderTextures[rtOutGBuffer1], m_ptrRenderTextures[rtOutDepth]);
	m_pPassTileDeferredShading->Render(taskPool, taskGraph, ptrCommandBuffer);
}

void CRenderSystem::InternalPassBloom(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer, CCamera* pCamera, uint32_t rtInColor, uint32_t rtOutColor, uint32_t rtTempThreshold, uint32_t rtTempBlur0, uint32_t rtTempBlur1)
{
	m_pPassBloomLuminanceThreshold->SetCamera(pCamera);
	m_pPassBloomLuminanceThreshold->SetInputTexture(m_ptrRenderTextures[rtInColor]);
	m_pPassBloomLuminanceThreshold->SetOutputTexture(m_ptrRenderTextures[rtTempThreshold]);
	m_pPassBloomLuminanceThreshold->Render(taskPool, taskGraph, ptrCommandBuffer);

	m_pPassBloomBlurHorizontal->SetCamera(pCamera);
	m_pPassBloomBlurHorizontal->SetInputTexture(m_ptrRenderTextures[rtTempThreshold]);
	m_pPassBloomBlurHorizontal->SetOutputTexture(m_ptrRenderTextures[rtTempBlur0]);
	m_pPassBloomBlurHorizontal->SetParamRange(Settings()->GetValue("RenderSystem.Bloom.BlurRange.FirstTime"));
	m_pPassBloomBlurHorizontal->Render(taskPool, taskGraph, ptrCommandBuffer);

	m_pPassBloomBlurVertical->SetCamera(pCamera);
	m_pPassBloomBlurVertical->SetInputTexture(m_ptrRenderTextures[rtTempBlur0]);
	m_pPassBloomBlurVertical->SetOutputTexture(m_ptrRenderTextures[rtTempBlur1]);
	m_pPassBloomBlurVertical->SetParamRange(Settings()->GetValue("RenderSystem.Bloom.BlurRange.FirstTime"));
	m_pPassBloomBlurVertical->Render(taskPool, taskGraph, ptrCommandBuffer);

	m_pPassBloomBlurHorizontal->SetCamera(pCamera);
	m_pPassBloomBlurHorizontal->SetInputTexture(m_ptrRenderTextures[rtTempBlur1]);
	m_pPassBloomBlurHorizontal->SetOutputTexture(m_ptrRenderTextures[rtTempBlur0]);
	m_pPassBloomBlurHorizontal->SetParamRange(Settings()->GetValue("RenderSystem.Bloom.BlurRange.SecondTime"));
	m_pPassBloomBlurHorizontal->Render(taskPool, taskGraph, ptrCommandBuffer);

	m_pPassBloomBlurVertical->SetCamera(pCamera);
	m_pPassBloomBlurVertical->SetInputTexture(m_ptrRenderTextures[rtTempBlur0]);
	m_pPassBloomBlurVertical->SetOutputTexture(m_ptrRenderTextures[rtTempBlur1]);
	m_pPassBloomBlurVertical->SetParamRange(Settings()->GetValue("RenderSystem.Bloom.BlurRange.SecondTime"));
	m_pPassBloomBlurVertical->Render(taskPool, taskGraph, ptrCommandBuffer);

	m_pPassBloomBlendAdd->SetCamera(pCamera);
	m_pPassBloomBlendAdd->SetInputTexture(m_ptrRenderTextures[rtInColor], m_ptrRenderTextures[rtTempBlur1]);
	m_pPassBloomBlendAdd->SetOutputTexture(m_ptrRenderTextures[rtOutColor]);
	m_pPassBloomBlendAdd->Render(taskPool, taskGraph, ptrCommandBuffer);
}

void CRenderSystem::InternalPassEyeAdaptation(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer, CCamera* pCamera, uint32_t rtInColor, uint32_t rtOutColor, uint32_t rtTempBlur0, uint32_t rtTempBlur1)
{
	m_pPassBloomBlurHorizontal->SetCamera(pCamera);
	m_pPassBloomBlurHorizontal->SetInputTexture(m_ptrRenderTextures[rtInColor]);
	m_pPassBloomBlurHorizontal->SetOutputTexture(m_ptrRenderTextures[rtTempBlur0]);
	m_pPassBloomBlurHorizontal->SetParamRange(Settings()->GetValue("RenderSystem.Bloom.BlurRange.FirstTime"));
	m_pPassBloomBlurHorizontal->Render(taskPool, taskGraph, ptrCommandBuffer);

	m_pPassBloomBlurVertical->SetCamera(pCamera);
	m_pPassBloomBlurVertical->SetInputTexture(m_ptrRenderTextures[rtTempBlur0]);
	m_pPassBloomBlurVertical->SetOutputTexture(m_ptrRenderTextures[rtTempBlur1]);
	m_pPassBloomBlurVertical->SetParamRange(Settings()->GetValue("RenderSystem.Bloom.BlurRange.FirstTime"));
	m_pPassBloomBlurVertical->Render(taskPool, taskGraph, ptrCommandBuffer);

	m_pPassCopyColor->SetCamera(pCamera);
	m_pPassCopyColor->SetInputTexture(m_ptrRenderTextures[rtTempBlur1]);
	m_pPassCopyColor->SetOutputTexture(m_ptrRenderTextures[rtOutColor]);
	m_pPassCopyColor->Render(taskPool, taskGraph, ptrCommandBuffer);
}

void CRenderSystem::InternalPassColorGrading(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer, CCamera* pCamera, uint32_t rtInColor, uint32_t rtOutColor)
{
	m_pPassColorGrading->SetCamera(pCamera);
	m_pPassColorGrading->SetInputTexture(m_ptrRenderTextures[rtInColor]);
	m_pPassColorGrading->SetOutputTexture(m_ptrRenderTextures[rtOutColor]);
	m_pPassColorGrading->Render(taskPool, taskGraph, ptrCommandBuffer);
}

void CRenderSystem::InternalPassFinal(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer, CCamera* pCamera, uint32_t rtInColor, bool bPresent)
{
	m_pPassFinal->SetCamera(pCamera);
	m_pPassFinal->SetInputTexture(m_ptrRenderTextures[rtInColor]);
	m_pPassFinal->SetOutputTexture(GfxRenderer()->GetSwapChain()->GetFrameIndex(), m_ptrRenderTextures[GfxRenderer()->GetSwapChain()->GetFrameIndex()]);
	m_pPassFinal->Render(taskPool, taskGraph, ptrCommandBuffer, GfxRenderer()->GetSwapChain()->GetFrameIndex(), bPresent);
}
