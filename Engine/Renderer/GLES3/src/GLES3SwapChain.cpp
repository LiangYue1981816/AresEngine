#include "GLES3Renderer.h"


CGLES3SwapChain::CGLES3SwapChain(void* hDC, int width, int height, GfxPixelFormat format)
	: CGfxSwapChain(width, height, format)

#ifdef PLATFORM_WINDOWS
	, m_hDC(hDC)
#endif

	, m_format(GFX_PIXELFORMAT_UNDEFINED)
	, m_width(0)
	, m_height(0)
	, m_surface(0)

	, m_indexFrame(0)
{
	Create(width, height, format);
}

CGLES3SwapChain::~CGLES3SwapChain(void)
{
	Destroy();
}

bool CGLES3SwapChain::Create(int width, int height, GfxPixelFormat format)
{
	if (width != 0 && height != 0 && format != GFX_PIXELFORMAT_UNDEFINED) {
		m_width = width;
		m_height = height;
		m_format = format;

		m_ptrRenderTexture = GLES3Renderer()->NewRenderTexture(HashValue("SwapChain Frame RenderTexture"));
		m_ptrRenderTexture->Create(format, width, height);

		glGenFramebuffers(1, &m_surface);
		glBindFramebuffer(GL_FRAMEBUFFER, m_surface);
		{
			const float color[] = { 0.0f, 0.0f, 0.0f, 0.0f };
			const uint32_t drawBuffer = GL_COLOR_ATTACHMENT0;

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, CGLES3Helper::TranslateTextureTarget(((CGLES3RenderTexture*)m_ptrRenderTexture.GetPointer())->GetType()), ((CGLES3RenderTexture*)m_ptrRenderTexture.GetPointer())->GetTexture(), 0);
			glClearBufferfv(GL_COLOR, 0, color);

			glReadBuffers(1, &drawBuffer);
			glDrawBuffers(1, &drawBuffer);

			uint32_t status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
			ASSERT(status == GL_FRAMEBUFFER_COMPLETE);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		CHECK_GL_ERROR_ASSERT();
	}

	return true;
}

void CGLES3SwapChain::Destroy(void)
{
	if (m_surface) {
		glDeleteFramebuffers(1, &m_surface);
	}

	m_format = GFX_PIXELFORMAT_UNDEFINED;
	m_width = 0;
	m_height = 0;
	m_surface = 0;
}

const CGfxSemaphore* CGLES3SwapChain::GetAcquireSemaphore(void) const
{
	return nullptr;
}

GfxPixelFormat CGLES3SwapChain::GetFormat(void) const
{
	return m_format;
}

int CGLES3SwapChain::GetWidth(void) const
{
	return m_width;
}

int CGLES3SwapChain::GetHeight(void) const
{
	return m_height;
}

int CGLES3SwapChain::GetFrameIndex(void) const
{
	return m_indexFrame;
}

const CGfxRenderTexturePtr CGLES3SwapChain::GetFrameTexture(int index) const
{
	return m_ptrRenderTexture;
}

void CGLES3SwapChain::Present(const CGfxSemaphore* pWaitSemaphore)
{
	if (m_surface) {
		GLBindFramebuffer(GL_FRAMEBUFFER, 0);
		GLBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // Screen
		GLBindFramebuffer(GL_READ_FRAMEBUFFER, m_surface);
		{
			glBlitFramebuffer(
				0, 0, m_width, m_height,
				0, 0, m_width, m_height,
				GL_COLOR_BUFFER_BIT,
				GL_NEAREST);
		}
		GLBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		GLBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
		CHECK_GL_ERROR_ASSERT();
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
