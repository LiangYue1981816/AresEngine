#include "EngineHeader.h"
#include "RenderHeader.h"
#include "VKRenderer.h"
#include "GLES3Renderer.h"


void CRenderSystem::RenderForwardLighting(CTaskGraph& taskGraph, CCamera* pCamera, bool bPresent)
{
	m_pInstanceBufferPool->Clear();

	const CGfxSemaphore* pWaitSemaphore = GfxRenderer()->GetSwapChain()->GetAcquireSemaphore();
	const CGfxCommandBufferPtr ptrComputeCommandBuffer = m_ptrComputeCommandBuffer[GfxRenderer()->GetSwapChain()->GetFrameIndex()];
	const CGfxCommandBufferPtr ptrGraphicCommandBuffer = m_ptrGraphicCommandBuffer[GfxRenderer()->GetSwapChain()->GetFrameIndex()];

	GfxRenderer()->AcquireNextFrame();
	{
		GfxRenderer()->BeginRecord(ptrComputeCommandBuffer);
		{
			UpdateGPUScene(taskGraph, ptrComputeCommandBuffer);
		}
		GfxRenderer()->EndRecord(ptrComputeCommandBuffer);
		GfxRenderer()->Submit(ptrComputeCommandBuffer, pWaitSemaphore);

		GfxRenderer()->BeginRecord(ptrGraphicCommandBuffer);
		{
			RenderForwardLighting(taskGraph, pCamera, bPresent, ptrGraphicCommandBuffer);
		}
		GfxRenderer()->EndRecord(ptrGraphicCommandBuffer);
		GfxRenderer()->Submit(ptrGraphicCommandBuffer, ptrComputeCommandBuffer->GetSemaphore());
	}
	GfxRenderer()->Present(ptrGraphicCommandBuffer->GetSemaphore());
}

void CRenderSystem::RenderForwardLighting(CTaskGraph& taskGraph, CCamera* pCamera, bool bPresent, CGfxCommandBufferPtr ptrCommandBuffer)
{
	uint32_t rtFinal;

	uint32_t rtDepth = RENDER_TEXTURE_FULL_DEPTH;
	{
		m_pPassPreZ->SetCamera(pCamera);
		m_pPassPreZ->SetOutputTexture(GetRenderTexture(rtDepth));
		m_pPassPreZ->Render(taskGraph, ptrCommandBuffer);
	}

	uint32_t rtSSAO = RENDER_TEXTURE_FULL_HDR_COLOR0;
	uint32_t rtSSAOBlurHorizontal = RENDER_TEXTURE_FULL_HDR_COLOR1;
	uint32_t rtSSAOBlurVertical = RENDER_TEXTURE_FULL_HDR_COLOR0;
	{
		m_pPassSSAO->SetParamSamples(Settings()->GetValue("RenderSystem.SSAO.SampleCount"));
		m_pPassSSAO->SetParamMinRadius(Settings()->GetValue("RenderSystem.SSAO.SampleMinRadius"));
		m_pPassSSAO->SetParamMaxRadius(Settings()->GetValue("RenderSystem.SSAO.SampleMaxRadius"));
		m_pPassSSAO->SetCamera(pCamera);
		m_pPassSSAO->SetInputTexture(GetRenderTexture(rtDepth));
		m_pPassSSAO->SetOutputTexture(GetRenderTexture(rtSSAO));
		m_pPassSSAO->Render(taskGraph, ptrCommandBuffer);

		m_pPassSSAOBlurHorizontal->SetParamRange(Settings()->GetValue("RenderSystem.SSAO.BlurRange"));
		m_pPassSSAOBlurHorizontal->SetCamera(pCamera);
		m_pPassSSAOBlurHorizontal->SetInputTexture(GetRenderTexture(rtSSAO));
		m_pPassSSAOBlurHorizontal->SetOutputTexture(GetRenderTexture(rtSSAOBlurHorizontal));
		m_pPassSSAOBlurHorizontal->Render(taskGraph, ptrCommandBuffer);

		m_pPassSSAOBlurVertical->SetParamRange(Settings()->GetValue("RenderSystem.SSAO.BlurRange"));
		m_pPassSSAOBlurVertical->SetCamera(pCamera);
		m_pPassSSAOBlurVertical->SetInputTexture(GetRenderTexture(rtSSAOBlurHorizontal));
		m_pPassSSAOBlurVertical->SetOutputTexture(GetRenderTexture(rtSSAOBlurVertical));
		m_pPassSSAOBlurVertical->Render(taskGraph, ptrCommandBuffer);
	}

	uint32_t rtShadow = RENDER_TEXTURE_SHADOW;
	{
		m_pPassShadow->SetParamFactor(Settings()->GetValue("RenderSystem.Shadow.Factor"));
		m_pPassShadow->SetCamera(pCamera);
		m_pPassShadow->SetOutputTexture(GetRenderTexture(rtShadow));
		m_pPassShadow->Render(taskGraph, ptrCommandBuffer);
	}

	uint32_t rtColor = RENDER_TEXTURE_FULL_HDR_COLOR1;
	{
		m_pPassForwardLighting->SetCamera(pCamera);
		m_pPassForwardLighting->SetInputTexture(GetRenderTexture(rtShadow), GetRenderTexture(rtSSAO));
		m_pPassForwardLighting->SetOutputTexture(GetRenderTexture(rtColor), GetRenderTexture(rtDepth));
		m_pPassForwardLighting->Render(taskGraph, ptrCommandBuffer);
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
		m_pPassBloomLuminanceThreshold->Render(taskGraph, ptrCommandBuffer);

		m_pPassBloomBlurHorizontal->SetParamRange(Settings()->GetValue("RenderSystem.Bloom.BlurRange.FirstTime"));
		m_pPassBloomBlurHorizontal->SetCamera(pCamera);
		m_pPassBloomBlurHorizontal->SetInputTexture(GetRenderTexture(rtBloomThreshold));
		m_pPassBloomBlurHorizontal->SetOutputTexture(GetRenderTexture(rtBloomBlurHorizontal));
		m_pPassBloomBlurHorizontal->Render(taskGraph, ptrCommandBuffer);

		m_pPassBloomBlurVertical->SetParamRange(Settings()->GetValue("RenderSystem.Bloom.BlurRange.FirstTime"));
		m_pPassBloomBlurVertical->SetCamera(pCamera);
		m_pPassBloomBlurVertical->SetInputTexture(GetRenderTexture(rtBloomBlurHorizontal));
		m_pPassBloomBlurVertical->SetOutputTexture(GetRenderTexture(rtBloomBlurVertical));
		m_pPassBloomBlurVertical->Render(taskGraph, ptrCommandBuffer);

		m_pPassBloomBlurHorizontal->SetParamRange(Settings()->GetValue("RenderSystem.Bloom.BlurRange.SecondTime"));
		m_pPassBloomBlurHorizontal->SetCamera(pCamera);
		m_pPassBloomBlurHorizontal->SetInputTexture(GetRenderTexture(rtBloomBlurVertical));
		m_pPassBloomBlurHorizontal->SetOutputTexture(GetRenderTexture(rtBloomBlurHorizontal));
		m_pPassBloomBlurHorizontal->Render(taskGraph, ptrCommandBuffer);

		m_pPassBloomBlurVertical->SetParamRange(Settings()->GetValue("RenderSystem.Bloom.BlurRange.SecondTime"));
		m_pPassBloomBlurVertical->SetCamera(pCamera);
		m_pPassBloomBlurVertical->SetInputTexture(GetRenderTexture(rtBloomBlurHorizontal));
		m_pPassBloomBlurVertical->SetOutputTexture(GetRenderTexture(rtBloomBlurVertical));
		m_pPassBloomBlurVertical->Render(taskGraph, ptrCommandBuffer);

		m_pPassBloomBlendAdd->SetCamera(pCamera);
		m_pPassBloomBlendAdd->SetInputTexture(GetRenderTexture(rtColor), GetRenderTexture(rtBloomBlurVertical));
		m_pPassBloomBlendAdd->SetOutputTexture(GetRenderTexture(rtBloom));
		m_pPassBloomBlendAdd->Render(taskGraph, ptrCommandBuffer);
	}
	rtColor = rtBloom;
	rtFinal = RENDER_TEXTURE_FULL_HDR_COLOR1;

	m_pPassColorGrading->SetCamera(pCamera);
	m_pPassColorGrading->SetInputTexture(GetRenderTexture(rtColor));
	m_pPassColorGrading->SetOutputTexture(GetRenderTexture(rtFinal));
	m_pPassColorGrading->Render(taskGraph, ptrCommandBuffer);

	m_pPassFinal->SetCamera(pCamera);
	m_pPassFinal->SetInputTexture(GetRenderTexture(rtFinal));
	m_pPassFinal->SetOutputTexture(GfxRenderer()->GetSwapChain()->GetFrameIndex(), GetRenderTexture(GfxRenderer()->GetSwapChain()->GetFrameIndex()));
	m_pPassFinal->Render(taskGraph, ptrCommandBuffer, GfxRenderer()->GetSwapChain()->GetFrameIndex(), bPresent);
}
