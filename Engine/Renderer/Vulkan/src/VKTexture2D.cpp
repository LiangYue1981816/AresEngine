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
	m_samples = samples > 1 ? samples : 1;

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
	if (m_format != pixelFormat) {
		return false;
	}

	if (m_bExtern == true) {
		return false;
	}

	if (m_levels < level) {
		return false;
	}

	if (m_samples != 1) {
		return false;
	}

	m_transferLevels = m_levels;
	m_transferBuffers[level].region.bufferOffset = 0;
	m_transferBuffers[level].region.bufferRowLength = 0;
	m_transferBuffers[level].region.bufferImageHeight = 0;
	m_transferBuffers[level].region.imageSubresource = { VK_IMAGE_ASPECT_COLOR_BIT, (uint32_t)level, 0, 1 };
	m_transferBuffers[level].region.imageOffset = { xoffset, yoffset, 0 };
	m_transferBuffers[level].region.imageExtent = { (uint32_t)width, (uint32_t)height, 1 };
	m_transferBuffers[level].buffer.assign((uint8_t *)data, (uint8_t *)data + size);

	return true;
}

bool CVKTexture2D::TransferTexture2DCompressed(GfxPixelFormat pixelFormat, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void *data)
{
	if (m_format != pixelFormat) {
		return false;
	}

	if (m_bExtern == true) {
		return false;
	}

	if (m_levels < level) {
		return false;
	}

	if (m_samples != 1) {
		return false;
	}

	m_transferLevels = m_levels;
	m_transferBuffers[level].region.bufferOffset = 0;
	m_transferBuffers[level].region.bufferRowLength = 0;
	m_transferBuffers[level].region.bufferImageHeight = 0;
	m_transferBuffers[level].region.imageSubresource = { VK_IMAGE_ASPECT_COLOR_BIT, (uint32_t)level, 0, 1 };
	m_transferBuffers[level].region.imageOffset = { xoffset, yoffset, 0 };
	m_transferBuffers[level].region.imageExtent = { (uint32_t)width, (uint32_t)height, 1 };
	m_transferBuffers[level].buffer.assign((uint8_t *)data, (uint8_t *)data + size);

	return true;
}
