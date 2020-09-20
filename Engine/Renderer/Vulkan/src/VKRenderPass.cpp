#include "VKRenderer.h"


CVKRenderPass::CVKRenderPass(CVKDevice* pDevice, CVKRenderPassManager* pManager, uint32_t name, int numAttachments, int numSubpasses)
	: CGfxRenderPass(name, numAttachments, numSubpasses)
	, m_pManager(pManager)
	, m_pDevice(pDevice)

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
	if (m_pManager) {
		m_pManager->Destroy(this);
	}
	else {
		delete this;
	}
}

VkRenderPass CVKRenderPass::GetRenderPass(void) const
{
	return m_vkRenderPass;
}

bool CVKRenderPass::Create(void)
{
	struct SubpassAttachment {
		eastl::vector<VkAttachmentReference> inputAttachments;
		eastl::vector<VkAttachmentReference> outputAttachments;
		eastl::vector<VkAttachmentReference> resolveAttachments;
		eastl::vector<uint32_t> preserveAttachments;
		VkAttachmentReference depthStencilAttachment;
	};

	eastl::unordered_map<int, SubpassAttachment> subpassAttachments;
	{
		for (int indexSubpass = 0; indexSubpass < m_subpasses.size(); indexSubpass++) {
			for (const auto& itInputAttachment : m_subpasses[indexSubpass].inputAttachments) {
				if (CGfxHelper::IsFormatColor(m_attachments[itInputAttachment.first].format)) {
					VkAttachmentReference attachment = {};
					attachment.attachment = itInputAttachment.first;
					attachment.layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
					subpassAttachments[indexSubpass].inputAttachments.emplace_back(attachment);
				}
				else {
					VkAttachmentReference attachment = {};
					attachment.attachment = itInputAttachment.first;
					attachment.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
					subpassAttachments[indexSubpass].inputAttachments.emplace_back(attachment);
				}
			}

			for (const auto& itOutputAttachment : m_subpasses[indexSubpass].outputAttachments) {
				// Note: Layout for color attachment can only be VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL or VK_IMAGE_LAYOUT_GENERAL.
				VkAttachmentReference attachment = {};
				attachment.attachment = itOutputAttachment.first;
				attachment.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL; // VK_IMAGE_LAYOUT_GENERAL
				subpassAttachments[indexSubpass].outputAttachments.emplace_back(attachment);
			}

			if (m_subpasses[indexSubpass].resolveAttachments.empty() == false) {
				// Note: resolveAttachments is an optional array of outputAttachments size
				if (m_subpasses[indexSubpass].resolveAttachments.size() != m_subpasses[indexSubpass].outputAttachments.size()) {
					return false;
				}

				for (const auto& itResolveAttachment : m_subpasses[indexSubpass].resolveAttachments) {
					if (CGfxHelper::IsFormatColor(m_attachments[itResolveAttachment.first].format)) {
						VkAttachmentReference attachment = {};
						attachment.attachment = itResolveAttachment.first;
						attachment.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
						subpassAttachments[indexSubpass].resolveAttachments.emplace_back(attachment);
					}
					else {
						VkAttachmentReference attachment = {};
						attachment.attachment = itResolveAttachment.first;
						attachment.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
						subpassAttachments[indexSubpass].resolveAttachments.emplace_back(attachment);
					}
				}
			}

			for (const auto& itPreserveAttachment : m_subpasses[indexSubpass].preserveAttachments) {
				subpassAttachments[indexSubpass].preserveAttachments.emplace_back(itPreserveAttachment.first);
			}

			if (m_subpasses[indexSubpass].depthStencilAttachment >= 0) {
				VkAttachmentReference attachment = {};
				attachment.attachment = m_subpasses[indexSubpass].depthStencilAttachment;
				attachment.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
				subpassAttachments[indexSubpass].depthStencilAttachment = attachment;
			}

			if (subpassAttachments[indexSubpass].outputAttachments.empty()) {
				return false;
			}
		}

		if (subpassAttachments.empty()) {
			return false;
		}
	}

	eastl::vector<VkAttachmentDescription> attachments;
	eastl::vector<VkSubpassDescription> subpasses;
	eastl::vector<VkSubpassDependency> dependencies;
	{
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
			VkSubpassDescription subpass = {};
			subpass.flags = 0;
			subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
			subpass.inputAttachmentCount = subpassAttachments[indexSubpass].inputAttachments.size();
			subpass.pInputAttachments = subpassAttachments[indexSubpass].inputAttachments.data();
			subpass.colorAttachmentCount = subpassAttachments[indexSubpass].outputAttachments.size();
			subpass.pColorAttachments = subpassAttachments[indexSubpass].outputAttachments.data();
			subpass.pResolveAttachments = subpassAttachments[indexSubpass].resolveAttachments.data();
			subpass.preserveAttachmentCount = subpassAttachments[indexSubpass].preserveAttachments.size();
			subpass.pPreserveAttachments = subpassAttachments[indexSubpass].preserveAttachments.data();
			subpass.pDepthStencilAttachment = m_subpasses[indexSubpass].depthStencilAttachment >= 0 ? &subpassAttachments[indexSubpass].depthStencilAttachment : nullptr;
			subpasses.emplace_back(subpass);
		}

		for (int indexSubpass = 1; indexSubpass < m_subpasses.size(); indexSubpass++) {
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

		if (attachments.empty()) {
			return false;
		}

		if (subpasses.empty()) {
			return false;
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

void CVKRenderPass::Destroy(bool bClear /*= true*/)
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

bool CVKRenderPass::SetColorAttachment(int indexAttachment, GfxPixelFormat format, int samples, bool bInvalidation, bool bClear, float clearRed /*= 0.0f*/, float clearGreen /*= 0.0f*/, float clearBlue /*= 0.0f*/, float clearAlpha /*= 0.0f*/)
{
	if (CGfxHelper::IsFormatColor(format) == false) {
		return false;
	}

	if (indexAttachment < 0 || indexAttachment >= m_attachments.size()) {
		return false;
	}

	m_attachments[indexAttachment].Set(format, samples, bInvalidation, bClear, clearRed, clearGreen, clearBlue, clearAlpha);
	return true;
}

bool CVKRenderPass::SetDepthStencilAttachment(int indexAttachment, GfxPixelFormat format, int samples, bool bInvalidation, bool bClear, float clearDepth /*= 1.0f*/, int clearStencil /*= 0*/)
{
	if (CGfxHelper::IsFormatDepthOrStencil(format) == false) {
		return false;
	}

	if (indexAttachment < 0 || indexAttachment >= m_attachments.size()) {
		return false;
	}

	m_attachments[indexAttachment].Set(format, samples, bInvalidation, bClear, clearDepth, clearStencil);
	return true;
}

bool CVKRenderPass::SetSubpassInputReference(int indexSubpass, int indexAttachment)
{
	if (indexSubpass < 0 || indexSubpass >= m_subpasses.size()) {
		return false;
	}

	if (indexAttachment < 0 || indexAttachment >= m_attachments.size()) {
		return false;
	}

	m_subpasses[indexSubpass].SetInputAttachment(indexAttachment);
	return true;
}

bool CVKRenderPass::SetSubpassOutputReference(int indexSubpass, int indexAttachment)
{
	if (indexSubpass < 0 || indexSubpass >= m_subpasses.size()) {
		return false;
	}

	if (indexAttachment < 0 || indexAttachment >= m_attachments.size()) {
		return false;
	}

	m_subpasses[indexSubpass].SetOutputAttachment(indexAttachment);
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
	if (indexSubpass < 0 || indexSubpass >= m_subpasses.size()) {
		return false;
	}

	if (indexAttachment < 0 || indexAttachment >= m_attachments.size()) {
		return false;
	}

	m_subpasses[indexSubpass].SetPreserveAttachment(indexAttachment);
	return true;
}

bool CVKRenderPass::SetSubpassDepthStencilReference(int indexSubpass, int indexAttachment)
{
	if (CGfxHelper::IsFormatDepthOrStencil(m_attachments[indexAttachment].format) == false) {
		return false;
	}

	if (indexSubpass < 0 || indexSubpass >= m_subpasses.size()) {
		return false;
	}

	if (indexAttachment < 0 || indexAttachment >= m_attachments.size()) {
		return false;
	}

	m_subpasses[indexSubpass].SetDepthStencilAttachment(indexAttachment);
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
