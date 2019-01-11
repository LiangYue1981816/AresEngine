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

uint64_t CVKFrameBuffer::GetFrameBuffer(void)
{
	return (uint64_t)m_vkFrameBuffer;
}

int CVKFrameBuffer::GetWidth(void) const
{
	return m_width;
}

int CVKFrameBuffer::GetHeight(void) const
{
	return m_height;
}

bool CVKFrameBuffer::Create(void)
{
	return true;
}

void CVKFrameBuffer::Destroy(void)
{

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
