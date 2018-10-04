#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxFrameBuffer.h"


static void glReadBuffers(GLsizei n, const GLenum *bufs)
{
	for (int index = 0; index < n; index++) {
		glReadBuffer(bufs[index]);
	}
}

CGfxFrameBuffer::CGfxFrameBuffer(GLuint width, GLuint height, bool bDepthRenderBuffer)
	: m_width(width)
	, m_height(height)

	, m_fbo(0)
	, m_rbo(0)
{
	glGenFramebuffers(1, &m_fbo);

	if (bDepthRenderBuffer) {
		glGenRenderbuffers(1, &m_rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_width, m_height);
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

bool CGfxFrameBuffer::SetDepthTexture(CGfxTexture2DPtr &ptrTexture)
{
	if (ptrTexture.IsValid()) {
		if (ptrTexture->GetWidth() != m_width || ptrTexture->GetHeight() != m_height) {
			return false;
		}
	}

	m_ptrDepthTexture = ptrTexture;
	return true;
}

bool CGfxFrameBuffer::SetColorTexture(GLuint index, CGfxTexture2DPtr &ptrTexture, bool invalidation)
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
	GLenum status;

	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	{
		eastl::vector<GLenum> drawBuffers;

		for (const auto &itTexture : m_ptrColorTextures) {
			if (itTexture.second.IsValid()) {
				drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + itTexture.first);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + itTexture.first, GL_TEXTURE_2D, itTexture.second->GetTexture(), 0);
			}
		}

		if (m_ptrDepthTexture.IsValid() || m_rbo) {
			if (m_ptrDepthTexture.IsValid()) {
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_ptrDepthTexture->GetTexture(), 0);
			}
			else {
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_rbo);
			}
		}

		glReadBuffers((GLsizei)drawBuffers.size(), drawBuffers.data());
		glDrawBuffers((GLsizei)drawBuffers.size(), drawBuffers.data());

		status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return status == GL_FRAMEBUFFER_COMPLETE;
}

GLuint CGfxFrameBuffer::GetWidth(void) const
{
	return m_width;
}

GLuint CGfxFrameBuffer::GetHeight(void) const
{
	return m_height;
}

CGfxTexture2DPtr CGfxFrameBuffer::GetDepthTexture(void) const
{
	return m_ptrDepthTexture;
}

CGfxTexture2DPtr CGfxFrameBuffer::GetColorTexture(GLuint index) const
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
	eastl::vector<GLenum> discardBuffers;

	for (const auto &itInvalidation : m_invalidations) {
		if (itInvalidation.second) {
			discardBuffers.push_back(GL_COLOR_ATTACHMENT0 + itInvalidation.first);
		}
	}

	discardBuffers.push_back(GL_DEPTH_ATTACHMENT);
	discardBuffers.push_back(GL_STENCIL_ATTACHMENT);
	discardBuffers.push_back(GL_DEPTH_STENCIL_ATTACHMENT);

	glInvalidateFramebuffer(GL_FRAMEBUFFER, (GLsizei)discardBuffers.size(), discardBuffers.data());
}
