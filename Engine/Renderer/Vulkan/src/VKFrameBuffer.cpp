#include "VKRenderer.h"


CVKFrameBuffer::CVKFrameBuffer(CVKDevice *pDevice, CVKFrameBufferManager *pManager, int width, int height, int numAttachments)
	: CGfxFrameBuffer(width, height, numAttachments)
	, m_pDevice(pDevice)
	, m_pManager(pManager)

	, m_width(width)
	, m_height(height)

	, m_vkFrameBuffer(VK_NULL_HANDLE)
{
	m_ptrAttachmentTextures.resize(numAttachments);
}

CVKFrameBuffer::~CVKFrameBuffer(void)
{
	Destroy();
}

void CVKFrameBuffer::Release(void)
{
	m_pManager->Destroy(this);
}

HANDLE CVKFrameBuffer::GetFrameBuffer(void)
{
	return (HANDLE)m_vkFrameBuffer;
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
	Destroy();
	{
		do {
			eastl::vector<VkImageView> attachments;
			{
				for (int indexAttachment = 0; indexAttachment < m_ptrAttachmentTextures.size(); indexAttachment++) {
					attachments.emplace_back((VkImageView)m_ptrAttachmentTextures[indexAttachment]->GetTexture());
				}
			}
			if (attachments.empty()) break;

			VkFramebufferCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			createInfo.pNext = nullptr;
			createInfo.flags = 0;
			createInfo.renderPass = (VkRenderPass)ptrRenderPass->GetRenderPass();
			createInfo.attachmentCount = attachments.size();
			createInfo.pAttachments = attachments.data();
			createInfo.width = m_width;
			createInfo.height = m_height;
			createInfo.layers = 1;
			CALL_VK_FUNCTION_BREAK(vkCreateFramebuffer(m_pDevice->GetDevice(), &createInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkFrameBuffer));

			return true;
		} while (false);
	}
	Destroy();
	return false;
}

void CVKFrameBuffer::Destroy(void)
{
	if (m_vkFrameBuffer) {
		vkDestroyFramebuffer(m_pDevice->GetDevice(), m_vkFrameBuffer, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
	}

	m_vkFrameBuffer = VK_NULL_HANDLE;
}

bool CVKFrameBuffer::SetAttachmentTexture(int indexAttachment, CGfxRenderTexturePtr ptrAttachmentTexture)
{
	if (ptrAttachmentTexture->GetWidth() == m_width && ptrAttachmentTexture->GetHeight() == m_height &&
		indexAttachment >= 0 && indexAttachment < (int)m_ptrAttachmentTextures.size()) {
		m_ptrAttachmentTextures[indexAttachment] = ptrAttachmentTexture;
		return true;
	}
	else {
		return false;
	}
}

CGfxRenderTexturePtr CVKFrameBuffer::GetAttachmentTexture(int indexAttachment) const
{
	if (indexAttachment >= 0 && indexAttachment < (int)m_ptrAttachmentTextures.size()) {
		return m_ptrAttachmentTextures[indexAttachment];
	}
	else {
		return nullptr;
	}
}
