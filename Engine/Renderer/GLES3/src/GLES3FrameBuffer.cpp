#include "GLES3Renderer.h"


CGLES3FrameBuffer::CGLES3FrameBuffer(CGLES3FrameBufferManager* pManager, int width, int height, int numAttachments)
	: CGfxFrameBuffer(width, height, numAttachments)
	, m_pManager(pManager)

	, m_fbo(0)
	, m_resolve(0)

	, m_width(width)
	, m_height(height)
	, m_ptrAttachmentTextures(numAttachments)
{

}

CGLES3FrameBuffer::~CGLES3FrameBuffer(void)
{
	Destroy(true);
}

void CGLES3FrameBuffer::Release(void)
{
	m_pManager->Destroy(this);
}

int CGLES3FrameBuffer::GetWidth(void) const
{
	return m_width;
}

int CGLES3FrameBuffer::GetHeight(void) const
{
	return m_height;
}

bool CGLES3FrameBuffer::Create(const CGfxRenderPassPtr ptrRenderPass)
{
	ASSERT(ptrRenderPass);

	Destroy(false);

	glGenFramebuffers(1, &m_fbo);
	glGenFramebuffers(1, &m_resolve);
	CHECK_GL_ERROR_ASSERT();

	return true;
}

void CGLES3FrameBuffer::Destroy(bool bClear)
{
	if (bClear) {
		m_ptrAttachmentTextures.clear();
	}

	if (m_fbo) {
		glDeleteFramebuffers(1, &m_fbo);
	}

	if (m_resolve) {
		glDeleteFramebuffers(1, &m_resolve);
	}

	m_fbo = 0;
	m_resolve = 0;
}

bool CGLES3FrameBuffer::SetAttachmentTexture(int indexAttachment, const CGfxRenderTexturePtr ptrAttachmentTexture)
{
	ASSERT(ptrAttachmentTexture);

	if (indexAttachment >= 0 && indexAttachment < m_ptrAttachmentTextures.size() && ptrAttachmentTexture->GetWidth() == m_width && ptrAttachmentTexture->GetHeight() == m_height) {
		m_ptrAttachmentTextures[indexAttachment] = ptrAttachmentTexture;
		return true;
	}
	else {
		return false;
	}
}

const CGfxRenderTexturePtr CGLES3FrameBuffer::GetAttachmentTexture(int indexAttachment) const
{
	if (indexAttachment >= 0 && indexAttachment < m_ptrAttachmentTextures.size()) {
		return m_ptrAttachmentTextures[indexAttachment];
	}
	else {
		return nullptr;
	}
}

void CGLES3FrameBuffer::Bind(const AttachmentInformation* pAttachmentInformations, const SubpassInformation* pSubpassInformation) const
{
	ASSERT(pAttachmentInformations);
	ASSERT(pSubpassInformation);
	ASSERT(m_fbo);

	eastl::unordered_map<int, ClearValue> attachmentClearValues;

	GLScissor(0, 0, m_width, m_height);
	GLViewport(0, 0, m_width, m_height);

	GLBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	{
		eastl::vector<uint32_t> drawBuffers;
		drawBuffers.reserve(pSubpassInformation->outputAttachments.size());
		{
			uint32_t indexAttachment = 0;

			for (const auto& itOutputAttachment : pSubpassInformation->outputAttachments) {
				if (const CGfxRenderTexturePtr ptrOutputTexture = GetAttachmentTexture(itOutputAttachment.first)) {
					ASSERT(CGfxHelper::IsFormatColor(ptrOutputTexture->GetFormat()));

					GLBindFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + indexAttachment, CGLES3Helper::TranslateTextureTarget(((CGLES3RenderTexture*)ptrOutputTexture.GetPointer())->GetType()), ((CGLES3RenderTexture*)ptrOutputTexture.GetPointer())->GetTexture(), 0);
					if (pAttachmentInformations[itOutputAttachment.first].bClear) {
						attachmentClearValues[indexAttachment] = pAttachmentInformations[itOutputAttachment.first].clearValue;
					}

					drawBuffers.emplace_back(GL_COLOR_ATTACHMENT0 + indexAttachment);
					indexAttachment++;
				}
			}

			if (const CGfxRenderTexturePtr ptrDepthStencilTexture = GetAttachmentTexture(pSubpassInformation->depthStencilAttachment)) {
				ASSERT(CGfxHelper::IsFormatDepthOrStencil(ptrDepthStencilTexture->GetFormat()));

				if (CGfxHelper::IsFormatDepthOnly(ptrDepthStencilTexture->GetFormat())) {
					GLBindFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, CGLES3Helper::TranslateTextureTarget(((CGLES3RenderTexture*)ptrDepthStencilTexture.GetPointer())->GetType()), ((CGLES3RenderTexture*)ptrDepthStencilTexture.GetPointer())->GetTexture(), 0);
					if (pAttachmentInformations[pSubpassInformation->depthStencilAttachment].bClear) {
						attachmentClearValues[GL_DEPTH] = pAttachmentInformations[pSubpassInformation->depthStencilAttachment].clearValue;
					}
				}
				else if (CGfxHelper::IsFormatStencilOnly(ptrDepthStencilTexture->GetFormat())) {
					GLBindFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, CGLES3Helper::TranslateTextureTarget(((CGLES3RenderTexture*)ptrDepthStencilTexture.GetPointer())->GetType()), ((CGLES3RenderTexture*)ptrDepthStencilTexture.GetPointer())->GetTexture(), 0);
					if (pAttachmentInformations[pSubpassInformation->depthStencilAttachment].bClear) {
						attachmentClearValues[GL_STENCIL] = pAttachmentInformations[pSubpassInformation->depthStencilAttachment].clearValue;
					}
				}
				else if (CGfxHelper::IsFormatDepthAndStencil(ptrDepthStencilTexture->GetFormat())) {
					GLBindFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, CGLES3Helper::TranslateTextureTarget(((CGLES3RenderTexture*)ptrDepthStencilTexture.GetPointer())->GetType()), ((CGLES3RenderTexture*)ptrDepthStencilTexture.GetPointer())->GetTexture(), 0);
					if (pAttachmentInformations[pSubpassInformation->depthStencilAttachment].bClear) {
						attachmentClearValues[GL_DEPTH_STENCIL] = pAttachmentInformations[pSubpassInformation->depthStencilAttachment].clearValue;
					}
				}
			}
		}
		GLReadBuffers(GL_FRAMEBUFFER, drawBuffers.size(), drawBuffers.data());
		GLDrawBuffers(GL_FRAMEBUFFER, drawBuffers.size(), drawBuffers.data());

		uint32_t status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		ASSERT(status == GL_FRAMEBUFFER_COMPLETE);
	}
	CHECK_GL_ERROR_ASSERT();

	for (const auto& itClearValue : attachmentClearValues) {
		switch (itClearValue.first) {
		case GL_DEPTH:
			glClearBufferfv(GL_DEPTH, 0, &itClearValue.second.depth);
			break;

		case GL_STENCIL:
			glClearBufferiv(GL_STENCIL, 0, &itClearValue.second.stencil);
			break;

		case GL_DEPTH_STENCIL:
			glClearBufferfi(GL_DEPTH_STENCIL, 0, itClearValue.second.depth, itClearValue.second.stencil);
			break;

		default:
			glClearBufferfv(GL_COLOR, itClearValue.first, itClearValue.second.color);
			break;
		}
	}
}

void CGLES3FrameBuffer::Resolve(const AttachmentInformation* pAttachmentInformations, const SubpassInformation* pSubpassInformation) const
{
	ASSERT(pAttachmentInformations);
	ASSERT(pSubpassInformation);
	ASSERT(m_resolve);

	if (pSubpassInformation->resolveAttachments.empty()) {
		return;
	}

	GLBindFramebuffer(GL_FRAMEBUFFER, m_resolve);
	{
		eastl::vector<uint32_t> drawBuffers;
		drawBuffers.reserve(pSubpassInformation->resolveAttachments.size());
		{
			uint32_t indexAttachment = 0;

			for (const auto& itResolveAttachment : pSubpassInformation->resolveAttachments) {
				if (const CGfxRenderTexturePtr ptrResolveTexture = GetAttachmentTexture(itResolveAttachment.first)) {
					ASSERT(CGfxHelper::IsFormatColor(ptrResolveTexture->GetFormat()));

					GLBindFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + indexAttachment, CGLES3Helper::TranslateTextureTarget(((CGLES3RenderTexture*)ptrResolveTexture.GetPointer())->GetType()), ((CGLES3RenderTexture*)ptrResolveTexture.GetPointer())->GetTexture(), 0);

					drawBuffers.emplace_back(GL_COLOR_ATTACHMENT0 + indexAttachment);
					indexAttachment++;
				}
			}
		}
		GLReadBuffers(GL_FRAMEBUFFER, drawBuffers.size(), drawBuffers.data());
		GLDrawBuffers(GL_FRAMEBUFFER, drawBuffers.size(), drawBuffers.data());

		uint32_t status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		ASSERT(status == GL_FRAMEBUFFER_COMPLETE);
	}
	CHECK_GL_ERROR_ASSERT();

	GLBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
	GLBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_resolve);
	{
		glBlitFramebuffer(
			0, 0, m_width, m_height,
			0, 0, m_width, m_height,
			GL_COLOR_BUFFER_BIT, GL_NEAREST);
	}
	CHECK_GL_ERROR_ASSERT();
}

void CGLES3FrameBuffer::InvalidateFramebuffer(const AttachmentInformation* pAttachmentInformations, const SubpassInformation* pSubpassInformation) const
{
	ASSERT(pAttachmentInformations);
	ASSERT(pSubpassInformation);

	eastl::vector<uint32_t> discardBuffers;
	discardBuffers.reserve(pSubpassInformation->outputAttachments.size());
	{
		uint32_t indexAttachment = 0;

		for (const auto& itOutputAttachment : pSubpassInformation->outputAttachments) {
			if (const CGfxRenderTexturePtr ptrOutputTexture = GetAttachmentTexture(itOutputAttachment.first)) {
				if (pAttachmentInformations[itOutputAttachment.first].bInvalidation) {
					discardBuffers.emplace_back(GL_COLOR_ATTACHMENT0 + indexAttachment);
				}

				indexAttachment++;
			}
		}

		if (const CGfxRenderTexturePtr ptrDepthStencilTexture = GetAttachmentTexture(pSubpassInformation->depthStencilAttachment)) {
			if (pAttachmentInformations[pSubpassInformation->depthStencilAttachment].bInvalidation) {
				discardBuffers.emplace_back(GL_DEPTH_STENCIL_ATTACHMENT);
			}
		}
	}
	glInvalidateFramebuffer(GL_FRAMEBUFFER, (int)discardBuffers.size(), discardBuffers.data());
	CHECK_GL_ERROR_ASSERT();
}
