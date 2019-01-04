#include "GLES3Renderer.h"


CGLES3SwapChain::CGLES3SwapChain(void *hDC, int width, int height, GfxPixelFormat pixelFormat)
	: CGfxSwapChain(width, height, pixelFormat)

	, m_hDC(hDC)
	, m_fbo(0)

	, m_indexFrame(0)
{
	if (m_width != 0 && m_height != 0 && m_pixelFormat != GFX_PIXELFORMAT_UNDEFINED) {
		m_ptrFrameTexture = GLES3Renderer()->NewRenderTexture(HashValue("SwapChain Frame Texture"));
		m_ptrFrameTexture->Create(m_pixelFormat, m_width, m_height);

		glGenFramebuffers(1, &m_fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
		{
			const float color[] = { 0.0f, 0.0f, 0.0f, 0.0f };
			const eastl::vector<uint32_t> drawBuffers{ GL_COLOR_ATTACHMENT0 };

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, CGLES3Helper::TranslateTextureTarget(((CGLES3RenderTexture *)m_ptrFrameTexture.GetPointer())->GetType()), ((CGLES3RenderTexture *)m_ptrFrameTexture.GetPointer())->GetTexture(), 0);
			glClearBufferfv(GL_COLOR, 0, color);

			glReadBuffers((int)drawBuffers.size(), drawBuffers.data());
			glDrawBuffers((int)drawBuffers.size(), drawBuffers.data());

			uint32_t status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
			bool bValid = status == GL_FRAMEBUFFER_COMPLETE;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

CGLES3SwapChain::~CGLES3SwapChain(void)
{
	if (m_fbo) {
		glDeleteFramebuffers(1, &m_fbo);
	}
}

void CGLES3SwapChain::Present(void)
{
	if (m_fbo) {
		GLBindFramebuffer(GL_FRAMEBUFFER, 0);
		GLBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // Screen
		GLBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
		{
			glBlitFramebuffer(
				0, 0, m_width, m_height,
				0, 0, m_width, m_height,
				GL_COLOR_BUFFER_BIT,
				GL_NEAREST);
		}
		GLBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		GLBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	}

#ifdef PLATFORM_WINDOWS
	if (m_hDC) {
		::SwapBuffers((HDC)m_hDC);
	}
#endif
}

void CGLES3SwapChain::AcquireNextFrame(void)
{
	m_indexFrame = (m_indexFrame + 1) % SWAPCHAIN_FRAME_COUNT;
}

int CGLES3SwapChain::GetFrameIndex(void) const
{
	return m_indexFrame;
}

const CGfxRenderTexturePtr& CGLES3SwapChain::GetFrameTexture(int index) const
{
	return m_ptrFrameTexture;
}
