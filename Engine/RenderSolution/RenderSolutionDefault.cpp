#include "TaskCommandBuffer.h"
#include "RenderSolutionDefault.h"


CRenderSolutionDefault::CRenderSolutionDefault(void)
{
	m_ptrRenderPass = Renderer()->CreateRenderPass(3, 1);
	m_ptrRenderPass->SetDepthStencilAttachment(0, true, true, 1.0f, 0);
	m_ptrRenderPass->SetColorAttachment(1, true, true, 0.2f, 0.2f, 0.2f, 0.0f);
	m_ptrRenderPass->SetColorAttachment(2, false, true, 0.2f, 0.2f, 0.2f, 0.0f);
	m_ptrRenderPass->SetSubpassOutputDepthStencilReference(0, 0);
	m_ptrRenderPass->SetSubpassOutputColorReference(0, 1);
	m_ptrRenderPass->SetSubpassResolveColorReference(0, 2);
}

CRenderSolutionDefault::~CRenderSolutionDefault(void)
{
	Clearup(0);
	Clearup(1);
}

void CRenderSolutionDefault::SetEnableMSAA(bool bEnable, int width, int height, int samples)
{

}

void CRenderSolutionDefault::Render(int indexQueue)
{
	m_taskCommandBuffer.Wait();
	{
		static uint32_t namePass = HashValue("Default");
		static CTaskCommandBuffer taskCommandBuffers[THREAD_COUNT];

		for (int indexThread = 0; indexThread < THREAD_COUNT; indexThread++) {
			taskCommandBuffers[indexThread].SetParams(indexThread, indexQueue, namePass);
			m_taskCommandBuffer.Task(&taskCommandBuffers[indexThread], MainCamera(), NULL, NULL);
		}
	}
	m_taskCommandBuffer.Dispatch();
}

void CRenderSolutionDefault::Present(int indexQueue)
{
	CGfxCommandBuffer *pMainCommandBuffer = &m_mainCommandBuffer[indexQueue];

	const CGfxRenderPassPtr &ptrRenderPass = m_ptrRenderPass;
	const CGfxFrameBufferPtr &ptrFrameBuffer = m_ptrFrameBufferScreens[Renderer()->GetSwapChain()->GetTextureIndex()];

	Renderer()->CmdBeginRenderPass(pMainCommandBuffer, ptrFrameBuffer, ptrRenderPass);
	{
		MainCamera()->CmdExecute(pMainCommandBuffer, indexQueue);
	}
	Renderer()->CmdEndRenderPass(pMainCommandBuffer);

	Renderer()->Submit(pMainCommandBuffer);
	Renderer()->Present();

	/*
	CGfxCommandBuffer *pMainCommandBuffer = &m_mainCommandBuffer[indexQueue];
	CGfxFrameBufferPtr &ptrFrameBuffer = m_bEnableMSAA ? m_ptrFrameBufferMSAA : m_ptrFrameBufferScreen;

	Renderer()->CmdBeginRenderPass(pMainCommandBuffer, ptrFrameBuffer);
	{
		Renderer()->CmdSetScissor(pMainCommandBuffer, 0, 0, ptrFrameBuffer->GetWidth(), ptrFrameBuffer->GetHeight());
		Renderer()->CmdSetViewport(pMainCommandBuffer, 0, 0, ptrFrameBuffer->GetWidth(), ptrFrameBuffer->GetHeight());
		Renderer()->CmdClearDepth(pMainCommandBuffer, 1.0f);
		Renderer()->CmdClearColor(pMainCommandBuffer, 0.2f, 0.2f, 0.2f, 0.0f);
		MainCamera()->CmdExecute(indexQueue, pMainCommandBuffer);
	}
	Renderer()->CmdEndRenderPass(pMainCommandBuffer);
	Renderer()->CmdResolve(pMainCommandBuffer, ptrFrameBuffer, m_ptrFrameBufferScreen);

	Renderer()->Submit(pMainCommandBuffer);
	Renderer()->Present();
	*/
}

void CRenderSolutionDefault::Clearup(int indexQueue)
{
	m_mainCommandBuffer[indexQueue].Clearup();
	MainCamera()->ClearQueue(indexQueue);
}
