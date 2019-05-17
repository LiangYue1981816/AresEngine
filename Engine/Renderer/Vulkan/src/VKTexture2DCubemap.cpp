#include "VKRenderer.h"


CVKTextureCubemap::CVKTextureCubemap(CVKDevice* pDevice, CVKTextureCubemapManager* pManager, uint32_t name)
	: CGfxTextureCubemap(name)
	, m_pDevice(pDevice)
	, m_pManager(pManager)
	, m_pTexture(nullptr)
{
	ASSERT(m_pDevice);
	ASSERT(m_pManager);

	m_pTexture = new CVKTexture(m_pDevice);
}

CVKTextureCubemap::~CVKTextureCubemap(void)
{
	delete m_pTexture;
}

void CVKTextureCubemap::Release(void)
{
	m_pManager->Destroy(this);
}

VkImageView CVKTextureCubemap::GetImageView(void) const
{
	return m_pTexture->GetImageView();
}

VkImageLayout CVKTextureCubemap::GetImageLayout(void) const
{
	return m_pTexture->GetImageLayout();
}

VkImageAspectFlags CVKTextureCubemap::GetImageAspectFlags(void) const
{
	return m_pTexture->GetImageAspectFlags();
}

GfxTextureType CVKTextureCubemap::GetType(void) const
{
	return m_pTexture->GetType();
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

bool CVKTextureCubemap::Create(GfxPixelFormat format, int width, int height, int levels)
{
	if (CVKHelper::IsFormatSupported((VkFormat)format) && CGfxHelper::IsFormatDepthOrStencil(format) == false) {
		return m_pTexture->Create(GFX_TEXTURE_CUBE_MAP, format, width, height, 1, levels, 1, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_IMAGE_TILING_OPTIMAL);
	}
	else {
		return false;
	}
}

void CVKTextureCubemap::Destroy(void)
{
	m_pTexture->Destroy();
}

bool CVKTextureCubemap::TextureCubemapData(GfxPixelFormat format, GfxCubemapFace face, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	return m_pTexture->TextureCubemapData(format, face, level, xoffset, yoffset, width, height, size, data);
}

bool CVKTextureCubemap::TextureCubemapDataCompressed(GfxPixelFormat format, GfxCubemapFace face, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	return m_pTexture->TextureCubemapData(format, face, level, xoffset, yoffset, width, height, size, data);
}
