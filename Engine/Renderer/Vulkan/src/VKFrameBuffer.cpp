#include "VKRenderer.h"


CVKFrameBuffer::CVKFrameBuffer(CVKDevice *pDevice, CVKFrameBufferManager *pManager, int width, int height)
	: CGfxFrameBuffer(width, height)
	, m_pManager(pManager)

	, m_width(width)
	, m_height(height)

	, m_vkFrameBuffer(VK_NULL_HANDLE)
{

}

CVKFrameBuffer::~CVKFrameBuffer(void)
{

}

void CVKFrameBuffer::Release(void)
{
	m_pManager->Destroy(this);
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
	if (ptrAttachmentTexture->GetWidth() == m_width && ptrAttachmentTexture->GetHeight() == m_height) {
		m_ptrAttachmentTextures[indexAttachment] = ptrAttachmentTexture;
		return true;
	}
	else {
		return false;
	}
}

CGfxRenderTexturePtr CVKFrameBuffer::GetAttachmentTexture(int indexAttachment) const
{
	const auto &itAttachmentTexture = m_ptrAttachmentTextures.find(indexAttachment);
	return itAttachmentTexture != m_ptrAttachmentTextures.end() ? itAttachmentTexture->second : nullptr;
}
