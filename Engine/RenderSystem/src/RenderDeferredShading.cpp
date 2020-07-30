#include "RenderHeader.h"


void CRenderSystem::RenderDeferredShading(CTaskPool& taskPool, CTaskGraph& taskGraph, CCamera* pCamera, CGfxCommandBufferPtr ptrComputeCommandBuffer, CGfxCommandBufferPtr ptrGraphicCommandBuffer, const CGfxSemaphore* pWaitSemaphore)
{
	m_pInstanceBufferPool->Clear();

	GfxRenderer()->BeginRecord(ptrComputeCommandBuffer);
	{
		{
			InternalComputeScene(taskPool, taskGraph, ptrComputeCommandBuffer);
		}
		{
			uint32_t rtInColors[] = { RENDER_TEXTURE_HISTOGRAM_HDR_COLOR0,RENDER_TEXTURE_HISTOGRAM_HDR_COLOR1,RENDER_TEXTURE_HISTOGRAM_HDR_COLOR2 };
			InternalComputeEyeHistogram(taskPool, taskGraph, ptrComputeCommandBuffer, rtInColors[GfxRenderer()->GetSwapChain()->GetFrameIndex()]);
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
			uint32_t rtInDepth = RENDER_TEXTURE_FULL_DEPTH;
			uint32_t rtOutDepth = RENDER_TEXTURE_FULL_DEPTH_COPY;
			InternalPassCopyDepthStencil(taskPool, taskGraph, ptrGraphicCommandBuffer, pCamera, rtInDepth, rtOutDepth);
		}
		{
			uint32_t rtInDepth = RENDER_TEXTURE_FULL_DEPTH_COPY;
			uint32_t rtInShadow = RENDER_TEXTURE_SHADOW;
			uint32_t rtInSSAO = RENDER_TEXTURE_FULL_HDR_COLOR0;
			uint32_t rtOutColor = RENDER_TEXTURE_FULL_HDR_COLOR1;
			uint32_t rtOutDepth = RENDER_TEXTURE_FULL_DEPTH;
			uint32_t rtGBuffer0 = RENDER_TEXTURE_GBUFFER0;
			uint32_t rtGBuffer1 = RENDER_TEXTURE_GBUFFER1;
			InternalPassDeferredLighting(taskPool, taskGraph, ptrGraphicCommandBuffer, pCamera, rtInDepth, rtInShadow, rtInSSAO, rtOutColor, rtOutDepth, rtGBuffer0, rtGBuffer1);
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
			uint32_t rtOutDownSamples[] = { RENDER_TEXTURE_HISTOGRAM_HDR_COLOR0,RENDER_TEXTURE_HISTOGRAM_HDR_COLOR1,RENDER_TEXTURE_HISTOGRAM_HDR_COLOR2 };
			InternalPassEyeAdaptation(taskPool, taskGraph, ptrGraphicCommandBuffer, pCamera, rtInColor, rtOutEyeAdaptation, rtOutDownSamples[GfxRenderer()->GetSwapChain()->GetFrameIndex()]);
		}
		{
			uint32_t rtInColor = RENDER_TEXTURE_FULL_HDR_COLOR1;
			uint32_t rtOutColor = RENDER_TEXTURE_FULL_HDR_COLOR0;
			InternalPassColorGrading(taskPool, taskGraph, ptrGraphicCommandBuffer, pCamera, rtInColor, rtOutColor);
		}
		{
			uint32_t rtInColor = RENDER_TEXTURE_FULL_HDR_COLOR0;
			InternalPassFinal(taskPool, taskGraph, ptrGraphicCommandBuffer, pCamera, rtInColor);
		}
	}
	GfxRenderer()->EndRecord(ptrGraphicCommandBuffer);
	GfxRenderer()->Submit(ptrGraphicCommandBuffer, ptrComputeCommandBuffer->GetSemaphore());
}
