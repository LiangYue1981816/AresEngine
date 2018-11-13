#include "Engine.h"
#include "RenderSolutionDefault.h"


CRenderSolutionDefault::CRenderSolutionDefault(void)
{
	SetEnableMSAA(false);

	m_ptrMainCommandBuffer[0] = Renderer()->NewCommandBuffer(true);
	m_ptrMainCommandBuffer[1] = Renderer()->NewCommandBuffer(true);
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
	m_ptrDepthStencilTexture = Renderer()->NewTexture2D(HashValue("DepthStencilTexture"));
	m_ptrDepthStencilTexture->Create(GL_DEPTH_STENCIL, GL_DEPTH24_STENCIL8, Renderer()->GetSwapChain()->GetWidth(), Renderer()->GetSwapChain()->GetHeight(), 1);

	for (int index = 0; index < CGfxSwapChain::SWAPCHAIN_IMAGE_COUNT; index++) {
		CGfxTexture2DPtr ptrColorTexture = Renderer()->GetSwapChain()->GetTexture(index);
		m_ptrFrameBufferScreen[index] = Renderer()->NewFrameBuffer(ptrColorTexture->GetWidth(), ptrColorTexture->GetHeight());
		m_ptrFrameBufferScreen[index]->SetAttachmentTexture(0, m_ptrDepthStencilTexture);
		m_ptrFrameBufferScreen[index]->SetAttachmentTexture(1, ptrColorTexture);
	}

	m_ptrRenderPass = Renderer()->NewRenderPass(2, 1);
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
	m_ptrColorTextureMSAA = Renderer()->NewTexture2D(HashValue("ColorTextureMSAA"));
	m_ptrColorTextureMSAA->Create(GL_RGBA, GL_RGBA8, Renderer()->GetSwapChain()->GetWidth(), Renderer()->GetSwapChain()->GetHeight(), 1, samples);

	m_ptrDepthStencilTextureMSAA = Renderer()->NewTexture2D(HashValue("DepthStencilTextureMSAA"));
	m_ptrDepthStencilTextureMSAA->Create(GL_DEPTH_STENCIL, GL_DEPTH24_STENCIL8, Renderer()->GetSwapChain()->GetWidth(), Renderer()->GetSwapChain()->GetHeight(), 1, samples);

	for (int index = 0; index < CGfxSwapChain::SWAPCHAIN_IMAGE_COUNT; index++) {
		CGfxTexture2DPtr ptrColorTexture = Renderer()->GetSwapChain()->GetTexture(index);
		m_ptrFrameBufferScreenMSAA[index] = Renderer()->NewFrameBuffer(ptrColorTexture->GetWidth(), ptrColorTexture->GetHeight());
		m_ptrFrameBufferScreenMSAA[index]->SetAttachmentTexture(0, m_ptrDepthStencilTextureMSAA);
		m_ptrFrameBufferScreenMSAA[index]->SetAttachmentTexture(1, m_ptrColorTextureMSAA);
		m_ptrFrameBufferScreenMSAA[index]->SetAttachmentTexture(2, ptrColorTexture);
	}

	m_ptrRenderPassMSAA = Renderer()->NewRenderPass(3, 1);
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
	event_wait(&m_eventFinish);
	{
		m_indexQueue = indexQueue;
	}
	event_unsignal(&m_eventFinish);
	event_signal(&m_eventDispatch);
}

void CRenderSolutionDefault::RenderThread(void)
{
	const static uint32_t nameDefaultPass = HashValue("Default");

	const CGfxRenderPassPtr &ptrRenderPass = m_bEnableMSAA ? m_ptrRenderPassMSAA : m_ptrRenderPass;
	const CGfxFrameBufferPtr &ptrFrameBuffer = m_bEnableMSAA ? m_ptrFrameBufferScreenMSAA[Renderer()->GetSwapChain()->GetTextureIndex()] : m_ptrFrameBufferScreen[Renderer()->GetSwapChain()->GetTextureIndex()];

	Renderer()->CmdBeginRenderPass(m_ptrMainCommandBuffer[m_indexQueue], ptrFrameBuffer, ptrRenderPass);
	{
		const glm::vec4 &scissor = MainCamera()->GetScissor();
		const glm::vec4 &viewport = MainCamera()->GetViewport();

		Renderer()->CmdSetScissor(m_ptrMainCommandBuffer[m_indexQueue], (int)scissor.x, (int)scissor.y, (int)scissor.z, (int)scissor.w);
		Renderer()->CmdSetViewport(m_ptrMainCommandBuffer[m_indexQueue], (int)viewport.x, (int)viewport.y, (int)viewport.z, (int)viewport.w);

		MainCamera()->CmdDraw(m_indexQueue, m_ptrMainCommandBuffer[m_indexQueue], SceneManager()->GetUniformEngine(), nameDefaultPass);
	}
	Renderer()->CmdEndRenderPass(m_ptrMainCommandBuffer[m_indexQueue]);
}

void CRenderSolutionDefault::Present(int indexQueue)
{
	Renderer()->Submit(m_ptrMainCommandBuffer[indexQueue]);
	Renderer()->Present();
}

void CRenderSolutionDefault::Clearup(int indexQueue)
{
	m_ptrMainCommandBuffer[indexQueue]->Clearup();
	MainCamera()->Clear(indexQueue);
}
