#include "EngineHeader.h"
#include "RenderHeader.h"
#include "VKRenderer.h"
#include "GLES3Renderer.h"


void CRenderSystem::RenderForwardShading(CTaskPool& taskPool, CTaskGraph& taskGraph, CCamera* pCamera, bool bPresent)
{
	m_pInstanceBufferPool->Clear();

	const CGfxSemaphore* pWaitSemaphore = GfxRenderer()->GetSwapChain()->GetAcquireSemaphore();
	const CGfxCommandBufferPtr ptrComputeCommandBuffer = m_ptrComputeCommandBuffer[GfxRenderer()->GetSwapChain()->GetFrameIndex()];
	const CGfxCommandBufferPtr ptrGraphicCommandBuffer = m_ptrGraphicCommandBuffer[GfxRenderer()->GetSwapChain()->GetFrameIndex()];

	GfxRenderer()->AcquireNextFrame();
	{
		GfxRenderer()->BeginRecord(ptrComputeCommandBuffer);
		{
			{
				InternalComputeScene(taskPool, taskGraph, ptrComputeCommandBuffer);
			}
		}
		GfxRenderer()->EndRecord(ptrComputeCommandBuffer);
		GfxRenderer()->Submit(ptrComputeCommandBuffer, pWaitSemaphore);

		GfxRenderer()->BeginRecord(ptrGraphicCommandBuffer);
		{
			{
				uint32_t rtOutDepth = RENDER_TEXTURE_FULL_DEPTH;
				InternalPassPreZ(taskPool, taskGraph, ptrGraphicCommandBuffer, pCamera, rtOutDepth);
			}
			{
				uint32_t rtOutShadow = RENDER_TEXTURE_SHADOW;
				InternalPassShadow(taskPool, taskGraph, ptrGraphicCommandBuffer, pCamera, rtOutShadow);
			}
			{
				uint32_t rtInDepth = RENDER_TEXTURE_FULL_DEPTH;
				uint32_t rtOutSSAO = RENDER_TEXTURE_FULL_HDR_COLOR0;
				uint32_t rtTempBlur = RENDER_TEXTURE_FULL_HDR_COLOR1;
				InternalPassSSAO(taskPool, taskGraph, ptrGraphicCommandBuffer, pCamera, rtInDepth, rtOutSSAO, rtTempBlur);
			}
			{
				uint32_t rtInShadow = RENDER_TEXTURE_SHADOW;
				uint32_t rtInSSAO = RENDER_TEXTURE_FULL_HDR_COLOR0;
				uint32_t rtOutColor = RENDER_TEXTURE_FULL_HDR_COLOR1;
				uint32_t rtOutDepth = RENDER_TEXTURE_FULL_DEPTH;
				InternalPassForwardLighting(taskPool, taskGraph, ptrGraphicCommandBuffer, pCamera, rtInShadow, rtInSSAO, rtOutColor, rtOutDepth);
			}
			{
				uint32_t rtInColor = RENDER_TEXTURE_FULL_HDR_COLOR1;
				uint32_t rtOutBloom = RENDER_TEXTURE_FULL_HDR_COLOR0;
				uint32_t rtTempThreshold = RENDER_TEXTURE_QUATER_HDR_COLOR0;
				uint32_t rtTempBlur0 = RENDER_TEXTURE_QUATER_HDR_COLOR1;
				uint32_t rtTempBlur1 = RENDER_TEXTURE_QUATER_HDR_COLOR0;
				InternalPassBloom(taskPool, taskGraph, ptrGraphicCommandBuffer, pCamera, rtInColor, rtOutBloom, rtTempThreshold, rtTempBlur0, rtTempBlur1);
			}
			{
				uint32_t rtInColor = RENDER_TEXTURE_FULL_HDR_COLOR0;
				uint32_t rtOutEyeAdaptation = RENDER_TEXTURE_FULL_HDR_COLOR1;
				uint32_t rtOutScaleColor = RENDER_TEXTURE_QUATER_HDR_COLOR0;
				InternalPassEyeAdaptation(taskPool, taskGraph, ptrGraphicCommandBuffer, pCamera, rtInColor, rtOutEyeAdaptation, rtOutScaleColor);
			}
			{
				uint32_t rtInColor = RENDER_TEXTURE_FULL_HDR_COLOR1;
				uint32_t rtOutColorGrading = RENDER_TEXTURE_FULL_HDR_COLOR0;
				InternalPassColorGrading(taskPool, taskGraph, ptrGraphicCommandBuffer, pCamera, rtInColor, rtOutColorGrading);
			}
			{
				uint32_t rtInColor = RENDER_TEXTURE_FULL_HDR_COLOR0;
				InternalPassFinal(taskPool, taskGraph, ptrGraphicCommandBuffer, pCamera, rtInColor, bPresent);
			}
		}
		GfxRenderer()->EndRecord(ptrGraphicCommandBuffer);
		GfxRenderer()->Submit(ptrGraphicCommandBuffer, ptrComputeCommandBuffer->GetSemaphore());
	}
	GfxRenderer()->Present(ptrGraphicCommandBuffer->GetSemaphore());
}
