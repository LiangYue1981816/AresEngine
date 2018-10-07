#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxFrameBuffer.h"


CGfxFrameBuffer::CGfxFrameBuffer(uint32_t width, uint32_t height, bool bDepthStencilRenderBuffer)
	: m_fbo(0)
	, m_rbo(0)

	, m_width(width)
	, m_height(height)
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

bool CGfxFrameBuffer::SetDepthStencilTexture(CGfxTexture2DPtr &ptrTexture, bool bInvalidation, bool bClear, float depth, int stencil)
{
	if (ptrTexture.IsValid()) {
		if (ptrTexture->GetWidth() != m_width || ptrTexture->GetHeight() != m_height) {
			return false;
		}
	}

	m_attachmentDepthStencil.ptrTexture = ptrTexture;
	m_attachmentDepthStencil.bInvalidation = ptrTexture.IsValid() && bInvalidation;
	m_attachmentDepthStencil.bClear = bClear;
	m_attachmentDepthStencil.depth = depth;
	m_attachmentDepthStencil.stencil = stencil;

	return true;
}

bool CGfxFrameBuffer::SetColorTexture(uint32_t index, CGfxTexture2DPtr &ptrTexture, bool bInvalidation, bool bClear, float red, float green, float blue, float alpha)
{
	if (ptrTexture.IsValid()) {
		if (ptrTexture->GetWidth() != m_width || ptrTexture->GetHeight() != m_height) {
			return false;
		}
	}

	m_attachmentColors[index].ptrTexture = ptrTexture;
	m_attachmentColors[index].bInvalidation = ptrTexture.IsValid() && bInvalidation;
	m_attachmentColors[index].bClear = bClear;
	m_attachmentColors[index].color[0] = red;
	m_attachmentColors[index].color[1] = green;
	m_attachmentColors[index].color[2] = blue;
	m_attachmentColors[index].color[3] = alpha;

	return true;
}

bool CGfxFrameBuffer::Apply(void)
{
	uint32_t status;

	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	{
		eastl::vector<uint32_t> drawBuffers;

		for (const auto &itAttachment : m_attachmentColors) {
			if (itAttachment.second.ptrTexture.IsValid()) {
				drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + itAttachment.first);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + itAttachment.first, GL_TEXTURE_2D, itAttachment.second.ptrTexture->GetTexture(), 0);
			}
		}

		if (m_attachmentDepthStencil.ptrTexture.IsValid() || m_rbo) {
			if (m_attachmentDepthStencil.ptrTexture.IsValid()) {
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_attachmentDepthStencil.ptrTexture->GetTexture(), 0);
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

CGfxTexture2DPtr CGfxFrameBuffer::GetDepthStencilTexture(void) const
{
	return m_attachmentDepthStencil.ptrTexture;
}

CGfxTexture2DPtr CGfxFrameBuffer::GetColorTexture(uint32_t index) const
{
	const auto &itAttachment = m_attachmentColors.find(index);
	return itAttachment != m_attachmentColors.end() ? itAttachment->second.ptrTexture : NULL;
}

void CGfxFrameBuffer::Bind(void) const
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

	for (const auto &itAttachment : m_attachmentColors) {
		if (itAttachment.second.ptrTexture.IsValid() && itAttachment.second.bClear) {
			glClearBufferfv(GL_COLOR, GL_COLOR_ATTACHMENT0 + itAttachment.first, itAttachment.second.color);
		}
	}

	if (m_attachmentDepthStencil.ptrTexture.IsValid() && m_attachmentDepthStencil.bClear) {
		glClearBufferfi(GL_DEPTH_STENCIL, 0, m_attachmentDepthStencil.depth, m_attachmentDepthStencil.stencil);
	}
}

void CGfxFrameBuffer::InvalidateFramebuffer(void) const
{
	eastl::vector<uint32_t> discardBuffers;

	for (const auto &itAttachment : m_attachmentColors) {
		if (itAttachment.second.ptrTexture.IsValid() && itAttachment.second.bInvalidation) {
			discardBuffers.push_back(GL_COLOR_ATTACHMENT0 + itAttachment.first);
		}
	}

	if (m_attachmentDepthStencil.ptrTexture.IsValid() || m_rbo) {
		if (m_attachmentDepthStencil.ptrTexture.IsValid()) {
			if (m_attachmentDepthStencil.bInvalidation) {
				discardBuffers.push_back(GL_DEPTH_STENCIL_ATTACHMENT);
			}
		}
		else {
			discardBuffers.push_back(GL_DEPTH_STENCIL_ATTACHMENT);
		}
	}

	glInvalidateFramebuffer(GL_FRAMEBUFFER, (int)discardBuffers.size(), discardBuffers.data());
}
