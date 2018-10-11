#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxRenderPass.h"


CGfxRenderPass::CGfxRenderPass(uint32_t numAttachments, uint32_t numSubpasses)
{
	m_attachments.resize(numAttachments);
	m_subpasses.resize(numAttachments);
}

CGfxRenderPass::~CGfxRenderPass(void)
{

}

void CGfxRenderPass::Release(void)
{

}

bool CGfxRenderPass::SetPresentAttachment(uint32_t indexAttachment, bool bInvalidation, bool bClear, float red, float green, float blue, float alpha)
{
	if (indexAttachment >= m_attachments.size()) {
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

bool CGfxRenderPass::SetColorAttachment(uint32_t indexAttachment, bool bInvalidation, bool bClear, float red, float green, float blue, float alpha)
{
	if (indexAttachment >= m_attachments.size()) {
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

bool CGfxRenderPass::SetDepthStencilAttachment(uint32_t indexAttachment, bool bInvalidation, bool bClear, float depth, int stencil)
{
	if (indexAttachment >= m_attachments.size()) {
		return false;
	}

	m_attachments[indexAttachment].bInvalidation = bInvalidation;
	m_attachments[indexAttachment].bClear = bClear;
	m_attachments[indexAttachment].depth = depth;
	m_attachments[indexAttachment].stencil = stencil;

	return true;
}

bool CGfxRenderPass::SetSubpassInputColorReference(uint32_t indexSubPass, uint32_t indexAttachment)
{
	if (indexSubPass >= m_subpasses.size()) {
		return false;
	}

	if (indexAttachment >= m_attachments.size()) {
		return false;
	}

	m_subpasses[indexSubPass].inputAttachments[indexAttachment] = indexAttachment;

	return true;
}

bool CGfxRenderPass::SetSubpassInputDepthStencilReference(uint32_t indexSubPass, uint32_t indexAttachment)
{
	if (indexSubPass >= m_subpasses.size()) {
		return false;
	}

	if (indexAttachment >= m_attachments.size()) {
		return false;
	}

	m_subpasses[indexSubPass].inputAttachments[indexAttachment] = indexAttachment;

	return true;
}

bool CGfxRenderPass::SetSubpassOutputColorReference(uint32_t indexSubPass, uint32_t indexAttachment)
{
	if (indexSubPass >= m_subpasses.size()) {
		return false;
	}

	if (indexAttachment >= m_attachments.size()) {
		return false;
	}

	m_subpasses[indexSubPass].colorAttachments[indexAttachment] = indexAttachment;

	return true;
}

bool CGfxRenderPass::SetSubpassOutputDepthStencilReference(uint32_t indexSubPass, uint32_t indexAttachment)
{
	if (indexSubPass >= m_subpasses.size()) {
		return false;
	}

	if (indexAttachment >= m_attachments.size()) {
		return false;
	}

	m_subpasses[indexSubPass].depthStencilAttachment = indexAttachment;

	return true;
}

bool CGfxRenderPass::SetSubpassResolveColorReference(uint32_t indexSubPass, uint32_t indexAttachment)
{
	if (indexSubPass >= m_subpasses.size()) {
		return false;
	}

	if (indexAttachment >= m_attachments.size()) {
		return false;
	}

	m_subpasses[indexSubPass].resolveAttachments[indexAttachment] = indexAttachment;

	return true;
}

uint32_t CGfxRenderPass::GetSubPassCount(void) const
{
	return m_subpasses.size();
}

uint32_t CGfxRenderPass::GetSubpassInputAttachmentCount(uint32_t indexSubPass) const
{
	return indexSubPass < m_subpasses.size() ? m_subpasses[indexSubPass].inputAttachments.size() : 0;
}

uint32_t CGfxRenderPass::GetSubpassOutputAttachmentCount(uint32_t indexSubPass) const
{
	return indexSubPass < m_subpasses.size() ? m_subpasses[indexSubPass].colorAttachments.size() : 0;
}

const SubPassInformation* CGfxRenderPass::GetSubPass(int indexSubPass) const
{
	return indexSubPass < m_subpasses.size() ? &m_subpasses[indexSubPass] : NULL;
}

uint32_t CGfxRenderPass::GetAttachmentCount(void) const
{
	return m_attachments.size();
}

const AttachmentInformation* CGfxRenderPass::GetAttachment(int indexAttachment) const
{
	return indexAttachment < m_attachments.size() ? &m_attachments[indexAttachment] : NULL;
}
