#include "VKRenderer.h"


CVKTexture2D::CVKTexture2D(CVKDevice* pDevice, CVKTexture2DManager* pManager, uint32_t name)
	: CGfxTexture2D(name)
	, m_pDevice(pDevice)
	, m_pManager(pManager)
{

}

CVKTexture2D::~CVKTexture2D(void)
{
	Destroy();
}

void CVKTexture2D::Release(void)
{
	m_pManager->Destroy(this);
}

VkImageView CVKTexture2D::GetImageView(void) const
{
	return m_ptrTexture->GetImageView();
}

VkImageLayout CVKTexture2D::GetImageLayout(void) const
{
	return m_ptrTexture->GetImageLayout();
}

VkImageAspectFlags CVKTexture2D::GetImageAspectFlags(void) const
{
	return m_ptrTexture->GetImageAspectFlags();
}

GfxTextureType CVKTexture2D::GetType(void) const
{
	return m_ptrTexture->GetType();
}

GfxPixelFormat CVKTexture2D::GetFormat(void) const
{
	return m_ptrTexture->GetFormat();
}

int CVKTexture2D::GetWidth(void) const
{
	return m_ptrTexture->GetWidth();
}

int CVKTexture2D::GetHeight(void) const
{
	return m_ptrTexture->GetHeight();
}

int CVKTexture2D::GetLevels(void) const
{
	return m_ptrTexture->GetLevels();
}

int CVKTexture2D::GetSamples(void) const
{
	return m_ptrTexture->GetSamples();
}

bool CVKTexture2D::Create(GfxPixelFormat format, int width, int height, int levels, int samples)
{
	samples = std::max(samples, 1);
	return m_ptrTexture->Create(samples == 1 ? GFX_TEXTURE_2D : GFX_TEXTURE_2D_MULTISAMPLE, format, width, height, 1, levels, samples, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_IMAGE_TILING_OPTIMAL);
}

void CVKTexture2D::Destroy(void)
{
	m_ptrTexture->Destroy();
}

bool CVKTexture2D::Texture2DData(GfxPixelFormat format, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	return m_ptrTexture->Texture2DData(format, level, xoffset, yoffset, width, height, size, data);
}

bool CVKTexture2D::Texture2DDataCompressed(GfxPixelFormat format, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	return m_ptrTexture->Texture2DData(format, level, xoffset, yoffset, width, height, size, data);
}
