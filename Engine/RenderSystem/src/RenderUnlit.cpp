#include "EngineHeader.h"
#include "RenderHeader.h"
#include "VKRenderer.h"
#include "GLES3Renderer.h"


void CRenderSystem::RenderUnlit(CTaskPool& taskPool, CTaskGraph& taskGraph, CCamera* pCamera, bool bPresent)
{
	m_pInstanceBufferPool->Clear();

	const CGfxSemaphore* pWaitSemaphore = GfxRenderer()->GetSwapChain()->GetAcquireSemaphore();
	const CGfxCommandBufferPtr ptrComputeCommandBuffer = m_ptrComputeCommandBuffer[GfxRenderer()->GetSwapChain()->GetFrameIndex()];
	const CGfxCommandBufferPtr ptrGraphicCommandBuffer = m_ptrGraphicCommandBuffer[GfxRenderer()->GetSwapChain()->GetFrameIndex()];

	GfxRenderer()->AcquireNextFrame();
	{
		GfxRenderer()->BeginRecord(ptrComputeCommandBuffer);
		{
			UpdateScene(taskPool, taskGraph, ptrComputeCommandBuffer);
		}
		GfxRenderer()->EndRecord(ptrComputeCommandBuffer);
		GfxRenderer()->Submit(ptrComputeCommandBuffer, pWaitSemaphore);

		GfxRenderer()->BeginRecord(ptrGraphicCommandBuffer);
		{
			RenderUnlit(taskPool, taskGraph, ptrGraphicCommandBuffer, pCamera, bPresent);
		}
		GfxRenderer()->EndRecord(ptrGraphicCommandBuffer);
		GfxRenderer()->Submit(ptrGraphicCommandBuffer, ptrComputeCommandBuffer->GetSemaphore());
	}
	GfxRenderer()->Present(ptrGraphicCommandBuffer->GetSemaphore());
}

void CRenderSystem::RenderUnlit(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer, CCamera* pCamera, bool bPresent)
{
	{
		uint32_t rtOutDepth = RENDER_TEXTURE_FULL_DEPTH;
		InternalPassPreZ(taskPool, taskGraph, ptrCommandBuffer, pCamera, rtOutDepth);
	}
	{
		uint32_t rtOutColor = RENDER_TEXTURE_FULL_HDR_COLOR0;
		uint32_t rtOutDepth = RENDER_TEXTURE_FULL_DEPTH;
		InternalPassUnlit(taskPool, taskGraph, ptrCommandBuffer, pCamera, rtOutColor, rtOutDepth);
	}
	{
		uint32_t rtInColor = RENDER_TEXTURE_FULL_HDR_COLOR0;
		InternalPassFinal(taskPool, taskGraph, ptrCommandBuffer, pCamera, rtInColor, bPresent);
	}
}
