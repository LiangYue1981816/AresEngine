#include "VKRenderer.h"


CVKRenderPass::CVKRenderPass(CVKDevice *pDevice, CVKRenderPassManager *pManager, int numAttachments, int numSubpasses)
	: CGfxRenderPass(numAttachments, numSubpasses)
	, m_pDevice(pDevice)
	, m_pManager(pManager)

	, m_vkRenderPass(VK_NULL_HANDLE)
{
	m_attachments.resize(numAttachments);
	m_subpasses.resize(numSubpasses);
}

CVKRenderPass::~CVKRenderPass(void)
{
	Destroy();
}

void CVKRenderPass::Release(void)
{
	m_pManager->Destroy(this);
}

HANDLE CVKRenderPass::GetRenderPass(void) const
{
	return m_vkRenderPass;
}

bool CVKRenderPass::Create(void)
{
	Destroy();

	eastl::vector<VkAttachmentDescription> attachments;
	eastl::vector<VkSubpassDescription> subpasses;
	eastl::vector<VkSubpassDependency> dependencies;

	eastl::unordered_map<int, VkAttachmentReference> depthStencilAttachmentReference;
	eastl::unordered_map<int, eastl::vector<VkAttachmentReference>> inputAttachmentReferences;
	eastl::unordered_map<int, eastl::vector<VkAttachmentReference>> outputAttachmentReferences;
	eastl::unordered_map<int, eastl::vector<VkAttachmentReference>> resolveAttachmentReferences;
	eastl::unordered_map<int, eastl::vector<uint32_t>> preserveAttachmentReferences;

	for (int indexAttachment = 0; indexAttachment < m_attachments.size(); indexAttachment++) {
		VkAttachmentDescription attachment = {};
		attachment.flags = 0;
		attachment.format = (VkFormat)m_attachments[indexAttachment].pixelFormat;
		attachment.samples = (VkSampleCountFlagBits)m_attachments[indexAttachment].samples;
		attachment.loadOp = m_attachments[indexAttachment].bClear ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD;
		attachment.storeOp = m_attachments[indexAttachment].bInvalidation ? VK_ATTACHMENT_STORE_OP_DONT_CARE : VK_ATTACHMENT_STORE_OP_STORE;
		attachment.stencilLoadOp = m_attachments[indexAttachment].bClear ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD;
		attachment.stencilStoreOp = m_attachments[indexAttachment].bInvalidation ? VK_ATTACHMENT_STORE_OP_DONT_CARE : VK_ATTACHMENT_STORE_OP_STORE;
		attachment.initialLayout = CVKHelper::IsFormatDepthOrStencil((VkFormat)m_attachments[indexAttachment].pixelFormat) ? VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL : VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		attachment.finalLayout = CVKHelper::IsFormatDepthOrStencil((VkFormat)m_attachments[indexAttachment].pixelFormat) ? VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL : VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		attachments.push_back(attachment);
	}

	for (int indexSubpass = 0; indexSubpass < m_subpasses.size(); indexSubpass++) {
		for (const auto &itInputAttachment : m_subpasses[indexSubpass].inputAttachments) {
			VkAttachmentReference attachment = {};
			attachment.attachment = itInputAttachment.first;
			attachment.layout = CVKHelper::IsFormatDepthOrStencil((VkFormat)m_attachments[itInputAttachment.first].pixelFormat) ? VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL : VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			inputAttachmentReferences[indexSubpass].emplace_back(attachment);
		}

		for (const auto &itOutputAttachment : m_subpasses[indexSubpass].outputAttachments) {
			VkAttachmentReference attachment = {};
			attachment.attachment = itOutputAttachment.first;
			attachment.layout = CVKHelper::IsFormatDepthOrStencil((VkFormat)m_attachments[itOutputAttachment.first].pixelFormat) ? VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL : VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			outputAttachmentReferences[indexSubpass].emplace_back(attachment);
		}

		for (const auto &itResolveAttachment : m_subpasses[indexSubpass].resolveAttachments) {
			VkAttachmentReference attachment = {};
			attachment.attachment = itResolveAttachment.first;
			attachment.layout = CVKHelper::IsFormatDepthOrStencil((VkFormat)m_attachments[itResolveAttachment.first].pixelFormat) ? VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL : VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			resolveAttachmentReferences[indexSubpass].emplace_back(attachment);
		}

		for (const auto &itPreserveAttachment : m_subpasses[indexSubpass].preserveAttachments) {
			preserveAttachmentReferences[indexSubpass].emplace_back(itPreserveAttachment.first);
		}

		if (m_subpasses[indexSubpass].depthStencilAttachment >= 0) {
			depthStencilAttachmentReference[indexSubpass].attachment = m_subpasses[indexSubpass].depthStencilAttachment;
			depthStencilAttachmentReference[indexSubpass].layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		}

		VkSubpassDescription subpass = {};
		subpass.flags = 0;
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.inputAttachmentCount = inputAttachmentReferences[indexSubpass].size();
		subpass.pInputAttachments = inputAttachmentReferences[indexSubpass].data();
		subpass.colorAttachmentCount = outputAttachmentReferences[indexSubpass].size();
		subpass.pColorAttachments = outputAttachmentReferences[indexSubpass].data();
		subpass.preserveAttachmentCount = preserveAttachmentReferences[indexSubpass].size();
		subpass.pPreserveAttachments = preserveAttachmentReferences[indexSubpass].data();
		subpass.pResolveAttachments = resolveAttachmentReferences[indexSubpass].data();
		subpass.pDepthStencilAttachment = m_subpasses[indexSubpass].depthStencilAttachment >= 0 ? &depthStencilAttachmentReference[indexSubpass] : nullptr;
		subpasses.emplace_back(subpass);

		if (indexSubpass > 0) {
			VkSubpassDependency dependency = {};
			dependency.srcSubpass = indexSubpass - 1;
			dependency.dstSubpass = indexSubpass;
			dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
			dependency.dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
			dependency.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			dependency.dstAccessMask = VK_ACCESS_INPUT_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
			dependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
			dependencies.emplace_back(dependency);
		}
	}

	VkRenderPassCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.flags = 0;
	createInfo.attachmentCount = attachments.size();
	createInfo.pAttachments = attachments.data();
	createInfo.subpassCount = subpasses.size();
	createInfo.pSubpasses = subpasses.data();
	createInfo.dependencyCount = dependencies.size();
	createInfo.pDependencies = dependencies.data();
	CALL_VK_FUNCTION_RETURN_BOOL(vkCreateRenderPass(m_pDevice->GetDevice(), &createInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkRenderPass));

	return true;
}

void CVKRenderPass::Destroy(void)
{
	if (m_vkRenderPass) {
		vkDestroyRenderPass(m_pDevice->GetDevice(), m_vkRenderPass, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
	}

	m_vkRenderPass = VK_NULL_HANDLE;
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
