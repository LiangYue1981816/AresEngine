#include "VKRenderer.h"


CVKFrameBuffer::CVKFrameBuffer(CVKDevice *pDevice, CVKFrameBufferManager *pManager, int width, int height, int numAttachments)
	: CGfxFrameBuffer(width, height, numAttachments)
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

bool CVKFrameBuffer::SetAttachmentTexture(int indexAttachment, CGfxRenderTexturePtr &ptrAttachmentTexture)
{
	if (ptrAttachmentTexture->GetWidth() == m_width && ptrAttachmentTexture->GetHeight() == m_height &&
		indexAttachment >= 0 && indexAttachment < m_ptrAttachmentTextures.size()) {
		m_ptrAttachmentTextures[indexAttachment] = ptrAttachmentTexture;
		return true;
	}
	else {
		return false;
	}
}

CGfxRenderTexturePtr CVKFrameBuffer::GetAttachmentTexture(int indexAttachment) const
{
	if (indexAttachment >= 0 && indexAttachment < m_ptrAttachmentTextures.size()) {
		return m_ptrAttachmentTextures[indexAttachment];
	}
	else {
		return nullptr;
	}
}

bool CVKFrameBuffer::Create(HANDLE hRenderPass)
{
	VkFramebufferCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.flags;
	createInfo.renderPass;
	createInfo.attachmentCount;
	createInfo.pAttachments;
	createInfo.width;
	createInfo.height;
	createInfo.layers;
	return true;
}

void CVKFrameBuffer::Destroy(void)
{

}
