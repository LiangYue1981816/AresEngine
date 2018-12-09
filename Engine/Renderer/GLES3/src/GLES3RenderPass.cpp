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

}

void CGLES3RenderPass::Release(void)
{
	m_pManager->Destroy(this);
}

bool CGLES3RenderPass::SetColorAttachment(int indexAttachment, bool bInvalidation, bool bClear, float red, float green, float blue, float alpha)
{
	if (indexAttachment >= (int)m_attachments.size()) {
		return false;
	}

	m_attachments[indexAttachment].bInvalidation = bInvalidation;
	m_attachments[indexAttachment].bClear = bClear;
	m_attachments[indexAttachment].color[0] = red;
	m_attachments[indexAttachment].color[1] = green;
	m_attachments[indexAttachment].color[2] = blue;
	m_attachments[indexAttachment].color[3] = alpha;

	return true;
}

bool CGLES3RenderPass::SetDepthStencilAttachment(int indexAttachment, bool bInvalidation, bool bClear, float depth, int stencil)
{
	if (indexAttachment >= (int)m_attachments.size()) {
		return false;
	}

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

bool CGLES3RenderPass::SetSubpassResolveColorReference(int indexSubPass, int indexAttachment)
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
	return indexAttachment >= 0 && indexAttachment < (int)m_attachments.size() ? &m_attachments[indexAttachment] : nullptr;
}

uint32_t CGLES3RenderPass::GetSubPassCount(void) const
{
	return m_subpasses.size();
}

uint32_t CGLES3RenderPass::GetSubpassInputAttachmentCount(int indexSubPass) const
{
	return indexSubPass >= 0 && indexSubPass < (int)m_subpasses.size() ? m_subpasses[indexSubPass].inputAttachments.size() : 0;
}

uint32_t CGLES3RenderPass::GetSubpassOutputAttachmentCount(int indexSubPass) const
{
	return indexSubPass >= 0 && indexSubPass < (int)m_subpasses.size() ? m_subpasses[indexSubPass].outputAttachments.size() : 0;
}

const SubPassInformation* CGLES3RenderPass::GetSubPass(int indexSubPass) const
{
	return indexSubPass >= 0 && indexSubPass < (int)m_subpasses.size() ? &m_subpasses[indexSubPass] : nullptr;
}
