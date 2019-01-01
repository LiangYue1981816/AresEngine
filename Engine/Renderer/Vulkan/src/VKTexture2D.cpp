#include "VKRenderer.h"


CVKTexture2D::CVKTexture2D(CVKDevice *pDevice, CVKTextureManager *pManager, uint32_t name)
	: CVKTexture(pDevice, pManager)
	, CGfxTexture2D(name)
{

}

CVKTexture2D::~CVKTexture2D(void)
{
	Destroy();
}

void CVKTexture2D::Release(void)
{

}

uint32_t CVKTexture2D::GetTarget(void) const
{
	return VK_IMAGE_TYPE_2D;
}

bool CVKTexture2D::Create(uint64_t texture)
{
	return CVKTexture::Create((VkImageView)texture);
}

bool CVKTexture2D::Create(GfxPixelFormat pixelFormat, int width, int height, int levels, int samples)
{
	Destroy();

	m_format = pixelFormat;

	m_width = width;
	m_height = height;
	m_levels = levels;
	m_samples = samples;

	m_ptrImage = CVKImagePtr(new CVKImage(m_pDevice, VK_IMAGE_TYPE_2D, (VkFormat)pixelFormat, width, height, levels, 1, (VkSampleCountFlagBits)samples, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT));

	return true;
}

void CVKTexture2D::Destroy(void)
{
	CVKTexture::Destroy();

	for (const auto &itLevelSize : m_size) {
		CGfxProfiler::DecTextureDataSize(itLevelSize.second);
	}

	m_size.clear();

	m_width = 0;
	m_height = 0;
	m_levels = 0;
	m_samples = 0;
	m_format = GFX_PIXELFORMAT_UNDEFINED;
}

bool CVKTexture2D::TransferTexture2D(GfxPixelFormat pixelFormat, int level, int xoffset, int yoffset, int width, int height, GfxDataType type, uint32_t size, const void *data)
{
	return false;
}

bool CVKTexture2D::TransferTexture2DCompressed(GfxPixelFormat pixelFormat, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void *data)
{
	return false;
}
