#include "VKRenderer.h"


CVKTexture::CVKTexture(CVKDevice *pDevice, CVKTextureManager *pManager)
	: m_pDevice(pDevice)
	, m_pManager(pManager)

	, m_vkImageView(VK_NULL_HANDLE)
{

}

CVKTexture::~CVKTexture(void)
{

}

CVKImagePtr CVKTexture::GetImage(void) const
{
	return m_ptrImage;
}

CVKImagePtr CVKTexture::GetImageTransfer(void) const
{
	return m_ptrImageTransfer;
}
