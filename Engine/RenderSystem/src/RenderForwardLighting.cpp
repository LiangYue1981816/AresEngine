#include "EngineHeader.h"
#include "RenderHeader.h"
#include "VKRenderer.h"
#include "GLES3Renderer.h"


void CRenderSystem::RenderForwardLighting(CTaskPool& taskPool, CTaskGraph& taskGraph, CCamera* pCamera, bool bPresent)
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
			RenderForwardLighting(taskPool, taskGraph, pCamera, bPresent, ptrGraphicCommandBuffer);
		}
		GfxRenderer()->EndRecord(ptrGraphicCommandBuffer);
		GfxRenderer()->Submit(ptrGraphicCommandBuffer, ptrComputeCommandBuffer->GetSemaphore());
	}
	GfxRenderer()->Present(ptrGraphicCommandBuffer->GetSemaphore());
}

void CRenderSystem::RenderForwardLighting(CTaskPool& taskPool, CTaskGraph& taskGraph, CCamera* pCamera, bool bPresent, CGfxCommandBufferPtr ptrCommandBuffer)
{
	{
		uint32_t rtOutDepth = RENDER_TEXTURE_FULL_DEPTH;
		InternalPassPreZ(taskPool, taskGraph, pCamera, ptrCommandBuffer, rtOutDepth);
	}
	{
		uint32_t rtOutShadow = RENDER_TEXTURE_SHADOW;
		InternalPassShadow(taskPool, taskGraph, pCamera, ptrCommandBuffer, rtOutShadow);
	}
	{
		uint32_t rtInDepth = RENDER_TEXTURE_FULL_DEPTH;
		uint32_t rtOutSSAO = RENDER_TEXTURE_FULL_HDR_COLOR0;
		uint32_t rtTempBlur = RENDER_TEXTURE_FULL_HDR_COLOR1;
		InternalPassSSAO(taskPool, taskGraph, pCamera, ptrCommandBuffer, rtInDepth, rtOutSSAO, rtTempBlur);
	}
	{
		uint32_t rtInShadow = RENDER_TEXTURE_SHADOW;
		uint32_t rtInSSAO = RENDER_TEXTURE_FULL_HDR_COLOR0;
		uint32_t rtOutColor = RENDER_TEXTURE_FULL_HDR_COLOR1;
		uint32_t rtOutDepth = RENDER_TEXTURE_FULL_DEPTH;
		InternalPassForwardLighting(taskPool, taskGraph, pCamera, ptrCommandBuffer, rtInShadow, rtInSSAO, rtOutColor, rtOutDepth);
	}
	{
		uint32_t rtInColor = RENDER_TEXTURE_FULL_HDR_COLOR1;
		uint32_t rtOutBloom = RENDER_TEXTURE_FULL_HDR_COLOR0;
		uint32_t rtTempThreshold = RENDER_TEXTURE_QUATER_HDR_COLOR0;
		uint32_t rtTempBlur0 = RENDER_TEXTURE_QUATER_HDR_COLOR0;
		uint32_t rtTempBlur1 = RENDER_TEXTURE_QUATER_HDR_COLOR1;
		InternalPassBloom(taskPool, taskGraph, pCamera, ptrCommandBuffer, rtInColor, rtOutBloom, rtTempThreshold, rtTempBlur0, rtTempBlur1);
	}
	{
		uint32_t rtInColor = RENDER_TEXTURE_FULL_HDR_COLOR0;
		uint32_t rtOutColor = RENDER_TEXTURE_FULL_HDR_COLOR1;
		InternalPassColorGrading(taskPool, taskGraph, pCamera, ptrCommandBuffer, rtInColor, rtOutColor);
	}
	{
		uint32_t rtInColor = RENDER_TEXTURE_FULL_HDR_COLOR1;
		InternalPassFinal(taskPool, taskGraph, pCamera, ptrCommandBuffer, rtInColor, bPresent);
	}
}
