#include "GLES3Renderer.h"


CGLES3FrameBuffer::CGLES3FrameBuffer(CGLES3FrameBufferManager *pManager, int width, int height, int numAttachments)
	: CGfxFrameBuffer(width, height, numAttachments)
	, m_pManager(pManager)

	, m_width(width)
	, m_height(height)

	, m_fbo(0)
	, m_resolve(0)
{
	m_ptrAttachmentTextures.resize(numAttachments);
}

CGLES3FrameBuffer::~CGLES3FrameBuffer(void)
{
	Destroy();
}

void CGLES3FrameBuffer::Release(void)
{
	m_pManager->Destroy(this);
}

HANDLE CGLES3FrameBuffer::GetFrameBuffer(void)
{
	return (HANDLE)m_fbo;
}

int CGLES3FrameBuffer::GetWidth(void) const
{
	return m_width;
}

int CGLES3FrameBuffer::GetHeight(void) const
{
	return m_height;
}

bool CGLES3FrameBuffer::SetAttachmentTexture(int indexAttachment, CGfxRenderTexturePtr &ptrAttachmentTexture)
{
	if (ptrAttachmentTexture->GetWidth() == m_width && ptrAttachmentTexture->GetHeight() == m_height &&
		indexAttachment >= 0 && indexAttachment < (int)m_ptrAttachmentTextures.size()) {
		m_ptrAttachmentTextures[indexAttachment] = ptrAttachmentTexture;
		return true;
	}
	else {
		return false;
	}
}

CGfxRenderTexturePtr CGLES3FrameBuffer::GetAttachmentTexture(int indexAttachment) const
{
	if (indexAttachment >= 0 && indexAttachment < (int)m_ptrAttachmentTextures.size()) {
		return m_ptrAttachmentTextures[indexAttachment];
	}
	else {
		return nullptr;
	}
}

bool CGLES3FrameBuffer::Create(HANDLE hRenderPass)
{
	Destroy();

	glGenFramebuffers(1, &m_fbo);
	glGenFramebuffers(1, &m_resolve);

	return true;
}

void CGLES3FrameBuffer::Destroy(void)
{
	if (m_fbo) {
		glDeleteFramebuffers(1, &m_fbo);
	}

	if (m_resolve) {
		glDeleteFramebuffers(1, &m_resolve);
	}

	m_fbo = 0;
	m_resolve = 0;
}

void CGLES3FrameBuffer::Bind(const AttachmentInformation *pAttachmentInformations, const SubPassInformation *pSubPassInformation)
{
	GLBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	{
		uint32_t indexAttachment = 0;
		eastl::vector<uint32_t> drawBuffers;

		for (const auto &itOutputAttachment : pSubPassInformation->outputAttachments) {
			const CGfxRenderTexturePtr ptrOutputTexture = GetAttachmentTexture(itOutputAttachment.first);
			if (ptrOutputTexture.IsValid()) {
				drawBuffers.emplace_back(GL_COLOR_ATTACHMENT0 + indexAttachment);
				GLBindFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + indexAttachment, CGLES3Helper::TranslateTextureTarget(((CGLES3RenderTexture *)ptrOutputTexture.GetPointer())->GetType()), (GLuint)((CGLES3RenderTexture *)ptrOutputTexture.GetPointer())->GetTexture(), 0);

				if (pAttachmentInformations[itOutputAttachment.first].bClear) {
					glClearBufferfv(GL_COLOR, indexAttachment, pAttachmentInformations[itOutputAttachment.first].color);
				}

				indexAttachment++;
			}
		}

		const CGfxRenderTexturePtr ptrDepthStencilTexture = GetAttachmentTexture(pSubPassInformation->depthStencilAttachment);
		if (ptrDepthStencilTexture.IsValid()) {
			GLBindFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, CGLES3Helper::TranslateTextureTarget(((CGLES3RenderTexture *)ptrDepthStencilTexture.GetPointer())->GetType()), (GLuint)((CGLES3RenderTexture *)ptrDepthStencilTexture.GetPointer())->GetTexture(), 0);

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

void CGLES3FrameBuffer::Resolve(const AttachmentInformation *pAttachmentInformations, const SubPassInformation *pSubPassInformation)
{
	if (pSubPassInformation->resolveAttachments.empty()) {
		return;
	}

	GLBindFramebuffer(GL_FRAMEBUFFER, m_resolve);
	{
		uint32_t indexAttachment = 0;
		eastl::vector<uint32_t> drawBuffers;

		for (const auto &itResolveAttachment : pSubPassInformation->resolveAttachments) {
			const CGfxRenderTexturePtr ptrResolveTexture = GetAttachmentTexture(itResolveAttachment.first);
			if (ptrResolveTexture.IsValid()) {
				drawBuffers.emplace_back(GL_COLOR_ATTACHMENT0 + indexAttachment);
				GLBindFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + indexAttachment, CGLES3Helper::TranslateTextureTarget(((CGLES3RenderTexture *)ptrResolveTexture.GetPointer())->GetType()), (GLuint)((CGLES3RenderTexture *)ptrResolveTexture.GetPointer())->GetTexture(), 0);

				if (pAttachmentInformations[itResolveAttachment.first].bClear) {
					glClearBufferfv(GL_COLOR, indexAttachment, pAttachmentInformations[itResolveAttachment.first].color);
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

void CGLES3FrameBuffer::InvalidateFramebuffer(const AttachmentInformation *pAttachmentInformations, const SubPassInformation *pSubPassInformation)
{
	eastl::vector<uint32_t> discardBuffers;
	{
		uint32_t indexAttachment = 0;

		for (const auto &itOutputAttachment : pSubPassInformation->outputAttachments) {
			const CGfxRenderTexturePtr ptrOutputTexture = GetAttachmentTexture(itOutputAttachment.first);
			if (ptrOutputTexture.IsValid()) {
				if (pAttachmentInformations[itOutputAttachment.first].bInvalidation) {
					discardBuffers.emplace_back(GL_COLOR_ATTACHMENT0 + indexAttachment);
				}

				indexAttachment++;
			}
		}

		const CGfxRenderTexturePtr ptrDepthStencilTexture = GetAttachmentTexture(pSubPassInformation->depthStencilAttachment);
		if (ptrDepthStencilTexture.IsValid()) {
			if (pAttachmentInformations[pSubPassInformation->depthStencilAttachment].bInvalidation) {
				discardBuffers.emplace_back(GL_DEPTH_STENCIL_ATTACHMENT);
			}
		}
	}
	glInvalidateFramebuffer(GL_FRAMEBUFFER, (int)discardBuffers.size(), discardBuffers.data());
}
