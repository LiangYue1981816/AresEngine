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
	m_ptrMainCommandBuffer[0]->Clearup();
	m_ptrMainCommandBuffer[1]->Clearup();
	m_ptrMainCommandBuffer[2]->Clearup();
}

void CRenderSolutionDefault::CreateFrameBuffer(void)
{
	for (int indexFrame = 0; indexFrame < CGfxSwapChain::SWAPCHAIN_FRAME_COUNT; indexFrame++) {
		m_renderParams.ptrColorTextures[indexFrame] = GfxRenderer()->GetSwapChain()->GetFrameTexture(indexFrame);
		m_renderParams.ptrDepthStencilTextures[indexFrame] = GfxRenderer()->NewRenderTexture(HashValueFormat("DepthStencilTexture %d", indexFrame));
		m_renderParams.ptrDepthStencilTextures[indexFrame]->Create(GFX_PIXELFORMAT_D32_SFLOAT_PACK32, GfxRenderer()->GetSwapChain()->GetWidth(), GfxRenderer()->GetSwapChain()->GetHeight());
	}

	const int numAttachments = 2;
	const int numSubpasses = 1;

	m_renderParams.ptrRenderPass = GfxRenderer()->NewRenderPass(HashValue("Default"), numAttachments, numSubpasses);
	{
		m_renderParams.ptrRenderPass->SetColorAttachment(0, m_renderParams.ptrColorTextures[0]->GetFormat(), m_renderParams.ptrColorTextures[0]->GetSamples(), true, false, true, 0.2f, 0.2f, 0.2f, 0.0f);
		m_renderParams.ptrRenderPass->SetDepthStencilAttachment(1, m_renderParams.ptrDepthStencilTextures[0]->GetFormat(), m_renderParams.ptrDepthStencilTextures[0]->GetSamples(), true, true, 1.0f, 0);

		m_renderParams.ptrRenderPass->SetSubpassOutputColorReference(0, 0);
		m_renderParams.ptrRenderPass->SetSubpassOutputDepthStencilReference(0, 1);
	}
	m_renderParams.ptrRenderPass->Create();

	for (int indexFrame = 0; indexFrame < CGfxSwapChain::SWAPCHAIN_FRAME_COUNT; indexFrame++) {
		m_renderParams.ptrFrameBuffer[indexFrame] = GfxRenderer()->NewFrameBuffer(m_renderParams.ptrColorTextures[indexFrame]->GetWidth(), m_renderParams.ptrColorTextures[indexFrame]->GetHeight(), numAttachments);
		{
			m_renderParams.ptrFrameBuffer[indexFrame]->SetAttachmentTexture(0, m_renderParams.ptrColorTextures[indexFrame]);
			m_renderParams.ptrFrameBuffer[indexFrame]->SetAttachmentTexture(1, m_renderParams.ptrDepthStencilTextures[indexFrame]);
		}
		m_renderParams.ptrFrameBuffer[indexFrame]->Create(m_renderParams.ptrRenderPass);
	}
}

void CRenderSolutionDefault::DestroyFrameBuffer(void)
{
	for (int indexFrame = 0; indexFrame < CGfxSwapChain::SWAPCHAIN_FRAME_COUNT; indexFrame++) {
		m_renderParams.ptrColorTextures[indexFrame].Release();
		m_renderParams.ptrDepthStencilTextures[indexFrame].Release();
	}

	for (int indexFrame = 0; indexFrame < CGfxSwapChain::SWAPCHAIN_FRAME_COUNT; indexFrame++) {
		m_renderParams.ptrFrameBuffer[indexFrame].Release();
	}

	m_renderParams.ptrRenderPass.Release();
}

void CRenderSolutionDefault::CreateFrameBufferMSAA(int samples)
{
	for (int indexFrame = 0; indexFrame < CGfxSwapChain::SWAPCHAIN_FRAME_COUNT; indexFrame++) {
		m_renderParamsMSAA.ptrColorTextures[indexFrame] = GfxRenderer()->GetSwapChain()->GetFrameTexture(indexFrame);
		m_renderParamsMSAA.ptrColorTexturesMSAA[indexFrame] = GfxRenderer()->NewRenderTexture(HashValueFormat("ColorTexture %d", indexFrame));
		m_renderParamsMSAA.ptrColorTexturesMSAA[indexFrame]->Create(GFX_PIXELFORMAT_BGRA8_UNORM_PACK8, GfxRenderer()->GetSwapChain()->GetWidth(), GfxRenderer()->GetSwapChain()->GetHeight(), samples);
		m_renderParamsMSAA.ptrDepthStencilTexturesMSAA[indexFrame] = GfxRenderer()->NewRenderTexture(HashValueFormat("DepthStencilTexture %d", indexFrame));
		m_renderParamsMSAA.ptrDepthStencilTexturesMSAA[indexFrame]->Create(GFX_PIXELFORMAT_D32_SFLOAT_PACK32, GfxRenderer()->GetSwapChain()->GetWidth(), GfxRenderer()->GetSwapChain()->GetHeight(), samples);
	}

	const int numAttachments = 3;
	const int numSubpasses = 1;

	m_renderParamsMSAA.ptrRenderPass = GfxRenderer()->NewRenderPass(HashValue("Default"), numAttachments, numSubpasses);
	{
		m_renderParamsMSAA.ptrRenderPass->SetColorAttachment(0, m_renderParamsMSAA.ptrColorTextures[0]->GetFormat(), m_renderParamsMSAA.ptrColorTextures[0]->GetSamples(), true, false, false, 0.2f, 0.2f, 0.2f, 0.0f);
		m_renderParamsMSAA.ptrRenderPass->SetColorAttachment(1, m_renderParamsMSAA.ptrColorTexturesMSAA[0]->GetFormat(), m_renderParamsMSAA.ptrColorTexturesMSAA[0]->GetSamples(), false, false, true, 0.2f, 0.2f, 0.2f, 0.0f);
		m_renderParamsMSAA.ptrRenderPass->SetDepthStencilAttachment(2, m_renderParamsMSAA.ptrDepthStencilTexturesMSAA[0]->GetFormat(), m_renderParamsMSAA.ptrDepthStencilTexturesMSAA[0]->GetSamples(), true, true, 1.0f, 0);

		m_renderParamsMSAA.ptrRenderPass->SetSubpassResolveReference(0, 0);
		m_renderParamsMSAA.ptrRenderPass->SetSubpassOutputColorReference(0, 1);
		m_renderParamsMSAA.ptrRenderPass->SetSubpassOutputDepthStencilReference(0, 2);
	}
	m_renderParamsMSAA.ptrRenderPass->Create();

	for (int indexFrame = 0; indexFrame < CGfxSwapChain::SWAPCHAIN_FRAME_COUNT; indexFrame++) {
		m_renderParamsMSAA.ptrFrameBuffer[indexFrame] = GfxRenderer()->NewFrameBuffer(m_renderParamsMSAA.ptrColorTextures[indexFrame]->GetWidth(), m_renderParamsMSAA.ptrColorTextures[indexFrame]->GetHeight(), numAttachments);
		{
			m_renderParamsMSAA.ptrFrameBuffer[indexFrame]->SetAttachmentTexture(0, m_renderParamsMSAA.ptrColorTextures[indexFrame]);
			m_renderParamsMSAA.ptrFrameBuffer[indexFrame]->SetAttachmentTexture(1, m_renderParamsMSAA.ptrColorTexturesMSAA[indexFrame]);
			m_renderParamsMSAA.ptrFrameBuffer[indexFrame]->SetAttachmentTexture(2, m_renderParamsMSAA.ptrDepthStencilTexturesMSAA[indexFrame]);
		}
		m_renderParamsMSAA.ptrFrameBuffer[indexFrame]->Create(m_renderParamsMSAA.ptrRenderPass);
	}
}

void CRenderSolutionDefault::DestroyFrameBufferMSAA(void)
{
	for (int indexFrame = 0; indexFrame < CGfxSwapChain::SWAPCHAIN_FRAME_COUNT; indexFrame++) {
		m_renderParamsMSAA.ptrColorTextures[indexFrame].Release();
		m_renderParamsMSAA.ptrColorTexturesMSAA[indexFrame].Release();
		m_renderParamsMSAA.ptrDepthStencilTexturesMSAA[indexFrame].Release();
	}

	for (int indexFrame = 0; indexFrame < CGfxSwapChain::SWAPCHAIN_FRAME_COUNT; indexFrame++) {
		m_renderParamsMSAA.ptrFrameBuffer[indexFrame].Release();
	}

	m_renderParamsMSAA.ptrRenderPass.Release();
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

		const CGfxRenderPassPtr ptrRenderPass = m_bEnableMSAA ? m_renderParamsMSAA.ptrRenderPass : m_renderParams.ptrRenderPass;
		const CGfxFrameBufferPtr ptrFrameBuffer = m_bEnableMSAA ? m_renderParamsMSAA.ptrFrameBuffer[indexFrame] : m_renderParams.ptrFrameBuffer[indexFrame];
		const CGfxRenderTexturePtr ptrPresentTexture = m_bEnableMSAA ? m_renderParamsMSAA.ptrColorTextures[indexFrame] : m_renderParams.ptrColorTextures[indexFrame];
		const CGfxRenderTexturePtr ptrColorTexture = m_bEnableMSAA ? m_renderParamsMSAA.ptrColorTexturesMSAA[indexFrame] : m_renderParams.ptrColorTextures[indexFrame];
		const CGfxRenderTexturePtr ptrDepthStencilTexture = m_bEnableMSAA ? m_renderParamsMSAA.ptrDepthStencilTexturesMSAA[indexFrame] : m_renderParams.ptrDepthStencilTextures[indexFrame];

		const CGfxCommandBufferPtr ptrMainCommandBuffer = m_ptrMainCommandBuffer[indexFrame];
		{
			ptrMainCommandBuffer->Clearup();

			GfxRenderer()->BeginRecord(ptrMainCommandBuffer);
			{
				if (m_bEnableMSAA) {
					GfxRenderer()->CmdSetImageLayout(ptrMainCommandBuffer, ptrPresentTexture, GFX_IMAGE_LAYOUT_PRESENT_SRC_OPTIMAL);
					GfxRenderer()->CmdSetImageLayout(ptrMainCommandBuffer, ptrColorTexture, GFX_IMAGE_LAYOUT_GENERAL);
					GfxRenderer()->CmdSetImageLayout(ptrMainCommandBuffer, ptrDepthStencilTexture, GFX_IMAGE_LAYOUT_GENERAL);
				}
				else {
					GfxRenderer()->CmdSetImageLayout(ptrMainCommandBuffer, ptrPresentTexture, GFX_IMAGE_LAYOUT_PRESENT_SRC_OPTIMAL);
					GfxRenderer()->CmdSetImageLayout(ptrMainCommandBuffer, ptrDepthStencilTexture, GFX_IMAGE_LAYOUT_GENERAL);
				}

				GfxRenderer()->CmdBeginRenderPass(ptrMainCommandBuffer, ptrFrameBuffer, ptrRenderPass);
				{
					m_pMainQueue->CmdDraw(indexQueue, ptrMainCommandBuffer, m_pEngine->GetDescriptorSet(), m_pMainCamera->GetDescriptorSet(), nameDefaultPass, m_pMainCamera->GetScissor(), m_pMainCamera->GetViewport());
				}
				GfxRenderer()->CmdEndRenderPass(ptrMainCommandBuffer);
			}
			GfxRenderer()->EndRecord(ptrMainCommandBuffer);
		}
		GfxRenderer()->Submit(ptrMainCommandBuffer);
	}
	GfxRenderer()->Present();
}
