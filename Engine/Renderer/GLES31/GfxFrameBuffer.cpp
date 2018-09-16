#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxFrameBuffer.h"


static void glReadBuffers(GLsizei n, const GLenum *bufs)
{
	for (int index = 0; index < n; index++) {
		glReadBuffer(bufs[index]);
	}
}

CGfxFrameBuffer::CGfxFrameBuffer(GLuint width, GLuint height)
	: m_width(width)
	, m_height(height)

	, m_fbo(0)
	, m_rbo(0)

	, refCount(0)
{
	glGenRenderbuffers(1, &m_rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_width, m_height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_rbo);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

CGfxFrameBuffer::~CGfxFrameBuffer(void)
{
	glDeleteFramebuffers(1, &m_fbo);
	glDeleteRenderbuffers(1, &m_rbo);

	for (const auto &itTexture : m_pTextures) {
		itTexture.second->Release();
	}

	m_pTextures.clear();
	m_invalidations.clear();
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

bool CGfxFrameBuffer::SetRenderTexture(GLuint index, CGfxTexture2D *pTexture, bool invalidation)
{
	if (m_pTextures.find(index) != m_pTextures.end()) {
		return false;
	}

	if (pTexture->GetWidth() != m_width || pTexture->GetHeight() != m_height) {
		return false;
	}

	pTexture->Retain();

	m_pTextures[index] = pTexture;
	m_invalidations[index] = invalidation;

	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	{
		eastl::vector<GLenum> drawBuffers;

		for (const auto &itTexture : m_pTextures) {
			drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + itTexture.first);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + itTexture.first, GL_TEXTURE_2D, itTexture.second->GetTexture(), 0);
		}

		glReadBuffers((GLsizei)drawBuffers.size(), drawBuffers.data());
		glDrawBuffers((GLsizei)drawBuffers.size(), drawBuffers.data());
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return CheckFramebufferStatus();
}

bool CGfxFrameBuffer::CheckFramebufferStatus(void)
{
	GLenum status;

	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	{
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
