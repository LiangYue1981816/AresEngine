#include "VKRenderer.h"


CVKTextureCubemap::CVKTextureCubemap(CVKDevice* pDevice, CVKTextureCubemapManager* pManager, uint32_t name, GfxPixelFormat format, int width, int height, int levels)
	: CGfxTextureCubemap(name)
	, m_pManager(pManager)
	, m_pDevice(pDevice)
	, m_pTexture(nullptr)
{
	m_pTexture = new CVKTexture(
		pDevice,
		GFX_TEXTURE_CUBE_MAP,
		format,
		width,
		height,
		1,
		levels,
		1,
		VK_IMAGE_ASPECT_COLOR_BIT,
		VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
		VK_IMAGE_TILING_OPTIMAL);
}

CVKTextureCubemap::~CVKTextureCubemap(void)
{
	delete m_pTexture;
}

void CVKTextureCubemap::Release(void)
{
	if (m_pManager) {
		m_pManager->Destroy(this);
	}
	else {
		delete this;
	}
}

VkImage CVKTextureCubemap::GetImage(void) const
{
	return m_pTexture->GetImage();
}

VkImageView CVKTextureCubemap::GetImageView(void) const
{
	return m_pTexture->GetImageView();
}

VkImageAspectFlags CVKTextureCubemap::GetImageAspectFlags(void) const
{
	return m_pTexture->GetImageAspectFlags();
}

GfxPixelFormat CVKTextureCubemap::GetFormat(void) const
{
	return m_pTexture->GetFormat();
}

int CVKTextureCubemap::GetWidth(void) const
{
	return m_pTexture->GetWidth();
}

int CVKTextureCubemap::GetHeight(void) const
{
	return m_pTexture->GetHeight();
}

int CVKTextureCubemap::GetLevels(void) const
{
	return m_pTexture->GetLevels();
}
