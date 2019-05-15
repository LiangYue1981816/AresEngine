#include "EngineHeader.h"


CRenderSolutionDefault::CRenderSolutionDefault(void)
{
	SetEnableMSAA(false);

	m_ptrMainCommandBuffer[0] = GfxRenderer()->NewCommandBuffer(0, true);
	m_ptrMainCommandBuffer[1] = GfxRenderer()->NewCommandBuffer(0, true);
}

CRenderSolutionDefault::~CRenderSolutionDefault(void)
{
	Clearup(0);
	Clearup(1);
}

void CRenderSolutionDefault::CreateFrameBuffer(void)
{
	const GfxPixelFormat formatDepthStencils[] = {
		GFX_PIXELFORMAT_D24_UNORM_S8_UINT_PACK32,
		GFX_PIXELFORMAT_D32_SFLOAT_S8_UINT_PACK64,
		GFX_PIXELFORMAT_D16_UNORM_S8_UINT_PACK32,
	};

	m_ptrDepthStencilTexture = GfxRenderer()->NewRenderTexture(HashValue("DepthStencilTexture"));
	for (int indexFormat = 0; indexFormat < sizeof(formatDepthStencils) / sizeof(GfxPixelFormat); indexFormat++) {
		if (m_ptrDepthStencilTexture->Create(formatDepthStencils[indexFormat], GfxRenderer()->GetSwapChain()->GetWidth(), GfxRenderer()->GetSwapChain()->GetHeight())) {
			break;
		}
	}

	for (int indexFrame = 0; indexFrame < CGfxSwapChain::SWAPCHAIN_FRAME_COUNT; indexFrame++) {
		m_ptrColorTextures[indexFrame] = GfxRenderer()->GetSwapChain()->GetFrameTexture(indexFrame);
	}

	int numAttachments = 2;
	int numSubpasses = 1;

	m_ptrRenderPass = GfxRenderer()->NewRenderPass(HashValue("Default"), numAttachments, numSubpasses);
	m_ptrRenderPass->SetDepthStencilAttachment(0, m_ptrDepthStencilTexture->GetFormat(), m_ptrDepthStencilTexture->GetSamples(), true, true, 1.0f, 0);
	m_ptrRenderPass->SetColorAttachment(1, m_ptrColorTextures[0]->GetFormat(), m_ptrColorTextures[0]->GetSamples(), false, true, 0.2f, 0.2f, 0.2f, 0.0f);
	m_ptrRenderPass->SetSubpassOutputDepthStencilReference(0, 0);
	m_ptrRenderPass->SetSubpassOutputColorReference(0, 1);
	m_ptrRenderPass->Create();

	for (int indexFrame = 0; indexFrame < CGfxSwapChain::SWAPCHAIN_FRAME_COUNT; indexFrame++) {
		m_ptrFrameBufferScreen[indexFrame] = GfxRenderer()->NewFrameBuffer(m_ptrColorTextures[indexFrame]->GetWidth(), m_ptrColorTextures[indexFrame]->GetHeight(), numAttachments);
		m_ptrFrameBufferScreen[indexFrame]->SetAttachmentTexture(0, m_ptrDepthStencilTexture);
		m_ptrFrameBufferScreen[indexFrame]->SetAttachmentTexture(1, m_ptrColorTextures[indexFrame]);
		m_ptrFrameBufferScreen[indexFrame]->Create(m_ptrRenderPass);
	}
}

void CRenderSolutionDefault::DestroyFrameBuffer(void)
{
	m_ptrDepthStencilTexture.Release();

	for (int indexFrame = 0; indexFrame < CGfxSwapChain::SWAPCHAIN_FRAME_COUNT; indexFrame++) {
		m_ptrColorTextures[indexFrame].Release();
	}

	for (int indexFrame = 0; indexFrame < CGfxSwapChain::SWAPCHAIN_FRAME_COUNT; indexFrame++) {
		m_ptrFrameBufferScreen[indexFrame].Release();
	}

	m_ptrRenderPass.Release();
}

void CRenderSolutionDefault::CreateFrameBufferMSAA(int samples)
{
	const GfxPixelFormat formatDepthStencils[] = {
		GFX_PIXELFORMAT_D24_UNORM_S8_UINT_PACK32,
		GFX_PIXELFORMAT_D32_SFLOAT_S8_UINT_PACK64,
		GFX_PIXELFORMAT_D16_UNORM_S8_UINT_PACK32,
	};

	m_ptrColorTextureMSAA = GfxRenderer()->NewRenderTexture(HashValue("ColorTextureMSAA"));
	m_ptrColorTextureMSAA->Create(GFX_PIXELFORMAT_RGBA8_UNORM_PACK8, GfxRenderer()->GetSwapChain()->GetWidth(), GfxRenderer()->GetSwapChain()->GetHeight(), samples);

	m_ptrDepthStencilTextureMSAA = GfxRenderer()->NewRenderTexture(HashValue("DepthStencilTextureMSAA"));
	for (int indexFormat = 0; indexFormat < sizeof(formatDepthStencils) / sizeof(GfxPixelFormat); indexFormat++) {
		if (m_ptrDepthStencilTextureMSAA->Create(formatDepthStencils[indexFormat], GfxRenderer()->GetSwapChain()->GetWidth(), GfxRenderer()->GetSwapChain()->GetHeight(), samples)) {
			break;
		}
	}

	for (int indexFrame = 0; indexFrame < CGfxSwapChain::SWAPCHAIN_FRAME_COUNT; indexFrame++) {
		m_ptrColorTextures[indexFrame] = GfxRenderer()->GetSwapChain()->GetFrameTexture(indexFrame);
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

	for (int indexFrame = 0; indexFrame < CGfxSwapChain::SWAPCHAIN_FRAME_COUNT; indexFrame++) {
		m_ptrFrameBufferScreenMSAA[indexFrame] = GfxRenderer()->NewFrameBuffer(m_ptrColorTextures[indexFrame]->GetWidth(), m_ptrColorTextures[indexFrame]->GetHeight(), numAttachments);
		m_ptrFrameBufferScreenMSAA[indexFrame]->SetAttachmentTexture(0, m_ptrDepthStencilTextureMSAA);
		m_ptrFrameBufferScreenMSAA[indexFrame]->SetAttachmentTexture(1, m_ptrColorTextureMSAA);
		m_ptrFrameBufferScreenMSAA[indexFrame]->SetAttachmentTexture(2, m_ptrColorTextures[indexFrame]);
		m_ptrFrameBufferScreenMSAA[indexFrame]->Create(m_ptrRenderPassMSAA);
	}
}

void CRenderSolutionDefault::DestroyFrameBufferMSAA(void)
{
	m_ptrColorTextureMSAA.Release();
	m_ptrDepthStencilTextureMSAA.Release();

	for (int indexFrame = 0; indexFrame < CGfxSwapChain::SWAPCHAIN_FRAME_COUNT; indexFrame++) {
		m_ptrColorTextures[indexFrame].Release();
	}

	for (int indexFrame = 0; indexFrame < CGfxSwapChain::SWAPCHAIN_FRAME_COUNT; indexFrame++) {
		m_ptrFrameBufferScreenMSAA[indexFrame].Release();
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
	// Uniform
	{
		m_pUniformEngine->SetTime(Engine()->GetTotalTime(), Engine()->GetDeltaTime());
	}

	// Update logic & camera
	{
		SceneManager()->UpdateLogic(Engine()->GetTotalTime(), Engine()->GetDeltaTime());
		SceneManager()->UpdateCamera(m_pMainCamera, indexQueue);
	}

	// Build command buffer
	{
		static const uint32_t nameDefaultPass = HashValue("Default");

		const CGfxRenderPassPtr ptrRenderPass = m_bEnableMSAA ? m_ptrRenderPassMSAA : m_ptrRenderPass;
		const CGfxFrameBufferPtr ptrFrameBuffer = m_bEnableMSAA ? m_ptrFrameBufferScreenMSAA[GfxRenderer()->GetSwapChain()->GetFrameIndex()] : m_ptrFrameBufferScreen[GfxRenderer()->GetSwapChain()->GetFrameIndex()];

		GfxRenderer()->CmdBeginRenderPass(m_ptrMainCommandBuffer[indexQueue], ptrFrameBuffer, ptrRenderPass);
		{
			const glm::vec4& scissor = m_pMainCamera->GetScissor();
			const glm::vec4& viewport = m_pMainCamera->GetViewport();

			GfxRenderer()->CmdSetScissor(m_ptrMainCommandBuffer[indexQueue], (int)scissor.x, (int)scissor.y, (int)scissor.z, (int)scissor.w);
			GfxRenderer()->CmdSetViewport(m_ptrMainCommandBuffer[indexQueue], (int)viewport.x, (int)viewport.y, (int)viewport.z, (int)viewport.w);

			m_pMainCamera->CmdDraw(indexQueue, m_ptrMainCommandBuffer[indexQueue], m_ptrDescriptorSetEngine, nameDefaultPass);
		}
		GfxRenderer()->CmdEndRenderPass(m_ptrMainCommandBuffer[indexQueue]);
	}
}

void CRenderSolutionDefault::Present(int indexQueue)
{
	m_pMainCamera->Apply();
	m_pShadowCamera->Apply();
	m_pUniformEngine->Apply();

	eastl::vector<CGfxCommandBufferPtr> ptrCommandBuffers;
	ptrCommandBuffers.emplace_back(m_ptrMainCommandBuffer[indexQueue]);

	GfxRenderer()->AcquireNextFrame();
	GfxRenderer()->Submit(ptrCommandBuffers);
	GfxRenderer()->Present();
}

void CRenderSolutionDefault::Clearup(int indexQueue)
{
	m_pMainCamera->Clear(indexQueue);
	m_pShadowCamera->Clear(indexQueue);
	m_ptrMainCommandBuffer[indexQueue]->Clearup();
}
