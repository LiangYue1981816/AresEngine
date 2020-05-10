#include "EngineHeader.h"
#include "RenderHeader.h"
#include "VKRenderer.h"
#include "GLES3Renderer.h"


void CRenderSystem::RenderDefault(CTaskPool& taskPool, CTaskGraph& taskGraph, CCamera* pCamera, bool bPresent)
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
			RenderDefault(taskPool, taskGraph, ptrGraphicCommandBuffer, pCamera, bPresent);
		}
		GfxRenderer()->EndRecord(ptrGraphicCommandBuffer);
		GfxRenderer()->Submit(ptrGraphicCommandBuffer, ptrComputeCommandBuffer->GetSemaphore());
	}
	GfxRenderer()->Present(ptrGraphicCommandBuffer->GetSemaphore());
}

void CRenderSystem::RenderDefault(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer, CCamera* pCamera, bool bPresent)
{
	uint32_t rtFinal;

	uint32_t rtDepth = RENDER_TEXTURE_FULL_DEPTH;
	{
		m_pPassPreZ->SetCamera(pCamera);
		m_pPassPreZ->SetOutputTexture(GetRenderTexture(rtDepth));
		m_pPassPreZ->Render(taskPool, taskGraph, ptrCommandBuffer);
	}

	uint32_t rtColor = RENDER_TEXTURE_FULL_HDR_COLOR0;
	{
		m_pPassDefault->SetCamera(pCamera);
		m_pPassDefault->SetOutputTexture(GetRenderTexture(rtColor), GetRenderTexture(rtDepth));
		m_pPassDefault->Render(taskPool, taskGraph, ptrCommandBuffer);
	}
	rtFinal = rtColor;

	m_pPassFinal->SetCamera(pCamera);
	m_pPassFinal->SetInputTexture(GetRenderTexture(rtFinal));
	m_pPassFinal->SetOutputTexture(GfxRenderer()->GetSwapChain()->GetFrameIndex(), GetRenderTexture(GfxRenderer()->GetSwapChain()->GetFrameIndex()));
	m_pPassFinal->Render(taskPool, taskGraph, ptrCommandBuffer, GfxRenderer()->GetSwapChain()->GetFrameIndex(), bPresent);
}
