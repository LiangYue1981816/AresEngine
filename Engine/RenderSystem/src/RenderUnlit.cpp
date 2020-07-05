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
				uint32_t rtOutColor = RENDER_TEXTURE_FULL_HDR_COLOR0;
				uint32_t rtOutDepth = RENDER_TEXTURE_FULL_DEPTH;
				InternalPassUnlit(taskPool, taskGraph, ptrGraphicCommandBuffer, pCamera, rtOutColor, rtOutDepth);
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
