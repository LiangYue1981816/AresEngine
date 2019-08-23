#include "VKRenderer.h"


CVKTexture2D::CVKTexture2D(CVKDevice* pDevice, CVKTexture2DManager* pManager, uint32_t name)
	: CGfxTexture2D(name)
	, m_pDevice(pDevice)
	, m_pManager(pManager)
	, m_pTexture(nullptr)
{
	m_pTexture = new CVKTexture(m_pDevice);
}

CVKTexture2D::~CVKTexture2D(void)
{
	delete m_pTexture;
}

void CVKTexture2D::Release(void)
{
	m_pManager->Destroy(this);
}

VkImageView CVKTexture2D::GetImageView(void) const
{
	return m_pTexture->GetImageView();
}

VkImageLayout CVKTexture2D::GetImageLayout(void) const
{
	return m_pTexture->GetImageLayout();
}

VkImageAspectFlags CVKTexture2D::GetImageAspectFlags(void) const
{
	return m_pTexture->GetImageAspectFlags();
}

GfxTextureType CVKTexture2D::GetType(void) const
{
	return m_pTexture->GetType();
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

bool CVKTexture2D::Create(GfxPixelFormat format, int width, int height, int levels, int samples)
{
	if (CVKHelper::IsFormatSupported((VkFormat)format) && CGfxHelper::IsFormatColor(format)) {
		samples = std::max(samples, 1);
		return m_pTexture->Create(samples == 1 ? GFX_TEXTURE_2D : GFX_TEXTURE_2D_MULTISAMPLE, format, width, height, 1, levels, samples, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_IMAGE_TILING_OPTIMAL);
	}
	else {
		return false;
	}
}

void CVKTexture2D::Destroy(void)
{
	m_pTexture->Destroy();
}

bool CVKTexture2D::Texture2DData(GfxPixelFormat format, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	return m_pTexture->Texture2DData(format, level, xoffset, yoffset, width, height, size, data);
}

bool CVKTexture2D::Texture2DDataCompressed(GfxPixelFormat format, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	return m_pTexture->Texture2DData(format, level, xoffset, yoffset, width, height, size, data);
}

bool CVKTexture2D::PipelineBarrier(VkCommandBuffer vkCommandBuffer, VkImageLayout imageLayout)
{
	return m_pTexture->PipelineBarrier(vkCommandBuffer, imageLayout);
}

bool CVKTexture2D::PipelineBarrier(VkCommandBuffer vkCommandBuffer, VkImageLayout imageLayout, VkAccessFlags srcAccessFlags, VkAccessFlags dstAccessFlags, VkPipelineStageFlags srcPipelineStageFlags, VkPipelineStageFlags dstPipelineStageFlags)
{
	return m_pTexture->PipelineBarrier(vkCommandBuffer, imageLayout, srcAccessFlags, dstAccessFlags, srcPipelineStageFlags, dstPipelineStageFlags);
}
