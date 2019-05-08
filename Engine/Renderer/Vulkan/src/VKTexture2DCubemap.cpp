#include "VKRenderer.h"


CVKTextureCubemap::CVKTextureCubemap(CVKDevice* pDevice, CVKTextureCubemapManager* pManager, uint32_t name)
	: CGfxTextureCubemap(name)
	, m_pDevice(pDevice)
	, m_pManager(pManager)
{

}

CVKTextureCubemap::~CVKTextureCubemap(void)
{
	Destroy();
}

void CVKTextureCubemap::Release(void)
{
	m_pManager->Destroy(this);
}

VkImageView CVKTextureCubemap::GetImageView(void) const
{
	if (m_ptrTexture) {
		return m_ptrTexture->GetImageView();
	}
	else {
		return VK_NULL_HANDLE;
	}
}

VkImageLayout CVKTextureCubemap::GetImageLayout(void) const
{
	if (m_ptrTexture) {
		return m_ptrTexture->GetImageLayout();
	}
	else {
		return VK_IMAGE_LAYOUT_UNDEFINED;
	}
}

VkImageAspectFlags CVKTextureCubemap::GetImageAspectFlags(void) const
{
	if (m_ptrTexture) {
		return m_ptrTexture->GetImageAspectFlags();
	}
	else {
		return 0;
	}
}

GfxTextureType CVKTextureCubemap::GetType(void) const
{
	if (m_ptrTexture) {
		return m_ptrTexture->GetType();
	}
	else {
		return GFX_TEXTURE_CUBE_MAP;
	}
}

GfxPixelFormat CVKTextureCubemap::GetFormat(void) const
{
	if (m_ptrTexture) {
		return m_ptrTexture->GetFormat();
	}
	else {
		return GFX_PIXELFORMAT_UNDEFINED;
	}
}

int CVKTextureCubemap::GetWidth(void) const
{
	if (m_ptrTexture) {
		return m_ptrTexture->GetWidth();
	}
	else {
		return 0;
	}
}

int CVKTextureCubemap::GetHeight(void) const
{
	if (m_ptrTexture) {
		return m_ptrTexture->GetHeight();
	}
	else {
		return 0;
	}
}

int CVKTextureCubemap::GetLevels(void) const
{
	if (m_ptrTexture) {
		return m_ptrTexture->GetLevels();
	}
	else {
		return 0;
	}
}

bool CVKTextureCubemap::Create(GfxPixelFormat format, int width, int height, int levels)
{
	if (m_ptrTexture && CVKHelper::IsFormatSupported((VkFormat)format) && CVKHelper::IsFormatDepthOrStencil((VkFormat)format) == false) {
		return m_ptrTexture->Create(GFX_TEXTURE_CUBE_MAP, format, width, height, 1, levels, 1, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_IMAGE_TILING_OPTIMAL);
	}
	else {
		return false;
	}
}

void CVKTextureCubemap::Destroy(void)
{
	if (m_ptrTexture) {
		m_ptrTexture->Destroy();
	}
}

bool CVKTextureCubemap::TextureCubemapData(GfxPixelFormat format, GfxCubemapFace face, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	if (m_ptrTexture) {
		return m_ptrTexture->TextureCubemapData(format, face, level, xoffset, yoffset, width, height, size, data);
	}
	else {
		return false;
	}
}

bool CVKTextureCubemap::TextureCubemapDataCompressed(GfxPixelFormat format, GfxCubemapFace face, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	if (m_ptrTexture) {
		return m_ptrTexture->TextureCubemapData(format, face, level, xoffset, yoffset, width, height, size, data);
	}
	else {
		return false;
	}
}
