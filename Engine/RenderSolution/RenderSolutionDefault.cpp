#include "TaskCommandBuffer.h"
#include "RenderSolutionDefault.h"


CRenderSolutionDefault::CRenderSolutionDefault(void)
	: m_mainCommandBuffer{ CGfxCommandBuffer(true), CGfxCommandBuffer(true) }
{
	SetEnableMSAA(false);
}

CRenderSolutionDefault::~CRenderSolutionDefault(void)
{
	Clearup(0);
	Clearup(1);
}

void CRenderSolutionDefault::SetEnableMSAA(bool bEnable, int samples)
{
	m_bEnableMSAA = bEnable;

	DestroyFrameBuffer();
	DestroyFrameBufferMSAA();

	if (m_bEnableMSAA) {
		CreateFrameBufferMSAA(samples);
	}
	else {
		CreateFrameBuffer();
	}
}

void CRenderSolutionDefault::CreateFrameBuffer(void)
{
	m_ptrDepthStencilTexture = Renderer()->CreateTexture2D(HashValue("DepthStencilTexture"));
	m_ptrDepthStencilTexture->Create(GL_DEPTH_STENCIL, GL_DEPTH24_STENCIL8, Renderer()->GetSwapChain()->GetWidth(), Renderer()->GetSwapChain()->GetHeight(), 1);

	for (int index = 0; index < CGfxSwapChain::SWAPCHAIN_IMAGE_COUNT; index++) {
		CGfxTexture2DPtr ptrColorTexture = Renderer()->GetSwapChain()->GetTexture(index);
		m_ptrFrameBufferScreen[index] = Renderer()->CreateFrameBuffer(ptrColorTexture->GetWidth(), ptrColorTexture->GetHeight());
		m_ptrFrameBufferScreen[index]->SetAttachmentTexture(0, m_ptrDepthStencilTexture);
		m_ptrFrameBufferScreen[index]->SetAttachmentTexture(1, ptrColorTexture);
	}

	m_ptrRenderPass = Renderer()->CreateRenderPass(2, 1);
	m_ptrRenderPass->SetDepthStencilAttachment(0, true, true, 1.0f, 0);
	m_ptrRenderPass->SetColorAttachment(1, false, true, 0.2f, 0.2f, 0.2f, 0.0f);
	m_ptrRenderPass->SetSubpassOutputDepthStencilReference(0, 0);
	m_ptrRenderPass->SetSubpassOutputColorReference(0, 1);
}

void CRenderSolutionDefault::DestroyFrameBuffer(void)
{
	m_ptrDepthStencilTexture.Release();

	for (int index = 0; index < CGfxSwapChain::SWAPCHAIN_IMAGE_COUNT; index++) {
		m_ptrFrameBufferScreen[index].Release();
	}
}

void CRenderSolutionDefault::CreateFrameBufferMSAA(int samples)
{
	m_ptrColorTextureMSAA = Renderer()->CreateTexture2D(HashValue("ColorTextureMSAA"));
	m_ptrColorTextureMSAA->Create(GL_RGBA, GL_RGBA8, Renderer()->GetSwapChain()->GetWidth(), Renderer()->GetSwapChain()->GetHeight(), 1, samples);

	m_ptrDepthStencilTextureMSAA = Renderer()->CreateTexture2D(HashValue("DepthStencilTextureMSAA"));
	m_ptrDepthStencilTextureMSAA->Create(GL_DEPTH_STENCIL, GL_DEPTH24_STENCIL8, Renderer()->GetSwapChain()->GetWidth(), Renderer()->GetSwapChain()->GetHeight(), 1, samples);

	for (int index = 0; index < CGfxSwapChain::SWAPCHAIN_IMAGE_COUNT; index++) {
		CGfxTexture2DPtr ptrColorTexture = Renderer()->GetSwapChain()->GetTexture(index);
		m_ptrFrameBufferScreenMSAA[index] = Renderer()->CreateFrameBuffer(ptrColorTexture->GetWidth(), ptrColorTexture->GetHeight());
		m_ptrFrameBufferScreenMSAA[index]->SetAttachmentTexture(0, m_ptrDepthStencilTextureMSAA);
		m_ptrFrameBufferScreenMSAA[index]->SetAttachmentTexture(1, m_ptrColorTextureMSAA);
		m_ptrFrameBufferScreenMSAA[index]->SetAttachmentTexture(2, ptrColorTexture);
	}

	m_ptrRenderPassMSAA = Renderer()->CreateRenderPass(3, 1);
	m_ptrRenderPassMSAA->SetDepthStencilAttachment(0, true, true, 1.0f, 0);
	m_ptrRenderPassMSAA->SetColorAttachment(1, true, true, 0.2f, 0.2f, 0.2f, 0.0f);
	m_ptrRenderPassMSAA->SetColorAttachment(2, false, true, 0.2f, 0.2f, 0.2f, 0.0f);
	m_ptrRenderPassMSAA->SetSubpassOutputDepthStencilReference(0, 0);
	m_ptrRenderPassMSAA->SetSubpassOutputColorReference(0, 1);
	m_ptrRenderPassMSAA->SetSubpassResolveColorReference(0, 2);
}

void CRenderSolutionDefault::DestroyFrameBufferMSAA(void)
{
	m_ptrColorTextureMSAA.Release();
	m_ptrDepthStencilTextureMSAA.Release();

	for (int index = 0; index < CGfxSwapChain::SWAPCHAIN_IMAGE_COUNT; index++) {
		m_ptrFrameBufferScreenMSAA[index].Release();
	}
}

void CRenderSolutionDefault::Render(int indexQueue)
{
	m_taskCommandBuffer.Wait();
	{
		static uint32_t namePass = HashValue("Default");
		static CTaskCommandBuffer taskCommandBuffers[THREAD_COUNT];

		for (int indexThread = 0; indexThread < THREAD_COUNT; indexThread++) {
			m_secondaryCommandBuffer[indexThread][indexQueue].emplace_back(false);
			CGfxCommandBuffer *pCommandBuffer = &m_secondaryCommandBuffer[indexThread][indexQueue][m_secondaryCommandBuffer[indexThread][indexQueue].size() - 1];

			taskCommandBuffers[indexThread].SetParams(pCommandBuffer, indexThread, indexQueue, namePass);
			m_taskCommandBuffer.Task(&taskCommandBuffers[indexThread], MainCamera(), nullptr, nullptr);
		}
	}
	m_taskCommandBuffer.Dispatch();
}

void CRenderSolutionDefault::Present(int indexQueue)
{
	CGfxCommandBuffer *pMainCommandBuffer = &m_mainCommandBuffer[indexQueue];

	const CGfxRenderPassPtr &ptrRenderPass = m_bEnableMSAA ? m_ptrRenderPassMSAA : m_ptrRenderPass;
	const CGfxFrameBufferPtr &ptrFrameBuffer = m_bEnableMSAA ? m_ptrFrameBufferScreenMSAA[Renderer()->GetSwapChain()->GetTextureIndex()] : m_ptrFrameBufferScreen[Renderer()->GetSwapChain()->GetTextureIndex()];

	Renderer()->CmdBeginRenderPass(pMainCommandBuffer, ptrFrameBuffer, ptrRenderPass);
	{
		const glm::vec4 &scissor = MainCamera()->GetScissor();
		const glm::vec4 &viewport = MainCamera()->GetViewport();

		Renderer()->CmdSetScissor(pMainCommandBuffer, (int)scissor.x, (int)scissor.y, (int)scissor.z, (int)scissor.w);
		Renderer()->CmdSetViewport(pMainCommandBuffer, (int)viewport.x, (int)viewport.y, (int)viewport.z, (int)viewport.w);

		for (int indexThread = 0; indexThread < THREAD_COUNT; indexThread++) {
			for (const auto &itCommandBuffer : m_secondaryCommandBuffer[indexThread][indexQueue]) {
				Renderer()->CmdExecute(pMainCommandBuffer, (CGfxCommandBuffer *)&itCommandBuffer);
			}
		}
	}
	Renderer()->CmdEndRenderPass(pMainCommandBuffer);

	Renderer()->Submit(pMainCommandBuffer);
	Renderer()->Present();
}

void CRenderSolutionDefault::Clearup(int indexQueue)
{
	m_mainCommandBuffer[indexQueue].Clearup();

	for (int indexThread = 0; indexThread < THREAD_COUNT; indexThread++) {
		m_secondaryCommandBuffer[indexThread][indexQueue].clear();
	}

	MainCamera()->Clear(indexQueue);
}
