#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxFrameBuffer.h"


CGfxFrameBuffer::CGfxFrameBuffer(uint32_t width, uint32_t height)
	: m_fbo(0)

	, m_width(width)
	, m_height(height)
{
	glGenFramebuffers(1, &m_fbo);
}

CGfxFrameBuffer::~CGfxFrameBuffer(void)
{
	glDeleteFramebuffers(1, &m_fbo);
}

void CGfxFrameBuffer::Release(void)
{
	Renderer()->DestroyFrameBuffer(this);
}

bool CGfxFrameBuffer::SetAttachmentTexture(uint32_t indexAttachment, CGfxTexture2DPtr &ptrAttachmentTexture)
{
	if (ptrAttachmentTexture->GetWidth() == m_width && ptrAttachmentTexture->GetHeight() == m_height) {
		m_ptrAttachmentTextures[indexAttachment] = ptrAttachmentTexture;
		return true;
	}

	return false;
}

uint32_t CGfxFrameBuffer::GetFBO(void) const
{
	return m_fbo;
}

uint32_t CGfxFrameBuffer::GetWidth(void) const
{
	return m_width;
}

uint32_t CGfxFrameBuffer::GetHeight(void) const
{
	return m_height;
}

CGfxTexture2DPtr CGfxFrameBuffer::GetAttachmentTexture(uint32_t indexAttachment) const
{
	const auto &itAttachmentTexture = m_ptrAttachmentTextures.find(indexAttachment);
	return itAttachmentTexture != m_ptrAttachmentTextures.end() ? itAttachmentTexture->second : NULL;
}

void CGfxFrameBuffer::Bind(const AttachmentInformation *pAttachmentInformations, const SubPassInformation *pSubPassInformation) const
{
	GLBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
	{
		eastl::vector<uint32_t> drawBuffers;

		for (const auto &itAttachment : pSubPassInformation->outputAttachments) {
			CGfxTexture2DPtr ptrColorTexture = GetAttachmentTexture(itAttachment.first);
			if (ptrColorTexture.IsValid()) {
				drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + itAttachment.first);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + itAttachment.first, ptrColorTexture->GetTarget(), ptrColorTexture->GetTexture(), 0);

				if (pAttachmentInformations[itAttachment.first].bClear) {
					glClearBufferfv(GL_COLOR, GL_COLOR_ATTACHMENT0 + itAttachment.first, pAttachmentInformations[itAttachment.first].color);
				}
			}
		}

		CGfxTexture2DPtr ptrDepthStencilTexture = GetAttachmentTexture(pSubPassInformation->depthStencilAttachment);
		if (ptrDepthStencilTexture.IsValid()) {
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, ptrDepthStencilTexture->GetTarget(), ptrDepthStencilTexture->GetTexture(), 0);

			if (pAttachmentInformations[pSubPassInformation->depthStencilAttachment].bClear) {
				glClearBufferfi(GL_DEPTH_STENCIL, 0, pAttachmentInformations[pSubPassInformation->depthStencilAttachment].depth, pAttachmentInformations[pSubPassInformation->depthStencilAttachment].stencil);
			}
		}

		glReadBuffers((int)drawBuffers.size(), drawBuffers.data());
		glDrawBuffers((int)drawBuffers.size(), drawBuffers.data());

		uint32_t status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		bool bValid = status == GL_FRAMEBUFFER_COMPLETE;
	}
}

void CGfxFrameBuffer::InvalidateFramebuffer(const AttachmentInformation *pAttachmentInformations, const SubPassInformation *pSubPassInformation) const
{
	eastl::vector<uint32_t> discardBuffers;
	{
		for (const auto &itAttachment : pSubPassInformation->outputAttachments) {
			CGfxTexture2DPtr ptrColorTexture = GetAttachmentTexture(itAttachment.first);
			if (ptrColorTexture.IsValid()) {
				if (pAttachmentInformations[itAttachment.first].bInvalidation) {
					discardBuffers.push_back(GL_COLOR_ATTACHMENT0 + itAttachment.first);
				}
			}
		}

		CGfxTexture2DPtr ptrDepthStencilTexture = GetAttachmentTexture(pSubPassInformation->depthStencilAttachment);
		if (ptrDepthStencilTexture.IsValid()) {
			if (pAttachmentInformations[pSubPassInformation->depthStencilAttachment].bInvalidation) {
				discardBuffers.push_back(GL_DEPTH_STENCIL_ATTACHMENT);
			}
		}
	}
	glInvalidateFramebuffer(GL_DRAW_FRAMEBUFFER, (int)discardBuffers.size(), discardBuffers.data());
}
