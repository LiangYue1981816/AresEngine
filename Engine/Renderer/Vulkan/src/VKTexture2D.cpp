#include "VKRenderer.h"


CVKTexture2D::CVKTexture2D(CVKDevice* pDevice, CVKTexture2DManager* pManager, uint32_t name, GfxPixelFormat format, int width, int height, int levels, int samples)
	: CGfxTexture2D(name)
	, m_pManager(pManager)
	, m_pDevice(pDevice)
	, m_pTexture(nullptr)
{
	ASSERT(CVKHelper::IsFormatSupported((VkFormat)format));
	ASSERT(CGfxHelper::IsFormatColor(format));

	m_pTexture = new CVKTexture(
		pDevice, 
		samples == 1 ? GFX_TEXTURE_2D : GFX_TEXTURE_2D_MULTISAMPLE, 
		format, 
		width, 
		height, 
		1, 
		levels, 
		samples, 
		VK_IMAGE_ASPECT_COLOR_BIT, 
		VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_STORAGE_BIT,
		VK_IMAGE_TILING_OPTIMAL);
}

CVKTexture2D::~CVKTexture2D(void)
{
	delete m_pTexture;
}

void CVKTexture2D::Release(void)
{
	if (m_pManager) {
		m_pManager->Destroy(this);
	}
	else {
		delete this;
	}
}

VkImage CVKTexture2D::GetImage(void) const
{
	return m_pTexture->GetImage();
}

VkImageView CVKTexture2D::GetImageView(void) const
{
	return m_pTexture->GetImageView();
}

VkImageAspectFlags CVKTexture2D::GetImageAspectFlags(void) const
{
	return m_pTexture->GetImageAspectFlags();
}

GfxPixelFormat CVKTexture2D::GetFormat(void) const
{
	return m_pTexture->GetFormat();
}

int CVKTexture2D::GetWidth(void) const
{
	return m_pTexture->GetWidth();
}

int CVKTexture2D::GetHeight(void) const
{
	return m_pTexture->GetHeight();
}

int CVKTexture2D::GetLevels(void) const
{
	return m_pTexture->GetLevels();
}

int CVKTexture2D::GetSamples(void) const
{
	return m_pTexture->GetSamples();
}
