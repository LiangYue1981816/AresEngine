#include "VKRenderer.h"


CVKTexture2DArray::CVKTexture2DArray(CVKDevice* pDevice, uint32_t name)
	: CGfxTexture2DArray(name)
	, m_pDevice(pDevice)
{

}

CVKTexture2DArray::~CVKTexture2DArray(void)
{
	Destroy();
}

void CVKTexture2DArray::Release(void)
{

}

VkImageView CVKTexture2DArray::GetImageView(void) const
{
	return m_ptrTexture->GetImageView();
}

VkImageLayout CVKTexture2DArray::GetImageLayout(void) const
{
	return m_ptrTexture->GetImageLayout();
}

VkImageAspectFlags CVKTexture2DArray::GetImageAspectFlags(void) const
{
	return m_ptrTexture->GetImageAspectFlags();
}

GfxTextureType CVKTexture2DArray::GetType(void) const
{
	return m_ptrTexture->GetType();
}

GfxPixelFormat CVKTexture2DArray::GetFormat(void) const
{
	return m_ptrTexture->GetFormat();
}

int CVKTexture2DArray::GetWidth(void) const
{
	return m_ptrTexture->GetWidth();
}

int CVKTexture2DArray::GetHeight(void) const
{
	return m_ptrTexture->GetHeight();
}

int CVKTexture2DArray::GetLayers(void) const
{
	return m_ptrTexture->GetLayers();
}

int CVKTexture2DArray::GetLevels(void) const
{
	return m_ptrTexture->GetLevels();
}

bool CVKTexture2DArray::Create(GfxPixelFormat format, int width, int height, int layers, int levels)
{
	return m_ptrTexture->Create(GFX_TEXTURE_2D_ARRAY, format, width, height, layers, levels, 1, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_IMAGE_TILING_OPTIMAL);
}

void CVKTexture2DArray::Destroy(void)
{
	return m_ptrTexture->Destroy();
}

bool CVKTexture2DArray::Texture2DArrayData(GfxPixelFormat format, int layer, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	return m_ptrTexture->Texture2DArrayData(format, layer, level, xoffset, yoffset, width, height, size, data);
}

bool CVKTexture2DArray::Texture2DArrayDataCompressed(GfxPixelFormat format, int layer, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	return m_ptrTexture->Texture2DArrayData(format, layer, level, xoffset, yoffset, width, height, size, data);
}
