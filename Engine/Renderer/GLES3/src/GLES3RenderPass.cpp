#include "GLES3Renderer.h"


CGLES3RenderPass::CGLES3RenderPass(CGLES3RenderPassManager* pManager, uint32_t name, int numAttachments, int numSubpasses)
	: CGfxRenderPass(name, numAttachments, numSubpasses)
	, m_pManager(pManager)

	, m_attachments(numAttachments)
	, m_subpasses(numAttachments)
{

}

CGLES3RenderPass::~CGLES3RenderPass(void)
{
	Destroy(true);
}

void CGLES3RenderPass::Release(void)
{
	m_pManager->Destroy(this);
}

bool CGLES3RenderPass::Create(void)
{
	return true;
}

void CGLES3RenderPass::Destroy(bool bClear)
{
	if (bClear) {
		m_attachments.clear();
		m_subpasses.clear();
	}
}

bool CGLES3RenderPass::SetColorAttachment(int indexAttachment, GfxPixelFormat format, int samples, bool bPresent, bool bInvalidation, bool bClear, float red, float green, float blue, float alpha)
{
	if (CGfxHelper::IsFormatColor(format) == false) {
		return false;
	}

	if (indexAttachment < 0 || indexAttachment >= m_attachments.size()) {
		return false;
	}

	m_attachments[indexAttachment].format = format;
	m_attachments[indexAttachment].samples = samples;
	m_attachments[indexAttachment].bPresent = bPresent;
	m_attachments[indexAttachment].bInvalidation = bInvalidation;
	m_attachments[indexAttachment].bClear = bClear;
	m_attachments[indexAttachment].clearValue.color[0] = red;
	m_attachments[indexAttachment].clearValue.color[1] = green;
	m_attachments[indexAttachment].clearValue.color[2] = blue;
	m_attachments[indexAttachment].clearValue.color[3] = alpha;

	return true;
}

bool CGLES3RenderPass::SetDepthStencilAttachment(int indexAttachment, GfxPixelFormat format, int samples, bool bInvalidation, bool bClear, float depth, int stencil)
{
	if (CGfxHelper::IsFormatDepthOrStencil(format) == false) {
		return false;
	}

	if (indexAttachment < 0 || indexAttachment >= m_attachments.size()) {
		return false;
	}

	m_attachments[indexAttachment].format = format;
	m_attachments[indexAttachment].samples = samples;
	m_attachments[indexAttachment].bInvalidation = bInvalidation;
	m_attachments[indexAttachment].bClear = bClear;
	m_attachments[indexAttachment].clearValue.depth = depth;
	m_attachments[indexAttachment].clearValue.stencil = stencil;

	return true;
}

bool CGLES3RenderPass::SetSubpassInputColorReference(int indexSubpass, int indexAttachment)
{
	if (indexSubpass < 0 || indexSubpass >= m_subpasses.size()) {
		return false;
	}

	if (indexAttachment < 0 || indexAttachment >= m_attachments.size()) {
		return false;
	}

	if (CGfxHelper::IsFormatColor(m_attachments[indexAttachment].format) == false) {
		return false;
	}

	m_subpasses[indexSubpass].inputAttachments[indexAttachment] = indexAttachment;
	return true;
}

bool CGLES3RenderPass::SetSubpassOutputColorReference(int indexSubpass, int indexAttachment)
{
	if (indexSubpass < 0 || indexSubpass >= m_subpasses.size()) {
		return false;
	}

	if (indexAttachment < 0 || indexAttachment >= m_attachments.size()) {
		return false;
	}

	if (CGfxHelper::IsFormatColor(m_attachments[indexAttachment].format) == false) {
		return false;
	}

	m_subpasses[indexSubpass].outputAttachments[indexAttachment] = indexAttachment;
	return true;
}

bool CGLES3RenderPass::SetSubpassOutputDepthStencilReference(int indexSubpass, int indexAttachment)
{
	if (indexSubpass < 0 || indexSubpass >= m_subpasses.size()) {
		return false;
	}

	if (indexAttachment < 0 || indexAttachment >= m_attachments.size()) {
		return false;
	}

	if (CGfxHelper::IsFormatDepthOrStencil(m_attachments[indexAttachment].format) == false) {
		return false;
	}

	m_subpasses[indexSubpass].depthStencilAttachment = indexAttachment;
	return true;
}

bool CGLES3RenderPass::SetSubpassResolveReference(int indexSubpass, int indexAttachment)
{
	if (indexSubpass < 0 || indexSubpass >= m_subpasses.size()) {
		return false;
	}

	if (indexAttachment < 0 || indexAttachment >= m_attachments.size()) {
		return false;
	}

	m_subpasses[indexSubpass].resolveAttachments[indexAttachment] = indexAttachment;
	return true;
}

bool CGLES3RenderPass::SetSubpassPreserveReference(int indexSubpass, int indexAttachment)
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
	if (indexAttachment >= 0 && indexAttachment < m_attachments.size()) {
		return &m_attachments[indexAttachment];
	}
	else {
		return nullptr;
	}
}

uint32_t CGLES3RenderPass::GetSubpassCount(void) const
{
	return m_subpasses.size();
}

uint32_t CGLES3RenderPass::GetSubpassInputAttachmentCount(int indexSubpass) const
{
	if (indexSubpass >= 0 && indexSubpass < m_subpasses.size()) {
		return m_subpasses[indexSubpass].inputAttachments.size();
	}
	else {
		return 0;
	}
}

uint32_t CGLES3RenderPass::GetSubpassOutputAttachmentCount(int indexSubpass) const
{
	if (indexSubpass >= 0 && indexSubpass < m_subpasses.size()) {
		return m_subpasses[indexSubpass].outputAttachments.size();
	}
	else {
		return 0;
	}
}

const SubpassInformation* CGLES3RenderPass::GetSubpass(int indexSubpass) const
{
	if (indexSubpass >= 0 && indexSubpass < m_subpasses.size()) {
		return &m_subpasses[indexSubpass];
	}
	else {
		return nullptr;
	}
}
