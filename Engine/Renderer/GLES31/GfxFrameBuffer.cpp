#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxFrameBuffer.h"


CGfxFrameBuffer::CGfxFrameBuffer(int width, int height)
	: m_fbo(0)
	, m_resolve(0)

	, m_width(width)
	, m_height(height)
{
	glGenFramebuffers(1, &m_fbo);
	glGenFramebuffers(1, &m_resolve);
}

CGfxFrameBuffer::~CGfxFrameBuffer(void)
{
	glDeleteFramebuffers(1, &m_fbo);
	glDeleteFramebuffers(1, &m_resolve);
}

void CGfxFrameBuffer::Release(void)
{
	Renderer()->DestroyFrameBuffer(this);
}

bool CGfxFrameBuffer::SetAttachmentTexture(int indexAttachment, CGfxTexture2DPtr &ptrAttachmentTexture)
{
	if (ptrAttachmentTexture->GetWidth() == m_width && ptrAttachmentTexture->GetHeight() == m_height) {
		m_ptrAttachmentTextures[indexAttachment] = ptrAttachmentTexture;
		return true;
	}

	return false;
}

int CGfxFrameBuffer::GetWidth(void) const
{
	return m_width;
}

int CGfxFrameBuffer::GetHeight(void) const
{
	return m_height;
}

CGfxTexture2DPtr CGfxFrameBuffer::GetAttachmentTexture(int indexAttachment) const
{
	const auto &itAttachmentTexture = m_ptrAttachmentTextures.find(indexAttachment);
	return itAttachmentTexture != m_ptrAttachmentTextures.end() ? itAttachmentTexture->second : NULL;
}

void CGfxFrameBuffer::Bind(const AttachmentInformation *pAttachmentInformations, const SubPassInformation *pSubPassInformation) const
{
	GLBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	{
		uint32_t indexAttachment = 0;
		eastl::vector<uint32_t> drawBuffers;

		for (const auto &itAttachment : pSubPassInformation->outputAttachments) {
			const CGfxTexture2DPtr ptrColorTexture = GetAttachmentTexture(itAttachment.first);
			if (ptrColorTexture.IsValid()) {
				drawBuffers.emplace_back(GL_COLOR_ATTACHMENT0 + indexAttachment);
				GLBindFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + indexAttachment, ptrColorTexture->GetTarget(), ptrColorTexture->GetTexture(), 0);

				if (pAttachmentInformations[itAttachment.first].bClear) {
					glClearBufferfv(GL_COLOR, indexAttachment, pAttachmentInformations[itAttachment.first].color);
				}

				indexAttachment++;
			}
		}

		const CGfxTexture2DPtr ptrDepthStencilTexture = GetAttachmentTexture(pSubPassInformation->depthStencilAttachment);
		if (ptrDepthStencilTexture.IsValid()) {
			GLBindFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, ptrDepthStencilTexture->GetTarget(), ptrDepthStencilTexture->GetTexture(), 0);

			if (pAttachmentInformations[pSubPassInformation->depthStencilAttachment].bClear) {
				glClearBufferfi(GL_DEPTH_STENCIL, 0, pAttachmentInformations[pSubPassInformation->depthStencilAttachment].depth, pAttachmentInformations[pSubPassInformation->depthStencilAttachment].stencil);
			}
		}

		GLReadBuffers(GL_FRAMEBUFFER, (int)drawBuffers.size(), drawBuffers.data());
		GLDrawBuffers(GL_FRAMEBUFFER, (int)drawBuffers.size(), drawBuffers.data());

		uint32_t status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		bool bValid = status == GL_FRAMEBUFFER_COMPLETE;
	}
}

void CGfxFrameBuffer::Resolve(const AttachmentInformation *pAttachmentInformations, const SubPassInformation *pSubPassInformation) const
{
	if (pSubPassInformation->resolveAttachments.empty()) {
		return;
	}

	GLBindFramebuffer(GL_FRAMEBUFFER, m_resolve);
	{
		uint32_t indexAttachment = 0;
		eastl::vector<uint32_t> drawBuffers;

		for (const auto &itAttachment : pSubPassInformation->resolveAttachments) {
			const CGfxTexture2DPtr ptrColorTexture = GetAttachmentTexture(itAttachment.first);
			if (ptrColorTexture.IsValid()) {
				drawBuffers.emplace_back(GL_COLOR_ATTACHMENT0 + indexAttachment);
				GLBindFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + indexAttachment, ptrColorTexture->GetTarget(), ptrColorTexture->GetTexture(), 0);
					
				if (pAttachmentInformations[itAttachment.first].bClear) {
					glClearBufferfv(GL_COLOR, indexAttachment, pAttachmentInformations[itAttachment.first].color);
				}

				indexAttachment++;
			}
		}

		GLReadBuffers(GL_FRAMEBUFFER, (int)drawBuffers.size(), drawBuffers.data());
		GLDrawBuffers(GL_FRAMEBUFFER, (int)drawBuffers.size(), drawBuffers.data());

		uint32_t status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		bool bValid = status == GL_FRAMEBUFFER_COMPLETE;
	}

	GLBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
	GLBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_resolve);
	{
		glBlitFramebuffer(
			0, 0, m_width, m_height,
			0, 0, m_width, m_height,
			GL_COLOR_BUFFER_BIT, GL_NEAREST);
	}
}

void CGfxFrameBuffer::InvalidateFramebuffer(const AttachmentInformation *pAttachmentInformations, const SubPassInformation *pSubPassInformation) const
{
	eastl::vector<uint32_t> discardBuffers;
	{
		uint32_t indexAttachment = 0;

		for (const auto &itAttachment : pSubPassInformation->outputAttachments) {
			const CGfxTexture2DPtr ptrColorTexture = GetAttachmentTexture(itAttachment.first);
			if (ptrColorTexture.IsValid()) {
				if (pAttachmentInformations[itAttachment.first].bInvalidation) {
					discardBuffers.emplace_back(GL_COLOR_ATTACHMENT0 + indexAttachment);
				}

				indexAttachment++;
			}
		}

		const CGfxTexture2DPtr ptrDepthStencilTexture = GetAttachmentTexture(pSubPassInformation->depthStencilAttachment);
		if (ptrDepthStencilTexture.IsValid()) {
			if (pAttachmentInformations[pSubPassInformation->depthStencilAttachment].bInvalidation) {
				discardBuffers.emplace_back(GL_DEPTH_STENCIL_ATTACHMENT);
			}
		}
	}
	glInvalidateFramebuffer(GL_FRAMEBUFFER, (int)discardBuffers.size(), discardBuffers.data());
}
