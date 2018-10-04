#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxFrameBuffer.h"


CGfxFrameBuffer::CGfxFrameBuffer(uint32_t width, uint32_t height, bool bDepthStencilRenderBuffer)
	: m_width(width)
	, m_height(height)

	, m_fbo(0)
	, m_rbo(0)
{
	glGenFramebuffers(1, &m_fbo);

	if (bDepthStencilRenderBuffer) {
		glGenRenderbuffers(1, &m_rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}
}

CGfxFrameBuffer::~CGfxFrameBuffer(void)
{
	if (m_fbo) {
		glDeleteFramebuffers(1, &m_fbo);
	}

	if (m_rbo) {
		glDeleteRenderbuffers(1, &m_rbo);
	}
}

void CGfxFrameBuffer::Release(void)
{
	Renderer()->DestroyFrameBuffer(this);
}

bool CGfxFrameBuffer::SetDepthStencilTexture(CGfxTexture2DPtr &ptrTexture)
{
	if (ptrTexture.IsValid()) {
		if (ptrTexture->GetWidth() != m_width || ptrTexture->GetHeight() != m_height) {
			return false;
		}
	}

	m_ptrDepthTexture = ptrTexture;
	return true;
}

bool CGfxFrameBuffer::SetColorTexture(uint32_t index, CGfxTexture2DPtr &ptrTexture, bool invalidation)
{
	if (ptrTexture.IsValid()) {
		if (ptrTexture->GetWidth() != m_width || ptrTexture->GetHeight() != m_height) {
			return false;
		}
	}

	m_ptrColorTextures[index] = ptrTexture;
	m_invalidations[index] = ptrTexture.IsValid() && invalidation;

	return true;
}

bool CGfxFrameBuffer::Apply(void)
{
	uint32_t status;

	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	{
		eastl::vector<uint32_t> drawBuffers;

		for (const auto &itTexture : m_ptrColorTextures) {
			if (itTexture.second.IsValid()) {
				drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + itTexture.first);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + itTexture.first, GL_TEXTURE_2D, itTexture.second->GetTexture(), 0);
			}
		}

		if (m_ptrDepthTexture.IsValid() || m_rbo) {
			if (m_ptrDepthTexture.IsValid()) {
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_ptrDepthTexture->GetTexture(), 0);
			}
			else {
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);
			}
		}

		glReadBuffers((int)drawBuffers.size(), drawBuffers.data());
		glDrawBuffers((int)drawBuffers.size(), drawBuffers.data());

		status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return status == GL_FRAMEBUFFER_COMPLETE;
}

uint32_t CGfxFrameBuffer::GetWidth(void) const
{
	return m_width;
}

uint32_t CGfxFrameBuffer::GetHeight(void) const
{
	return m_height;
}

CGfxTexture2DPtr CGfxFrameBuffer::GetDepthTexture(void) const
{
	return m_ptrDepthTexture;
}

CGfxTexture2DPtr CGfxFrameBuffer::GetColorTexture(uint32_t index) const
{
	const auto &itTexture = m_ptrColorTextures.find(index);
	return itTexture != m_ptrColorTextures.end() ? itTexture->second : NULL;
}

void CGfxFrameBuffer::Bind(void) const
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
}

void CGfxFrameBuffer::InvalidateFramebuffer(void) const
{
	eastl::vector<uint32_t> discardBuffers;

	for (const auto &itInvalidation : m_invalidations) {
		if (itInvalidation.second) {
			discardBuffers.push_back(GL_COLOR_ATTACHMENT0 + itInvalidation.first);
		}
	}

	discardBuffers.push_back(GL_DEPTH_ATTACHMENT);
	discardBuffers.push_back(GL_STENCIL_ATTACHMENT);
	discardBuffers.push_back(GL_DEPTH_STENCIL_ATTACHMENT);

	glInvalidateFramebuffer(GL_FRAMEBUFFER, (int)discardBuffers.size(), discardBuffers.data());
}
