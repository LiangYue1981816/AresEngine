#include "EngineHeader.h"


CRenderSolutionDefault::CRenderSolutionDefault(void)
{
	SetEnableMSAA(false);

	m_ptrMainCommandBuffer[0] = GfxRenderer()->NewCommandBuffer(0, true);
	m_ptrMainCommandBuffer[1] = GfxRenderer()->NewCommandBuffer(0, true);
	m_ptrMainCommandBuffer[2] = GfxRenderer()->NewCommandBuffer(0, true);
}

CRenderSolutionDefault::~CRenderSolutionDefault(void)
{

}

void CRenderSolutionDefault::CreateFrameBuffer(void)
{
	for (int indexFrame = 0; indexFrame < CGfxSwapChain::SWAPCHAIN_FRAME_COUNT; indexFrame++) {
		m_ptrColorTextures[indexFrame] = GfxRenderer()->GetSwapChain()->GetFrameTexture(indexFrame);
		m_ptrDepthStencilTextures[indexFrame] = GfxRenderer()->NewRenderTexture(HashValueFormat("DepthStencilTexture %d", indexFrame));
		m_ptrDepthStencilTextures[indexFrame]->Create(GFX_PIXELFORMAT_D32_SFLOAT_PACK32, GfxRenderer()->GetSwapChain()->GetWidth(), GfxRenderer()->GetSwapChain()->GetHeight());
	}

	const int numAttachments = 2;
	const int numSubpasses = 1;

	m_ptrRenderPass = GfxRenderer()->NewRenderPass(HashValue("Default"), numAttachments, numSubpasses);
	{
		m_ptrRenderPass->SetColorAttachment(0, m_ptrColorTextures[0]->GetFormat(), m_ptrColorTextures[0]->GetSamples(), true, false, true, 0.2f, 0.2f, 0.2f, 0.0f);
		m_ptrRenderPass->SetDepthStencilAttachment(1, m_ptrDepthStencilTextures[0]->GetFormat(), m_ptrDepthStencilTextures[0]->GetSamples(), true, true, 1.0f, 0);

		m_ptrRenderPass->SetSubpassOutputColorReference(0, 0);
		m_ptrRenderPass->SetSubpassOutputDepthStencilReference(0, 1);
	}
	m_ptrRenderPass->Create();

	for (int indexFrame = 0; indexFrame < CGfxSwapChain::SWAPCHAIN_FRAME_COUNT; indexFrame++) {
		m_ptrFrameBuffer[indexFrame] = GfxRenderer()->NewFrameBuffer(m_ptrColorTextures[indexFrame]->GetWidth(), m_ptrColorTextures[indexFrame]->GetHeight(), numAttachments);
		{
			m_ptrFrameBuffer[indexFrame]->SetAttachmentTexture(0, m_ptrColorTextures[indexFrame]);
			m_ptrFrameBuffer[indexFrame]->SetAttachmentTexture(1, m_ptrDepthStencilTextures[indexFrame]);
		}
		m_ptrFrameBuffer[indexFrame]->Create(m_ptrRenderPass);
	}
}

void CRenderSolutionDefault::DestroyFrameBuffer(void)
{
	for (int indexFrame = 0; indexFrame < CGfxSwapChain::SWAPCHAIN_FRAME_COUNT; indexFrame++) {
		m_ptrColorTextures[indexFrame].Release();
		m_ptrDepthStencilTextures[indexFrame].Release();
	}

	for (int indexFrame = 0; indexFrame < CGfxSwapChain::SWAPCHAIN_FRAME_COUNT; indexFrame++) {
		m_ptrFrameBuffer[indexFrame].Release();
	}

	m_ptrRenderPass.Release();
}

void CRenderSolutionDefault::CreateFrameBufferMSAA(int samples)
{
	/*
	m_ptrColorTextureMSAA = GfxRenderer()->NewRenderTexture(HashValue("ColorTextureMSAA"));
	m_ptrColorTextureMSAA->Create(GFX_PIXELFORMAT_RGBA8_UNORM_PACK8, GfxRenderer()->GetSwapChain()->GetWidth(), GfxRenderer()->GetSwapChain()->GetHeight(), samples);

	m_ptrDepthStencilTextureMSAA = GfxRenderer()->NewRenderTexture(HashValue("DepthStencilTextureMSAA"));
	m_ptrDepthStencilTextureMSAA->Create(GFX_PIXELFORMAT_D24_UNORM_S8_UINT_PACK32, GfxRenderer()->GetSwapChain()->GetWidth(), GfxRenderer()->GetSwapChain()->GetHeight(), samples)

	for (int indexFrame = 0; indexFrame < CGfxSwapChain::SWAPCHAIN_FRAME_COUNT; indexFrame++) {
		m_ptrColorTextures[indexFrame] = GfxRenderer()->GetSwapChain()->GetFrameTexture(indexFrame);
	}

	int numAttachments = 3;
	int numSubpasses = 1;

	m_ptrRenderPassMSAA = GfxRenderer()->NewRenderPass(HashValue("DefaultMSAA"), numAttachments, numSubpasses);
	m_ptrRenderPassMSAA->SetDepthStencilAttachment(0, m_ptrDepthStencilTextureMSAA->GetFormat(), m_ptrDepthStencilTextureMSAA->GetSamples(), true, true, 1.0f, 0);
	m_ptrRenderPassMSAA->SetColorAttachment(1, m_ptrColorTextureMSAA->GetFormat(), m_ptrColorTextureMSAA->GetSamples(), false, true, true, 0.2f, 0.2f, 0.2f, 0.0f);
	m_ptrRenderPassMSAA->SetColorAttachment(2, m_ptrColorTextures[0]->GetFormat(), m_ptrColorTextures[0]->GetSamples(), true, false, true, 0.2f, 0.2f, 0.2f, 0.0f);
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
	*/
}

void CRenderSolutionDefault::DestroyFrameBufferMSAA(void)
{
	/*
	m_ptrColorTextureMSAA.Release();
	m_ptrDepthStencilTextureMSAA.Release();

	for (int indexFrame = 0; indexFrame < CGfxSwapChain::SWAPCHAIN_FRAME_COUNT; indexFrame++) {
		m_ptrColorTextures[indexFrame].Release();
	}

	for (int indexFrame = 0; indexFrame < CGfxSwapChain::SWAPCHAIN_FRAME_COUNT; indexFrame++) {
		m_ptrFrameBufferScreenMSAA[indexFrame].Release();
	}

	m_ptrRenderPassMSAA.Release();
	*/
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

void CRenderSolutionDefault::Update(int indexQueue)
{
	m_pMainQueue->Clear(indexQueue);
	m_pLightQueue->Clear(indexQueue);
	m_pShadowQueue->Clear(indexQueue);

	SceneManager()->UpdateLogic(Engine()->GetTotalTime(), Engine()->GetDeltaTime());
	SceneManager()->UpdateCamera(m_pMainCamera, m_pMainQueue, indexQueue);
}

void CRenderSolutionDefault::Render(int indexQueue)
{
	m_pEngine->Apply();
	m_pMainCamera->Apply();
	m_pShadowCamera->Apply();

	GfxRenderer()->AcquireNextFrame();
	{
		const uint32_t nameDefaultPass = HashValue("Default");

		const int indexFrame = GfxRenderer()->GetSwapChain()->GetFrameIndex();
		const CGfxRenderPassPtr ptrRenderPass = m_ptrRenderPass;
		const CGfxFrameBufferPtr ptrFrameBuffer = m_ptrFrameBuffer[indexFrame];

		const CGfxCommandBufferPtr ptrMainCommandBuffer = m_ptrMainCommandBuffer[indexFrame];
		{
			ptrMainCommandBuffer->Clearup();

			GfxRenderer()->BeginRecord(ptrMainCommandBuffer);
			GfxRenderer()->CmdBeginRenderPass(ptrMainCommandBuffer, ptrFrameBuffer, ptrRenderPass);
			{
				m_pMainQueue->CmdDraw(indexQueue, ptrMainCommandBuffer, m_pEngine->GetDescriptorSet(), m_pMainCamera->GetDescriptorSet(), nameDefaultPass, m_pMainCamera->GetScissor(), m_pMainCamera->GetViewport());
			}
			GfxRenderer()->CmdEndRenderPass(ptrMainCommandBuffer);
			GfxRenderer()->EndRecord(ptrMainCommandBuffer);
		}
		GfxRenderer()->Submit(ptrMainCommandBuffer);
	}
	GfxRenderer()->Present();
}
