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

bool CGLES3RenderPass::SetColorAttachment(int indexAttachment, GfxPixelFormat format, int samples, bool bInvalidation, bool bClear, float red, float green, float blue, float alpha)
{
	if (CGfxHelper::IsFormatColor(format) == false) {
		return false;
	}

	if (indexAttachment < 0 || indexAttachment >= m_attachments.size()) {
		return false;
	}

	m_attachments[indexAttachment].Set(format, samples, bInvalidation, bClear, red, green, blue, alpha);
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

	m_attachments[indexAttachment].Set(format, samples, bInvalidation, bClear, depth, stencil);
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

	m_subpasses[indexSubpass].SetInputAttachment(indexAttachment);
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

	m_subpasses[indexSubpass].SetOutputAttachment(indexAttachment);
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

	m_subpasses[indexSubpass].SetDepthStencilAttachment(indexAttachment);
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

	m_subpasses[indexSubpass].SetResolveAttachment(indexAttachment);
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
