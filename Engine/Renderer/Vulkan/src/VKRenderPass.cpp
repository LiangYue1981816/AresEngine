#include "VKRenderer.h"


CVKRenderPass::CVKRenderPass(CVKDevice* pDevice, CVKRenderPassManager* pManager, uint32_t name, int numAttachments, int numSubpasses)
	: CGfxRenderPass(name, numAttachments, numSubpasses)
	, m_pDevice(pDevice)
	, m_pManager(pManager)

	, m_vkRenderPass(VK_NULL_HANDLE)

	, m_attachments(numAttachments)
	, m_subpasses(numSubpasses)
{

}

CVKRenderPass::~CVKRenderPass(void)
{
	Destroy(true);
}

void CVKRenderPass::Release(void)
{
	m_pManager->Destroy(this);
}

VkRenderPass CVKRenderPass::GetRenderPass(void) const
{
	ASSERT(m_vkRenderPass);
	return m_vkRenderPass;
}

bool CVKRenderPass::Create(void)
{
	Destroy(false);
	{
		do {
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
				attachment.format = (VkFormat)m_attachments[indexAttachment].format;
				attachment.samples = CVKHelper::TranslateSampleCount(m_attachments[indexAttachment].samples);
				attachment.loadOp = m_attachments[indexAttachment].bClear ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD;
				attachment.storeOp = m_attachments[indexAttachment].bInvalidation ? VK_ATTACHMENT_STORE_OP_DONT_CARE : VK_ATTACHMENT_STORE_OP_STORE;
				attachment.stencilLoadOp = m_attachments[indexAttachment].bClear ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD;
				attachment.stencilStoreOp = m_attachments[indexAttachment].bInvalidation ? VK_ATTACHMENT_STORE_OP_DONT_CARE : VK_ATTACHMENT_STORE_OP_STORE;
				attachment.initialLayout = VK_IMAGE_LAYOUT_GENERAL;
				attachment.finalLayout = VK_IMAGE_LAYOUT_GENERAL;
				attachments.emplace_back(attachment);
			}

			for (int indexSubpass = 0; indexSubpass < m_subpasses.size(); indexSubpass++) {
				for (const auto& itInputAttachment : m_subpasses[indexSubpass].inputAttachments) {
					if (CGfxHelper::IsFormatColor(m_attachments[itInputAttachment.first].format)) {
						VkAttachmentReference attachment = {};
						attachment.attachment = itInputAttachment.first;
						attachment.layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
						inputAttachmentReferences[indexSubpass].emplace_back(attachment);
					}
					else if (CGfxHelper::IsFormatDepthOrStencil(m_attachments[itInputAttachment.first].format)) {
						VkAttachmentReference attachment = {};
						attachment.attachment = itInputAttachment.first;
						attachment.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
						inputAttachmentReferences[indexSubpass].emplace_back(attachment);
					}
					else {
						VkAttachmentReference attachment = {};
						attachment.attachment = itInputAttachment.first;
						attachment.layout = VK_IMAGE_LAYOUT_GENERAL;
						inputAttachmentReferences[indexSubpass].emplace_back(attachment);
					}
				}

				for (const auto& itOutputAttachment : m_subpasses[indexSubpass].outputAttachments) {
					// Layout for color attachment can only be VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL or VK_IMAGE_LAYOUT_GENERAL.
					VkAttachmentReference attachment = {};
					attachment.attachment = itOutputAttachment.first;
					attachment.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL; // VK_IMAGE_LAYOUT_GENERAL
					outputAttachmentReferences[indexSubpass].emplace_back(attachment);
				}

				for (const auto& itResolveAttachment : m_subpasses[indexSubpass].resolveAttachments) {
					if (CGfxHelper::IsFormatColor(m_attachments[itResolveAttachment.first].format)) {
						VkAttachmentReference attachment = {};
						attachment.attachment = itResolveAttachment.first;
						attachment.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
						resolveAttachmentReferences[indexSubpass].emplace_back(attachment);
					}
					else if (CGfxHelper::IsFormatDepthOrStencil(m_attachments[itResolveAttachment.first].format)) {
						VkAttachmentReference attachment = {};
						attachment.attachment = itResolveAttachment.first;
						attachment.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
						resolveAttachmentReferences[indexSubpass].emplace_back(attachment);
					}
					else {
						VkAttachmentReference attachment = {};
						attachment.attachment = itResolveAttachment.first;
						attachment.layout = VK_IMAGE_LAYOUT_GENERAL;
						resolveAttachmentReferences[indexSubpass].emplace_back(attachment);
					}
				}

				for (const auto& itPreserveAttachment : m_subpasses[indexSubpass].preserveAttachments) {
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
			CALL_VK_FUNCTION_BREAK(vkCreateRenderPass(m_pDevice->GetDevice(), &createInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkRenderPass));

			return true;
		} while (false);
	}
	Destroy(true);
	return false;
}

void CVKRenderPass::Destroy(bool bClear)
{
	if (bClear) {
		m_attachments.clear();
		m_subpasses.clear();
	}

	if (m_vkRenderPass) {
		vkDestroyRenderPass(m_pDevice->GetDevice(), m_vkRenderPass, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
	}

	m_vkRenderPass = VK_NULL_HANDLE;
}

bool CVKRenderPass::SetColorAttachment(int indexAttachment, GfxPixelFormat format, int samples, bool bInvalidation, bool bClear, float red, float green, float blue, float alpha)
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

bool CVKRenderPass::SetDepthStencilAttachment(int indexAttachment, GfxPixelFormat format, int samples, bool bInvalidation, bool bClear, float depth, int stencil)
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

bool CVKRenderPass::SetSubpassInputColorReference(int indexSubpass, int indexAttachment)
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

	m_subpasses[indexSubpass].SetInputColorAttachment(indexAttachment);
	return true;
}

bool CVKRenderPass::SetSubpassInputDepthStencilReference(int indexSubpass, int indexAttachment)
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

	m_subpasses[indexSubpass].SetInputDepthStencilAttachment(indexAttachment);
	return true;
}

bool CVKRenderPass::SetSubpassOutputColorReference(int indexSubpass, int indexAttachment)
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

	m_subpasses[indexSubpass].SetOutputColorAttachment(indexAttachment);
	return true;
}

bool CVKRenderPass::SetSubpassOutputDepthStencilReference(int indexSubpass, int indexAttachment)
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

	m_subpasses[indexSubpass].SetOutputDepthStencilAttachment(indexAttachment);
	return true;
}

bool CVKRenderPass::SetSubpassDepthStencilReference(int indexSubpass, int indexAttachment)
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

bool CVKRenderPass::SetSubpassResolveReference(int indexSubpass, int indexAttachment)
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

bool CVKRenderPass::SetSubpassPreserveReference(int indexSubpass, int indexAttachment)
{
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
	if (indexAttachment >= 0 && indexAttachment < m_attachments.size()) {
		return &m_attachments[indexAttachment];
	}
	else {
		return nullptr;
	}
}

uint32_t CVKRenderPass::GetSubpassCount(void) const
{
	return m_subpasses.size();
}

uint32_t CVKRenderPass::GetSubpassInputAttachmentCount(int indexSubpass) const
{
	if (indexSubpass >= 0 && indexSubpass < m_subpasses.size()) {
		return m_subpasses[indexSubpass].inputAttachments.size();
	}
	else {
		return 0;
	}
}

uint32_t CVKRenderPass::GetSubpassOutputAttachmentCount(int indexSubpass) const
{
	if (indexSubpass >= 0 && indexSubpass < m_subpasses.size()) {
		return m_subpasses[indexSubpass].outputAttachments.size();
	}
	else {
		return 0;
	}
}

const SubpassInformation* CVKRenderPass::GetSubpass(int indexSubpass) const
{
	if (indexSubpass >= 0 && indexSubpass < m_subpasses.size()) {
		return &m_subpasses[indexSubpass];
	}
	else {
		return nullptr;
	}
}
