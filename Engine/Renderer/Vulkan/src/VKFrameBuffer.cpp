#include "VKRenderer.h"


CVKFrameBuffer::CVKFrameBuffer(CVKDevice* pDevice, CVKFrameBufferManager* pManager, int width, int height, int numAttachments)
	: CGfxFrameBuffer(width, height, numAttachments)
	, m_pManager(pManager)
	, m_pDevice(pDevice)

	, m_vkFrameBuffer(VK_NULL_HANDLE)

	, m_width(width)
	, m_height(height)
	, m_ptrAttachmentTextures(numAttachments)
{

}

CVKFrameBuffer::~CVKFrameBuffer(void)
{
	Destroy(true);
}

void CVKFrameBuffer::Release(void)
{
	if (m_pManager) {
		m_pManager->Destroy(this);
	}
	else {
		delete this;
	}
}

VkFramebuffer CVKFrameBuffer::GetFrameBuffer(void)
{
	return m_vkFrameBuffer;
}

int CVKFrameBuffer::GetWidth(void) const
{
	return m_width;
}

int CVKFrameBuffer::GetHeight(void) const
{
	return m_height;
}

bool CVKFrameBuffer::Create(const CGfxRenderPassPtr ptrRenderPass)
{
	if (ptrRenderPass == nullptr) {
		return false;
	}

	eastl::vector<VkImageView> attachments;
	{
		for (int indexAttachment = 0; indexAttachment < m_ptrAttachmentTextures.size(); indexAttachment++) {
			attachments.emplace_back(((CVKRenderTexture*)m_ptrAttachmentTextures[indexAttachment].GetPointer())->GetImageView());
		}

		if (attachments.empty()) {
			return false;
		}
	}

	VkFramebufferCreateInfo frameBufferCreateInfo = {};
	frameBufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	frameBufferCreateInfo.pNext = nullptr;
	frameBufferCreateInfo.flags = 0;
	frameBufferCreateInfo.renderPass = ((CVKRenderPass*)ptrRenderPass.GetPointer())->GetRenderPass();
	frameBufferCreateInfo.attachmentCount = attachments.size();
	frameBufferCreateInfo.pAttachments = attachments.data();
	frameBufferCreateInfo.width = m_width;
	frameBufferCreateInfo.height = m_height;
	frameBufferCreateInfo.layers = 1;
	CALL_VK_FUNCTION_RETURN_BOOL(vkCreateFramebuffer(m_pDevice->GetDevice(), &frameBufferCreateInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkFrameBuffer));

	return true;
}

void CVKFrameBuffer::Destroy(bool bClear /*= true*/)
{
	if (bClear) {
		m_ptrAttachmentTextures.clear();
	}

	if (m_vkFrameBuffer) {
		vkDestroyFramebuffer(m_pDevice->GetDevice(), m_vkFrameBuffer, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
	}

	m_vkFrameBuffer = VK_NULL_HANDLE;
}

bool CVKFrameBuffer::SetAttachmentTexture(int indexAttachment, const CGfxRenderTexturePtr ptrAttachmentTexture)
{
	if (ptrAttachmentTexture == nullptr) {
		return false;
	}

	if (ptrAttachmentTexture->GetWidth() != m_width || ptrAttachmentTexture->GetHeight() != m_height) {
		return false;
	}

	if (indexAttachment < 0 || indexAttachment >= m_ptrAttachmentTextures.size()) {
		return false;
	}

	m_ptrAttachmentTextures[indexAttachment] = ptrAttachmentTexture;
	return true;
}

const CGfxRenderTexturePtr CVKFrameBuffer::GetAttachmentTexture(int indexAttachment) const
{
	if (indexAttachment >= 0 && indexAttachment < m_ptrAttachmentTextures.size()) {
		return m_ptrAttachmentTextures[indexAttachment];
	}
	else {
		return nullptr;
	}
}
