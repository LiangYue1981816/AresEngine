#include "RenderHeader.h"


void CRenderSystem::RenderUnlit(CTaskPool& taskPool, CTaskGraph& taskGraph, CCamera* pCamera, CGfxCommandBufferPtr ptrComputeCommandBuffer, CGfxCommandBufferPtr ptrGraphicCommandBuffer, const CGfxSemaphore* pWaitSemaphore)
{
	m_pInstanceBufferPool->Clear();

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
			InternalPassFinal(taskPool, taskGraph, ptrGraphicCommandBuffer, pCamera, rtInColor);
		}
	}
	GfxRenderer()->EndRecord(ptrGraphicCommandBuffer);
	GfxRenderer()->Submit(ptrGraphicCommandBuffer, ptrComputeCommandBuffer->GetSemaphore());
}
