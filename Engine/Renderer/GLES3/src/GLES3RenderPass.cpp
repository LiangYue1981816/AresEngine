#include "GLES3Renderer.h"


CGLES3RenderPass::CGLES3RenderPass(CGLES3RenderPassManager *pManager, int numAttachments, int numSubpasses)
	: CGfxRenderPass(numAttachments, numSubpasses)
	, m_pManager(pManager)
{
	m_attachments.resize(numAttachments);
	m_subpasses.resize(numAttachments);
}

CGLES3RenderPass::~CGLES3RenderPass(void)
{
	Destroy();
}

void CGLES3RenderPass::Release(void)
{
	m_pManager->Destroy(this);
}

HANDLE CGLES3RenderPass::GetRenderPass(void) const
{
	return 0;
}

bool CGLES3RenderPass::SetColorAttachment(int indexAttachment, GfxPixelFormat pixelFormat, int samples, bool bInvalidation, bool bClear, float red, float green, float blue, float alpha)
{
	if (indexAttachment >= (int)m_attachments.size()) {
		return false;
	}

	m_attachments[indexAttachment].pixelFormat = pixelFormat;
	m_attachments[indexAttachment].samples = samples;
	m_attachments[indexAttachment].bInvalidation = bInvalidation;
	m_attachments[indexAttachment].bClear = bClear;
	m_attachments[indexAttachment].color[0] = red;
	m_attachments[indexAttachment].color[1] = green;
	m_attachments[indexAttachment].color[2] = blue;
	m_attachments[indexAttachment].color[3] = alpha;

	return true;
}

bool CGLES3RenderPass::SetDepthStencilAttachment(int indexAttachment, GfxPixelFormat pixelFormat, int samples, bool bInvalidation, bool bClear, float depth, int stencil)
{
	if (indexAttachment >= (int)m_attachments.size()) {
		return false;
	}

	m_attachments[indexAttachment].pixelFormat = pixelFormat;
	m_attachments[indexAttachment].samples = samples;
	m_attachments[indexAttachment].bInvalidation = bInvalidation;
	m_attachments[indexAttachment].bClear = bClear;
	m_attachments[indexAttachment].depth = depth;
	m_attachments[indexAttachment].stencil = stencil;

	return true;
}

bool CGLES3RenderPass::SetSubpassInputColorReference(int indexSubPass, int indexAttachment, const char *szName)
{
	if (indexSubPass >= (int)m_subpasses.size()) {
		return false;
	}

	if (indexAttachment >= (int)m_attachments.size()) {
		return false;
	}

	m_subpasses[indexSubPass].inputAttachments[indexAttachment] = szName;
	return true;
}

bool CGLES3RenderPass::SetSubpassOutputColorReference(int indexSubPass, int indexAttachment)
{
	if (indexSubPass >= (int)m_subpasses.size()) {
		return false;
	}

	if (indexAttachment >= (int)m_attachments.size()) {
		return false;
	}

	m_subpasses[indexSubPass].outputAttachments[indexAttachment] = indexAttachment;
	return true;
}

bool CGLES3RenderPass::SetSubpassOutputDepthStencilReference(int indexSubPass, int indexAttachment)
{
	if (indexSubPass >= (int)m_subpasses.size()) {
		return false;
	}

	if (indexAttachment >= (int)m_attachments.size()) {
		return false;
	}

	m_subpasses[indexSubPass].depthStencilAttachment = indexAttachment;
	return true;
}

bool CGLES3RenderPass::SetSubpassResolveReference(int indexSubPass, int indexAttachment)
{
	if (indexSubPass >= (int)m_subpasses.size()) {
		return false;
	}

	if (indexAttachment >= (int)m_attachments.size()) {
		return false;
	}

	m_subpasses[indexSubPass].resolveAttachments[indexAttachment] = indexAttachment;
	return true;
}

bool CGLES3RenderPass::SetSubpassPreserveReference(int indexSubPass, int indexAttachment)
{
	return true;
}

uint32_t CGLES3RenderPass::GetAttachmentCount(void) const
{
	return m_attachments.size();
}

const AttachmentInformation* CGLES3RenderPass::GetAttachments(void) const
{
	return m_attachments.data();
}

const AttachmentInformation* CGLES3RenderPass::GetAttachment(int indexAttachment) const
{
	if (indexAttachment >= 0 && indexAttachment < (int)m_attachments.size()) {
		return &m_attachments[indexAttachment];
	}
	else {
		return nullptr;
	}
}

uint32_t CGLES3RenderPass::GetSubPassCount(void) const
{
	return m_subpasses.size();
}

uint32_t CGLES3RenderPass::GetSubpassInputAttachmentCount(int indexSubPass) const
{
	if (indexSubPass >= 0 && indexSubPass < (int)m_subpasses.size()) {
		return m_subpasses[indexSubPass].inputAttachments.size();
	}
	else {
		return 0;
	}
}

uint32_t CGLES3RenderPass::GetSubpassOutputAttachmentCount(int indexSubPass) const
{
	if (indexSubPass >= 0 && indexSubPass < (int)m_subpasses.size()) {
		return m_subpasses[indexSubPass].outputAttachments.size();
	}
	else {
		return 0;
	}
}

const SubPassInformation* CGLES3RenderPass::GetSubPass(int indexSubPass) const
{
	if (indexSubPass >= 0 && indexSubPass < (int)m_subpasses.size()) {
		return &m_subpasses[indexSubPass];
	}
	else {
		return nullptr;
	}
}

bool CGLES3RenderPass::Create(void)
{
	Destroy();
	return true;
}

void CGLES3RenderPass::Destroy(void)
{

}
