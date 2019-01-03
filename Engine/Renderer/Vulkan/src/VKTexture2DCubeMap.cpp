#include "VKRenderer.h"


CVKTextureCubeMap::CVKTextureCubeMap(CVKDevice *pDevice, CVKTextureManager *pManager, uint32_t name)
	: CVKTexture(pDevice, pManager)
	, CGfxTextureCubeMap(name)
{

}

CVKTextureCubeMap::~CVKTextureCubeMap(void)
{
	Destroy();
}

void CVKTextureCubeMap::Release(void)
{
	m_pManager->Destroy(this);
}

bool CVKTextureCubeMap::Create(uint64_t texture)
{
	Destroy();

	m_type = GFX_TEXTURE_CUBE_MAP;
	return CVKTexture::CreateView((VkImageView)texture);
}

bool CVKTextureCubeMap::Create(GfxPixelFormat pixelFormat, int width, int height, int levels)
{
	Destroy();

	CALL_BOOL_FUNCTION_RETURN_BOOL(CVKHelper::IsFormatSupported((VkFormat)pixelFormat));
	CALL_BOOL_FUNCTION_RETURN_BOOL(CreateImage(VK_IMAGE_TYPE_2D, VK_IMAGE_VIEW_TYPE_CUBE, (VkFormat)pixelFormat, width, height, levels, 1, VK_SAMPLE_COUNT_1_BIT, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT));
	CALL_BOOL_FUNCTION_RETURN_BOOL(CreateView(VK_IMAGE_VIEW_TYPE_CUBE, VK_IMAGE_ASPECT_COLOR_BIT, (VkFormat)pixelFormat, levels, 1));

	m_format = pixelFormat;
	m_type = GFX_TEXTURE_CUBE_MAP;

	m_width = width;
	m_height = height;
	m_levels = levels;

	return true;
}

void CVKTextureCubeMap::Destroy(void)
{
	CVKTexture::Destroy();

	for (const auto &itFaceSize : m_size) {
		for (const auto &itLevelSize : itFaceSize.second) {
			CGfxProfiler::DecTextureDataSize(itLevelSize.second);
		}
	}

	m_size.clear();

	m_format = GFX_PIXELFORMAT_UNDEFINED;
	m_type = GFX_TEXTURE_INVALID_ENUM;

	m_width = 0;
	m_height = 0;
	m_levels = 0;
}

bool CVKTextureCubeMap::TransferTexture2D(GfxPixelFormat pixelFormat, GfxTextureCubeMapFace face, int level, int xoffset, int yoffset, int width, int height, GfxDataType type, uint32_t size, const void *data)
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

	CGfxProfiler::DecTextureDataSize(m_size[face][level]);
	{
		m_size[face][level] = size;

		m_transferLevels = m_levels;
		m_transferBuffers[face * m_levels + level].region.bufferOffset = 0;
		m_transferBuffers[face * m_levels + level].region.bufferRowLength = 0;
		m_transferBuffers[face * m_levels + level].region.bufferImageHeight = 0;
		m_transferBuffers[face * m_levels + level].region.imageSubresource = { VK_IMAGE_ASPECT_COLOR_BIT, (uint32_t)level, 0, 1 };
		m_transferBuffers[face * m_levels + level].region.imageOffset = { xoffset, yoffset, 0 };
		m_transferBuffers[face * m_levels + level].region.imageExtent = { (uint32_t)width, (uint32_t)height, 1 };
		m_transferBuffers[face * m_levels + level].buffer.assign((uint8_t *)data, (uint8_t *)data + size);
	}
	CGfxProfiler::IncTextureDataSize(m_size[face][level]);

	return true;
}

bool CVKTextureCubeMap::TransferTexture2DCompressed(GfxPixelFormat pixelFormat, GfxTextureCubeMapFace face, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void *data)
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

	CGfxProfiler::DecTextureDataSize(m_size[face][level]);
	{
		m_size[face][level] = size;

		m_transferLevels = m_levels;
		m_transferBuffers[face * m_levels + level].region.bufferOffset = 0;
		m_transferBuffers[face * m_levels + level].region.bufferRowLength = 0;
		m_transferBuffers[face * m_levels + level].region.bufferImageHeight = 0;
		m_transferBuffers[face * m_levels + level].region.imageSubresource = { VK_IMAGE_ASPECT_COLOR_BIT, (uint32_t)level, 0, 1 };
		m_transferBuffers[face * m_levels + level].region.imageOffset = { xoffset, yoffset, 0 };
		m_transferBuffers[face * m_levels + level].region.imageExtent = { (uint32_t)width, (uint32_t)height, 1 };
		m_transferBuffers[face * m_levels + level].buffer.assign((uint8_t *)data, (uint8_t *)data + size);
	}
	CGfxProfiler::IncTextureDataSize(m_size[face][level]);

	return true;
}
