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
	, m_pDepthTexture(NULL)

	, refCount(0)
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

	if (m_pDepthTexture) {
		m_pDepthTexture->Release();
	}

	for (const auto &itTexture : m_pColorTextures) {
		if (itTexture.second) {
			itTexture.second->Release();
		}
	}
}

void CGfxFrameBuffer::Retain(void)
{
	refCount++;
}

void CGfxFrameBuffer::Release(void)
{
	if (refCount > 0) {
		refCount--;
	}

	if (refCount == 0) {
		Renderer()->FreeFrameBuffer(this);
	}
}

bool CGfxFrameBuffer::SetDepthTexture(CGfxTexture2D *pTexture)
{
	if (pTexture) {
		if (pTexture->GetWidth() != m_width || pTexture->GetHeight() != m_height) {
			return false;
		}
	}

	if (m_pDepthTexture) {
		m_pDepthTexture->Release();
	}

	m_pDepthTexture = pTexture;

	if (m_pDepthTexture) {
		m_pDepthTexture->Retain();
	}

	return true;
}

bool CGfxFrameBuffer::SetColorTexture(GLuint index, CGfxTexture2D *pTexture, bool invalidation)
{
	if (pTexture) {
		if (pTexture->GetWidth() != m_width || pTexture->GetHeight() != m_height) {
			return false;
		}
	}

	if (m_pColorTextures[index]) {
		m_pColorTextures[index]->Release();
	}

	m_pColorTextures[index] = pTexture;
	m_invalidations[index] = pTexture && invalidation;

	if (m_pColorTextures[index]) {
		m_pColorTextures[index]->Retain();
	}

	return true;
}

bool CGfxFrameBuffer::Apply(void)
{
	GLenum status;

	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	{
		eastl::vector<GLenum> drawBuffers;

		for (const auto &itTexture : m_pColorTextures) {
			if (itTexture.second) {
				drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + itTexture.first);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + itTexture.first, GL_TEXTURE_2D, itTexture.second->GetTexture(), 0);
			}
		}

		if (m_pDepthTexture || m_rbo) {
			if (m_pDepthTexture) {
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_pDepthTexture->GetTexture(), 0);
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

CGfxTexture2D* CGfxFrameBuffer::GetDepthTexture(void) const
{
	return m_pDepthTexture;
}

CGfxTexture2D* CGfxFrameBuffer::GetColorTexture(GLuint index) const
{
	const auto &itTexture = m_pColorTextures.find(index);
	return itTexture != m_pColorTextures.end() ? itTexture->second : NULL;
}

void CGfxFrameBuffer::Bind(void)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
}

void CGfxFrameBuffer::InvalidateFramebuffer(void)
{
	eastl::vector<GLenum> discardBuffers;

	for (const auto &itInvalidation : m_invalidations) {
		if (itInvalidation.second) {
			discardBuffers.push_back(GL_COLOR_ATTACHMENT0 + itInvalidation.first);
		}
	}

	glInvalidateFramebuffer(GL_FRAMEBUFFER, (GLsizei)discardBuffers.size(), discardBuffers.data());
}
