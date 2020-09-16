#include "VKRenderer.h"


CVKTexture2DArray::CVKTexture2DArray(CVKDevice* pDevice, CVKTexture2DArrayManager* pManager, uint32_t name)
	: CGfxTexture2DArray(name)
	, m_pDevice(pDevice)
	, m_pManager(pManager)
	, m_pTexture(nullptr)
{
	m_pTexture = new CVKTexture(m_pDevice);
}

CVKTexture2DArray::~CVKTexture2DArray(void)
{
	delete m_pTexture;
}

void CVKTexture2DArray::Release(void)
{
	m_pManager->Destroy(this);
}

VkImage CVKTexture2DArray::GetImage(void) const
{
	return m_pTexture->GetImage();
}

VkImageView CVKTexture2DArray::GetImageView(void) const
{
	return m_pTexture->GetImageView();
}

VkImageAspectFlags CVKTexture2DArray::GetImageAspectFlags(void) const
{
	return m_pTexture->GetImageAspectFlags();
}

GfxTextureType CVKTexture2DArray::GetType(void) const
{
	return m_pTexture->GetType();
}

GfxPixelFormat CVKTexture2DArray::GetFormat(void) const
{
	return m_pTexture->GetFormat();
}

int CVKTexture2DArray::GetWidth(void) const
{
	return m_pTexture->GetWidth();
}

int CVKTexture2DArray::GetHeight(void) const
{
	return m_pTexture->GetHeight();
}

int CVKTexture2DArray::GetLayers(void) const
{
	return m_pTexture->GetLayers();
}

int CVKTexture2DArray::GetLevels(void) const
{
	return m_pTexture->GetLevels();
}

bool CVKTexture2DArray::Create(GfxPixelFormat format, int width, int height, int layers, int levels)
{
	if (CVKHelper::IsFormatSupported((VkFormat)format) && CGfxHelper::IsFormatColor(format)) {
		return m_pTexture->Create(GFX_TEXTURE_2D_ARRAY, format, width, height, layers, levels, 1, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_STORAGE_BIT, VK_IMAGE_TILING_OPTIMAL);
	}
	else {
		return false;
	}
}

void CVKTexture2DArray::Destroy(void)
{
	m_pTexture->Destroy();
}

bool CVKTexture2DArray::Texture2DArrayData(GfxPixelFormat format, int layer, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	return m_pTexture->Texture2DArrayData(format, layer, level, xoffset, yoffset, width, height, size, data);
}

bool CVKTexture2DArray::Texture2DArrayDataCompressed(GfxPixelFormat format, int layer, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	return m_pTexture->Texture2DArrayData(format, layer, level, xoffset, yoffset, width, height, size, data);
}
/*
bool CVKTexture2DArray::PipelineBarrier(VkCommandBuffer vkCommandBuffer, VkImageLayout imageLayout)
{
	return m_pTexture->PipelineBarrier(vkCommandBuffer, imageLayout);
}

bool CVKTexture2DArray::PipelineBarrier(VkCommandBuffer vkCommandBuffer, VkImageLayout imageLayout, VkAccessFlags srcAccessFlags, VkAccessFlags dstAccessFlags, VkPipelineStageFlags srcPipelineStageFlags, VkPipelineStageFlags dstPipelineStageFlags)
{
	return m_pTexture->PipelineBarrier(vkCommandBuffer, imageLayout, srcAccessFlags, dstAccessFlags, srcPipelineStageFlags, dstPipelineStageFlags);
}
*/