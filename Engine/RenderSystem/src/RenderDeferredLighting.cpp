#include "EngineHeader.h"
#include "RenderHeader.h"
#include "VKRenderer.h"
#include "GLES3Renderer.h"


void CRenderSystem::RenderDeferredLighting(CTaskPool& taskPool, CTaskGraph& taskGraph, CCamera* pCamera, bool bPresent)
{
	m_pInstanceBufferPool->Clear();

	const CGfxSemaphore* pWaitSemaphore = GfxRenderer()->GetSwapChain()->GetAcquireSemaphore();
	const CGfxCommandBufferPtr ptrComputeCommandBuffer = m_ptrComputeCommandBuffer[GfxRenderer()->GetSwapChain()->GetFrameIndex()];
	const CGfxCommandBufferPtr ptrGraphicCommandBuffer = m_ptrGraphicCommandBuffer[GfxRenderer()->GetSwapChain()->GetFrameIndex()];

	GfxRenderer()->AcquireNextFrame();
	{
		GfxRenderer()->BeginRecord(ptrComputeCommandBuffer);
		{
			UpdateGPUScene(taskPool, taskGraph, ptrComputeCommandBuffer);
		}
		GfxRenderer()->EndRecord(ptrComputeCommandBuffer);
		GfxRenderer()->Submit(ptrComputeCommandBuffer, pWaitSemaphore);

		GfxRenderer()->BeginRecord(ptrGraphicCommandBuffer);
		{
			RenderDeferredLighting(taskPool, taskGraph, pCamera, bPresent, ptrGraphicCommandBuffer);
		}
		GfxRenderer()->EndRecord(ptrGraphicCommandBuffer);
		GfxRenderer()->Submit(ptrGraphicCommandBuffer, ptrComputeCommandBuffer->GetSemaphore());
	}
	GfxRenderer()->Present(ptrGraphicCommandBuffer->GetSemaphore());
}

void CRenderSystem::RenderDeferredLighting(CTaskPool& taskPool, CTaskGraph& taskGraph, CCamera* pCamera, bool bPresent, CGfxCommandBufferPtr ptrCommandBuffer)
{
	uint32_t rtFinal;

	uint32_t rtDepth = RENDER_TEXTURE_FULL_DEPTH;
	{
		m_pPassPreZ->SetCamera(pCamera);
		m_pPassPreZ->SetOutputTexture(GetRenderTexture(rtDepth));
		m_pPassPreZ->Render(taskPool, taskGraph, ptrCommandBuffer);
	}

	uint32_t rtDepthCopy = RENDER_TEXTURE_FULL_DEPTH_COPY;
	{
		m_pPassCopyDepthStencil->SetCamera(pCamera);
		m_pPassCopyDepthStencil->SetInputTexture(GetRenderTexture(rtDepth));
		m_pPassCopyDepthStencil->SetOutputTexture(GetRenderTexture(rtDepthCopy));
		m_pPassCopyDepthStencil->Render(taskPool, taskGraph, ptrCommandBuffer);
	}

	uint32_t rtSSAO = RENDER_TEXTURE_FULL_HDR_COLOR0;
	uint32_t rtSSAOBlurHorizontal = RENDER_TEXTURE_FULL_HDR_COLOR1;
	uint32_t rtSSAOBlurVertical = RENDER_TEXTURE_FULL_HDR_COLOR0;
	{
		m_pPassSSAO->SetParamSamples(Settings()->GetValue("RenderSystem.SSAO.SampleCount"));
		m_pPassSSAO->SetParamMinSampleRadius(Settings()->GetValue("RenderSystem.SSAO.MinSampleRadius"));
		m_pPassSSAO->SetParamMaxSampleRadius(Settings()->GetValue("RenderSystem.SSAO.MaxSampleRadius"));
		m_pPassSSAO->SetParamMinDepthRange(Settings()->GetValue("RenderSystem.SSAO.MinDepthRange"));
		m_pPassSSAO->SetParamMaxDepthRange(Settings()->GetValue("RenderSystem.SSAO.MaxDepthRange"));
		m_pPassSSAO->SetCamera(pCamera);
		m_pPassSSAO->SetInputTexture(GetRenderTexture(rtDepth));
		m_pPassSSAO->SetOutputTexture(GetRenderTexture(rtSSAO));
		m_pPassSSAO->Render(taskPool, taskGraph, ptrCommandBuffer);

		m_pPassSSAOBlurHorizontal->SetParamRange(Settings()->GetValue("RenderSystem.SSAO.BlurRange"));
		m_pPassSSAOBlurHorizontal->SetCamera(pCamera);
		m_pPassSSAOBlurHorizontal->SetInputTexture(GetRenderTexture(rtSSAO));
		m_pPassSSAOBlurHorizontal->SetOutputTexture(GetRenderTexture(rtSSAOBlurHorizontal));
		m_pPassSSAOBlurHorizontal->Render(taskPool, taskGraph, ptrCommandBuffer);

		m_pPassSSAOBlurVertical->SetParamRange(Settings()->GetValue("RenderSystem.SSAO.BlurRange"));
		m_pPassSSAOBlurVertical->SetCamera(pCamera);
		m_pPassSSAOBlurVertical->SetInputTexture(GetRenderTexture(rtSSAOBlurHorizontal));
		m_pPassSSAOBlurVertical->SetOutputTexture(GetRenderTexture(rtSSAOBlurVertical));
		m_pPassSSAOBlurVertical->Render(taskPool, taskGraph, ptrCommandBuffer);
	}

	uint32_t rtShadow = RENDER_TEXTURE_SHADOW;
	{
		m_pPassShadow->SetParamFactor(Settings()->GetValue("RenderSystem.Shadow.Factor"));
		m_pPassShadow->SetParamSplitFactors(Settings()->GetValue("RenderSystem.Shadow.SplitFactor0"), Settings()->GetValue("RenderSystem.Shadow.SplitFactor1"), Settings()->GetValue("RenderSystem.Shadow.SplitFactor2"), Settings()->GetValue("RenderSystem.Shadow.SplitFactor3"));
		m_pPassShadow->SetCamera(pCamera);
		m_pPassShadow->SetOutputTexture(GetRenderTexture(rtShadow));
		m_pPassShadow->Render(taskPool, taskGraph, ptrCommandBuffer);
	}

	uint32_t rtColor = RENDER_TEXTURE_FULL_HDR_COLOR1;
	uint32_t rtGBuffer0 = RENDER_TEXTURE_GBUFFER0;
	uint32_t rtGBuffer1 = RENDER_TEXTURE_GBUFFER1;
	{
		m_pPassDeferredLighting->SetCamera(pCamera);
		m_pPassDeferredLighting->SetInputTexture(GetRenderTexture(rtDepthCopy), GetRenderTexture(rtShadow), GetRenderTexture(rtSSAO));
		m_pPassDeferredLighting->SetOutputTexture(GetRenderTexture(rtColor), GetRenderTexture(rtGBuffer0), GetRenderTexture(rtGBuffer1), GetRenderTexture(rtDepth));
		m_pPassDeferredLighting->Render(taskPool, taskGraph, ptrCommandBuffer);
	}
	rtFinal = RENDER_TEXTURE_FULL_HDR_COLOR0;

	uint32_t rtBloomThreshold = RENDER_TEXTURE_QUATER_HDR_COLOR0;
	uint32_t rtBloomBlurHorizontal = RENDER_TEXTURE_QUATER_HDR_COLOR1;
	uint32_t rtBloomBlurVertical = RENDER_TEXTURE_QUATER_HDR_COLOR0;
	uint32_t rtBloom = RENDER_TEXTURE_FULL_HDR_COLOR0;
	{
		m_pPassBloomLuminanceThreshold->SetCamera(pCamera);
		m_pPassBloomLuminanceThreshold->SetInputTexture(GetRenderTexture(rtColor));
		m_pPassBloomLuminanceThreshold->SetOutputTexture(GetRenderTexture(rtBloomThreshold));
		m_pPassBloomLuminanceThreshold->Render(taskPool, taskGraph, ptrCommandBuffer);

		m_pPassBloomBlurHorizontal->SetParamRange(Settings()->GetValue("RenderSystem.Bloom.BlurRange.FirstTime"));
		m_pPassBloomBlurHorizontal->SetCamera(pCamera);
		m_pPassBloomBlurHorizontal->SetInputTexture(GetRenderTexture(rtBloomThreshold));
		m_pPassBloomBlurHorizontal->SetOutputTexture(GetRenderTexture(rtBloomBlurHorizontal));
		m_pPassBloomBlurHorizontal->Render(taskPool, taskGraph, ptrCommandBuffer);

		m_pPassBloomBlurVertical->SetParamRange(Settings()->GetValue("RenderSystem.Bloom.BlurRange.FirstTime"));
		m_pPassBloomBlurVertical->SetCamera(pCamera);
		m_pPassBloomBlurVertical->SetInputTexture(GetRenderTexture(rtBloomBlurHorizontal));
		m_pPassBloomBlurVertical->SetOutputTexture(GetRenderTexture(rtBloomBlurVertical));
		m_pPassBloomBlurVertical->Render(taskPool, taskGraph, ptrCommandBuffer);

		m_pPassBloomBlurHorizontal->SetParamRange(Settings()->GetValue("RenderSystem.Bloom.BlurRange.SecondTime"));
		m_pPassBloomBlurHorizontal->SetCamera(pCamera);
		m_pPassBloomBlurHorizontal->SetInputTexture(GetRenderTexture(rtBloomBlurVertical));
		m_pPassBloomBlurHorizontal->SetOutputTexture(GetRenderTexture(rtBloomBlurHorizontal));
		m_pPassBloomBlurHorizontal->Render(taskPool, taskGraph, ptrCommandBuffer);

		m_pPassBloomBlurVertical->SetParamRange(Settings()->GetValue("RenderSystem.Bloom.BlurRange.SecondTime"));
		m_pPassBloomBlurVertical->SetCamera(pCamera);
		m_pPassBloomBlurVertical->SetInputTexture(GetRenderTexture(rtBloomBlurHorizontal));
		m_pPassBloomBlurVertical->SetOutputTexture(GetRenderTexture(rtBloomBlurVertical));
		m_pPassBloomBlurVertical->Render(taskPool, taskGraph, ptrCommandBuffer);

		m_pPassBloomBlendAdd->SetCamera(pCamera);
		m_pPassBloomBlendAdd->SetInputTexture(GetRenderTexture(rtColor), GetRenderTexture(rtBloomBlurVertical));
		m_pPassBloomBlendAdd->SetOutputTexture(GetRenderTexture(rtBloom));
		m_pPassBloomBlendAdd->Render(taskPool, taskGraph, ptrCommandBuffer);
	}
	rtColor = rtBloom;
	rtFinal = RENDER_TEXTURE_FULL_HDR_COLOR1;

	m_pPassColorGrading->SetCamera(pCamera);
	m_pPassColorGrading->SetInputTexture(GetRenderTexture(rtColor));
	m_pPassColorGrading->SetOutputTexture(GetRenderTexture(rtFinal));
	m_pPassColorGrading->Render(taskPool, taskGraph, ptrCommandBuffer);

	m_pPassFinal->SetCamera(pCamera);
	m_pPassFinal->SetInputTexture(GetRenderTexture(rtFinal));
	m_pPassFinal->SetOutputTexture(GfxRenderer()->GetSwapChain()->GetFrameIndex(), GetRenderTexture(GfxRenderer()->GetSwapChain()->GetFrameIndex()));
	m_pPassFinal->Render(taskPool, taskGraph, ptrCommandBuffer, GfxRenderer()->GetSwapChain()->GetFrameIndex(), bPresent);
}

void CRenderSystem::RenderTileBaseDeferredLighting(CTaskPool& taskPool, CTaskGraph& taskGraph, CCamera* pCamera, bool bPresent)
{
	m_pInstanceBufferPool->Clear();

	const CGfxSemaphore* pWaitSemaphore = GfxRenderer()->GetSwapChain()->GetAcquireSemaphore();
	const CGfxCommandBufferPtr ptrComputeCommandBuffer = m_ptrComputeCommandBuffer[GfxRenderer()->GetSwapChain()->GetFrameIndex()];
	const CGfxCommandBufferPtr ptrGraphicCommandBuffer = m_ptrGraphicCommandBuffer[GfxRenderer()->GetSwapChain()->GetFrameIndex()];

	GfxRenderer()->AcquireNextFrame();
	{
		GfxRenderer()->BeginRecord(ptrComputeCommandBuffer);
		{
			UpdateGPUScene(taskPool, taskGraph, ptrComputeCommandBuffer);
		}
		GfxRenderer()->EndRecord(ptrComputeCommandBuffer);
		GfxRenderer()->Submit(ptrComputeCommandBuffer, pWaitSemaphore);

		GfxRenderer()->BeginRecord(ptrGraphicCommandBuffer);
		{
			RenderDeferredLighting(taskPool, taskGraph, pCamera, bPresent, ptrGraphicCommandBuffer);
		}
		GfxRenderer()->EndRecord(ptrGraphicCommandBuffer);
		GfxRenderer()->Submit(ptrGraphicCommandBuffer, ptrComputeCommandBuffer->GetSemaphore());
	}
	GfxRenderer()->Present(ptrGraphicCommandBuffer->GetSemaphore());
}

void CRenderSystem::RenderTileBaseDeferredLighting(CTaskPool& taskPool, CTaskGraph& taskGraph, CCamera* pCamera, bool bPresent, CGfxCommandBufferPtr ptrCommandBuffer)
{

}
