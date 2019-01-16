#include "EngineHeader.h"


CRenderSolutionDefault::CRenderSolutionDefault(void)
{
	SetEnableMSAA(false);

	m_ptrMainCommandBuffer[0] = GfxRenderer()->NewCommandBuffer(true);
	m_ptrMainCommandBuffer[1] = GfxRenderer()->NewCommandBuffer(true);
}

CRenderSolutionDefault::~CRenderSolutionDefault(void)
{
	Clearup(0);
	Clearup(1);
}

void CRenderSolutionDefault::CreateFrameBuffer(void)
{
	m_ptrDepthStencilTexture = GfxRenderer()->NewRenderTexture(HashValue("DepthStencilTexture"));
	m_ptrDepthStencilTexture->Create(GFX_PIXELFORMAT_D24_UNORM_S8_UINT_PACK32, GfxRenderer()->GetSwapChain()->GetWidth(), GfxRenderer()->GetSwapChain()->GetHeight());

	for (int index = 0; index < CGfxSwapChain::SWAPCHAIN_FRAME_COUNT; index++) {
		m_ptrColorTextures[index] = GfxRenderer()->GetSwapChain()->GetFrameTexture(index);
	}

	int numAttachments = 2;
	int numSubpasses = 1;

	m_ptrRenderPass = GfxRenderer()->NewRenderPass(HashValue("Default"), numAttachments, numSubpasses);
	m_ptrRenderPass->SetDepthStencilAttachment(0, m_ptrDepthStencilTexture->GetFormat(), m_ptrDepthStencilTexture->GetSamples(), true, true, 1.0f, 0);
	m_ptrRenderPass->SetColorAttachment(1, m_ptrColorTextures[0]->GetFormat(), m_ptrColorTextures[0]->GetSamples(), false, true, 0.2f, 0.2f, 0.2f, 0.0f);
	m_ptrRenderPass->SetSubpassOutputDepthStencilReference(0, 0);
	m_ptrRenderPass->SetSubpassOutputColorReference(0, 1);
	m_ptrRenderPass->Create();

	for (int index = 0; index < CGfxSwapChain::SWAPCHAIN_FRAME_COUNT; index++) {
		m_ptrFrameBufferScreen[index] = GfxRenderer()->NewFrameBuffer(m_ptrColorTextures[index]->GetWidth(), m_ptrColorTextures[index]->GetHeight(), numAttachments);
		m_ptrFrameBufferScreen[index]->SetAttachmentTexture(0, m_ptrDepthStencilTexture);
		m_ptrFrameBufferScreen[index]->SetAttachmentTexture(1, m_ptrColorTextures[index]);
		m_ptrFrameBufferScreen[index]->Create(m_ptrRenderPass->GetRenderPass());
	}
}

void CRenderSolutionDefault::DestroyFrameBuffer(void)
{
	m_ptrDepthStencilTexture.Release();

	for (int index = 0; index < CGfxSwapChain::SWAPCHAIN_FRAME_COUNT; index++) {
		m_ptrColorTextures[index].Release();
	}

	for (int index = 0; index < CGfxSwapChain::SWAPCHAIN_FRAME_COUNT; index++) {
		m_ptrFrameBufferScreen[index].Release();
	}

	m_ptrRenderPass.Release();
}

void CRenderSolutionDefault::CreateFrameBufferMSAA(int samples)
{
	m_ptrColorTextureMSAA = GfxRenderer()->NewRenderTexture(HashValue("ColorTextureMSAA"));
	m_ptrColorTextureMSAA->Create(GFX_PIXELFORMAT_RGBA8_UNORM_PACK8, GfxRenderer()->GetSwapChain()->GetWidth(), GfxRenderer()->GetSwapChain()->GetHeight(), samples);

	m_ptrDepthStencilTextureMSAA = GfxRenderer()->NewRenderTexture(HashValue("DepthStencilTextureMSAA"));
	m_ptrDepthStencilTextureMSAA->Create(GFX_PIXELFORMAT_D24_UNORM_S8_UINT_PACK32, GfxRenderer()->GetSwapChain()->GetWidth(), GfxRenderer()->GetSwapChain()->GetHeight(), samples);

	for (int index = 0; index < CGfxSwapChain::SWAPCHAIN_FRAME_COUNT; index++) {
		m_ptrColorTextures[index] = GfxRenderer()->GetSwapChain()->GetFrameTexture(index);
	}

	int numAttachments = 3;
	int numSubpasses = 1;

	m_ptrRenderPassMSAA = GfxRenderer()->NewRenderPass(HashValue("DefaultMSAA"), numAttachments, numSubpasses);
	m_ptrRenderPassMSAA->SetDepthStencilAttachment(0, m_ptrDepthStencilTextureMSAA->GetFormat(), m_ptrDepthStencilTextureMSAA->GetSamples(), true, true, 1.0f, 0);
	m_ptrRenderPassMSAA->SetColorAttachment(1, m_ptrColorTextureMSAA->GetFormat(), m_ptrColorTextureMSAA->GetSamples(), true, true, 0.2f, 0.2f, 0.2f, 0.0f);
	m_ptrRenderPassMSAA->SetColorAttachment(2, m_ptrColorTextures[0]->GetFormat(), m_ptrColorTextures[0]->GetSamples(), false, true, 0.2f, 0.2f, 0.2f, 0.0f);
	m_ptrRenderPassMSAA->SetSubpassOutputDepthStencilReference(0, 0);
	m_ptrRenderPassMSAA->SetSubpassOutputColorReference(0, 1);
	m_ptrRenderPassMSAA->SetSubpassResolveReference(0, 2);
	m_ptrRenderPassMSAA->Create();

	for (int index = 0; index < CGfxSwapChain::SWAPCHAIN_FRAME_COUNT; index++) {
		m_ptrFrameBufferScreenMSAA[index] = GfxRenderer()->NewFrameBuffer(m_ptrColorTextures[index]->GetWidth(), m_ptrColorTextures[index]->GetHeight(), numAttachments);
		m_ptrFrameBufferScreenMSAA[index]->SetAttachmentTexture(0, m_ptrDepthStencilTextureMSAA);
		m_ptrFrameBufferScreenMSAA[index]->SetAttachmentTexture(1, m_ptrColorTextureMSAA);
		m_ptrFrameBufferScreenMSAA[index]->SetAttachmentTexture(2, m_ptrColorTextures[index]);
		m_ptrFrameBufferScreenMSAA[index]->Create(m_ptrRenderPassMSAA->GetRenderPass());
	}
}

void CRenderSolutionDefault::DestroyFrameBufferMSAA(void)
{
	m_ptrColorTextureMSAA.Release();
	m_ptrDepthStencilTextureMSAA.Release();

	for (int index = 0; index < CGfxSwapChain::SWAPCHAIN_FRAME_COUNT; index++) {
		m_ptrColorTextures[index].Release();
	}

	for (int index = 0; index < CGfxSwapChain::SWAPCHAIN_FRAME_COUNT; index++) {
		m_ptrFrameBufferScreenMSAA[index].Release();
	}

	m_ptrRenderPassMSAA.Release();
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

void CRenderSolutionDefault::Render(int indexQueue)
{
	// Update logic & camera
	{
		SceneManager()->UpdateLogic(Engine()->GetTotalTime(), Engine()->GetDeltaTime());
		SceneManager()->UpdateCamera(m_pMainCamera, indexQueue);
	}

	// Build command buffer
	{
		static const uint32_t nameDefaultPass = HashValue("Default");

		const CGfxRenderPassPtr &ptrRenderPass = m_bEnableMSAA ? m_ptrRenderPassMSAA : m_ptrRenderPass;
		const CGfxFrameBufferPtr &ptrFrameBuffer = m_bEnableMSAA ? m_ptrFrameBufferScreenMSAA[GfxRenderer()->GetSwapChain()->GetFrameIndex()] : m_ptrFrameBufferScreen[GfxRenderer()->GetSwapChain()->GetFrameIndex()];

		GfxRenderer()->CmdBeginRenderPass(m_ptrMainCommandBuffer[indexQueue], ptrFrameBuffer, ptrRenderPass);
		{
			const glm::vec4 &scissor = m_pMainCamera->GetScissor();
			const glm::vec4 &viewport = m_pMainCamera->GetViewport();

			GfxRenderer()->CmdSetScissor(m_ptrMainCommandBuffer[indexQueue], (int)scissor.x, (int)scissor.y, (int)scissor.z, (int)scissor.w);
			GfxRenderer()->CmdSetViewport(m_ptrMainCommandBuffer[indexQueue], (int)viewport.x, (int)viewport.y, (int)viewport.z, (int)viewport.w);

			m_pMainCamera->CmdDraw(indexQueue, m_ptrMainCommandBuffer[indexQueue], m_pUniformEngine->GetUniformBuffer(), nameDefaultPass);
		}
		GfxRenderer()->CmdEndRenderPass(m_ptrMainCommandBuffer[indexQueue]);
	}
}

void CRenderSolutionDefault::Present(int indexQueue)
{
	m_pUniformEngine->SetTime(Engine()->GetTotalTime(), Engine()->GetDeltaTime());

	m_pMainCamera->Apply();
	m_pShadowCamera->Apply();
	m_pUniformEngine->Apply();

	GfxRenderer()->Submit(m_ptrMainCommandBuffer[indexQueue]);
	GfxRenderer()->Present();
}

void CRenderSolutionDefault::Clearup(int indexQueue)
{
	m_pMainCamera->Clear(indexQueue);
	m_pShadowCamera->Clear(indexQueue);
	m_ptrMainCommandBuffer[indexQueue]->Clearup();
}
