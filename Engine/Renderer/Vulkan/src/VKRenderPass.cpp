#include "VKRenderer.h"


CVKRenderPass::CVKRenderPass(CVKDevice *pDevice, CVKRenderPassManager *pManager, int numAttachments, int numSubpasses)
	: CGfxRenderPass(numAttachments, numSubpasses)
	, m_pDevice(pDevice)
	, m_pManager(pManager)

	, m_vkRenderPass(VK_NULL_HANDLE)
{
	m_attachments.resize(numAttachments);
	m_subpasses.resize(numAttachments);
}

CVKRenderPass::~CVKRenderPass(void)
{
	Destroy();
}

void CVKRenderPass::Release(void)
{
	m_pManager->Destroy(this);
}

bool CVKRenderPass::Create(void)
{
	/*
	eastl::vector<VkAttachmentDescription> attachments;
	eastl::vector<VkSubpassDescription> subpasses;
	eastl::vector<VkSubpassDependency> dependencies;

	for (int index = 0; index < m_attachments.size(); index++) {
		VkAttachmentDescription attachment;
		attachment.flags = 0;
		attachment.format;
		attachment.samples;
		attachment.loadOp;
		attachment.storeOp;
		attachment.stencilLoadOp;
		attachment.stencilStoreOp;
		attachment.initialLayout;
		attachment.finalLayout;
	}

	VkRenderPassCreateInfo createInfo = {};
	createInfo.sType;
	createInfo.pNext;
	createInfo.flags;
	createInfo.attachmentCount;
	createInfo.pAttachments;
	createInfo.subpassCount;
	createInfo.pSubpasses;
	createInfo.dependencyCount;
	createInfo.pDependencies;
	return true;
	*/
	return true;
}

void CVKRenderPass::Destroy(void)
{

}

bool CVKRenderPass::SetColorAttachment(int indexAttachment, GfxPixelFormat pixelFormat, int samples, bool bInvalidation, bool bClear, float red, float green, float blue, float alpha)
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

bool CVKRenderPass::SetDepthStencilAttachment(int indexAttachment, GfxPixelFormat pixelFormat, int samples, bool bInvalidation, bool bClear, float depth, int stencil)
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

bool CVKRenderPass::SetSubpassInputColorReference(int indexSubPass, int indexAttachment, const char *szName)
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

bool CVKRenderPass::SetSubpassOutputColorReference(int indexSubPass, int indexAttachment)
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

bool CVKRenderPass::SetSubpassOutputDepthStencilReference(int indexSubPass, int indexAttachment)
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

bool CVKRenderPass::SetSubpassResolveReference(int indexSubPass, int indexAttachment)
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

bool CVKRenderPass::SetSubpassPreserveReference(int indexSubPass, int indexAttachment)
{
	if (indexSubPass >= (int)m_subpasses.size()) {
		return false;
	}

	if (indexAttachment >= (int)m_attachments.size()) {
		return false;
	}

	m_subpasses[indexSubPass].preserveAttachments[indexAttachment] = indexAttachment;
	return true;
}

uint32_t CVKRenderPass::GetAttachmentCount(void) const
{
	return m_attachments.size();
}

const AttachmentInformation* CVKRenderPass::GetAttachments(void) const
{
	return m_attachments.data();
}

const AttachmentInformation* CVKRenderPass::GetAttachment(int indexAttachment) const
{
	return indexAttachment >= 0 && indexAttachment < (int)m_attachments.size() ? &m_attachments[indexAttachment] : nullptr;
}

uint32_t CVKRenderPass::GetSubPassCount(void) const
{
	return m_subpasses.size();
}

uint32_t CVKRenderPass::GetSubpassInputAttachmentCount(int indexSubPass) const
{
	return indexSubPass >= 0 && indexSubPass < (int)m_subpasses.size() ? m_subpasses[indexSubPass].inputAttachments.size() : 0;
}

uint32_t CVKRenderPass::GetSubpassOutputAttachmentCount(int indexSubPass) const
{
	return indexSubPass >= 0 && indexSubPass < (int)m_subpasses.size() ? m_subpasses[indexSubPass].outputAttachments.size() : 0;
}

const SubPassInformation* CVKRenderPass::GetSubPass(int indexSubPass) const
{
	return indexSubPass >= 0 && indexSubPass < (int)m_subpasses.size() ? &m_subpasses[indexSubPass] : nullptr;
}
