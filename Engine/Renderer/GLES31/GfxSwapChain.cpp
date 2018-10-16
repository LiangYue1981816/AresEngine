#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxSwapChain.h"


CGfxSwapChain::CGfxSwapChain(void *hDC, int width, int height, uint32_t format)
	: m_hDC(hDC)
	, m_indexTexture(0)
{
	m_ptrTexture = Renderer()->CreateTexture2D(HashValue("SwapChain Color Texture"));
	m_ptrTexture->Create(format, format, width, height, 1, 0);

	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	{
		const float color[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		const eastl::vector<uint32_t> drawBuffers{ GL_COLOR_ATTACHMENT0 };

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_ptrTexture->GetTarget(), m_ptrTexture->GetTexture(), 0);
		glClearBufferfv(GL_COLOR, 0, color);

		glReadBuffers((int)drawBuffers.size(), drawBuffers.data());
		glDrawBuffers((int)drawBuffers.size(), drawBuffers.data());

		uint32_t status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		bool bValid = status == GL_FRAMEBUFFER_COMPLETE;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

CGfxSwapChain::~CGfxSwapChain(void)
{
	glDeleteFramebuffers(1, &m_fbo);
}

void CGfxSwapChain::Present(void)
{
	GLBindFramebuffer(GL_FRAMEBUFFER, 0);
	GLBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // Screen
	GLBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
	{
		glBlitFramebuffer(
			0, 0, m_ptrTexture->GetWidth(), m_ptrTexture->GetHeight(),
			0, 0, m_ptrTexture->GetWidth(), m_ptrTexture->GetHeight(),
			GL_COLOR_BUFFER_BIT,
			GL_NEAREST);
	}
	GLBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	GLBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

	m_indexTexture = (m_indexTexture + 1) % 3;

#ifdef _WINDOWS
	::SwapBuffers((HDC)m_hDC);
#endif
}

int CGfxSwapChain::GetWidth(void) const
{
	return m_ptrTexture->GetWidth();
}

int CGfxSwapChain::GetHeight(void) const
{
	return m_ptrTexture->GetHeight();
}

int CGfxSwapChain::GetTextureIndex(void) const
{
	return m_indexTexture;
}

const CGfxTexture2DPtr& CGfxSwapChain::GetTexture(int index) const
{
	return m_ptrTexture;
}
