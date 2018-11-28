#include "GfxHeader.h"


CGLES3SwapChain::CGLES3SwapChain(void *hDC, int width, int height, uint32_t format)
	: CGfxSwapChain(hDC, width, height, format)
	, m_hDC(hDC)
	, m_fbo(0)
{
	m_ptrTexture = GLES3Renderer()->NewTexture2D(HashValue("SwapChain Color Texture"));
	m_ptrTexture->Create(format, width, height, 1, 0);

	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	{
		const float color[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		const eastl::vector<uint32_t> drawBuffers{ GL_COLOR_ATTACHMENT0 };

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, ((CGLES3Texture2D *)m_ptrTexture.GetPointer())->GetTarget(), ((CGLES3Texture2D *)m_ptrTexture.GetPointer())->GetTexture(), 0);
		glClearBufferfv(GL_COLOR, 0, color);

		glReadBuffers((int)drawBuffers.size(), drawBuffers.data());
		glDrawBuffers((int)drawBuffers.size(), drawBuffers.data());

		uint32_t status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		bool bValid = status == GL_FRAMEBUFFER_COMPLETE;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

CGLES3SwapChain::~CGLES3SwapChain(void)
{
	glDeleteFramebuffers(1, &m_fbo);
}

void CGLES3SwapChain::Present(void)
{
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

#ifdef PLATFORM_WINDOWS
	::SwapBuffers((HDC)m_hDC);
#endif
}

void CGLES3SwapChain::AcquireNextTexture(void)
{
	m_indexTexture = (m_indexTexture + 1) % SWAPCHAIN_IMAGE_COUNT;
}

int CGLES3SwapChain::GetTextureIndex(void) const
{
	return m_indexTexture;
}

const CGfxTexture2DPtr& CGLES3SwapChain::GetTexture(int index) const
{
	return m_ptrTexture;
}
