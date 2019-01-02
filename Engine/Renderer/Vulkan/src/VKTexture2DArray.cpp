#include "VKRenderer.h"


CVKTexture2DArray::CVKTexture2DArray(CVKDevice *pDevice, CVKTextureManager *pManager, uint32_t name)
	: CVKTexture(pDevice, pManager)
	, CGfxTexture2DArray(name)
{

}

CVKTexture2DArray::~CVKTexture2DArray(void)
{
	Destroy();
}

void CVKTexture2DArray::Release(void)
{

}

bool CVKTexture2DArray::Create(uint64_t texture)
{
	Destroy();

	m_type = GFX_TEXTURE_2D_ARRAY;
	return CVKTexture::CreateView((VkImageView)texture);
}

bool CVKTexture2DArray::Create(GfxPixelFormat pixelFormat, int width, int height, int levels, int layers)
{
	Destroy();

	m_format = pixelFormat;
	m_type = GFX_TEXTURE_2D_ARRAY;

	m_width = width;
	m_height = height;
	m_levels = levels;
	m_layers = layers;

	CALL_BOOL_FUNCTION_RETURN_BOOL(CreateImage(VK_IMAGE_TYPE_2D, VK_IMAGE_VIEW_TYPE_2D_ARRAY, (VkFormat)pixelFormat, width, height, levels, layers, VK_SAMPLE_COUNT_1_BIT, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT));
	CALL_BOOL_FUNCTION_RETURN_BOOL(CreateView(VK_IMAGE_VIEW_TYPE_2D_ARRAY, VK_IMAGE_ASPECT_COLOR_BIT, (VkFormat)pixelFormat, levels, layers));

	return true;
}

void CVKTexture2DArray::Destroy(void)
{
	CVKTexture::Destroy();

	for (const auto &itLayerSize : m_size) {
		for (const auto &itLevelSize : itLayerSize.second) {
			CGfxProfiler::DecTextureDataSize(itLevelSize.second);
		}
	}

	m_size.clear();

	m_format = GFX_PIXELFORMAT_UNDEFINED;
	m_type = GFX_TEXTURE_INVALID_ENUM;

	m_width = 0;
	m_height = 0;
	m_levels = 0;
	m_layers = 0;
}

bool CVKTexture2DArray::TransferTexture2D(GfxPixelFormat pixelFormat, int layer, int level, int xoffset, int yoffset, int width, int height, GfxDataType type, uint32_t size, const void *data)
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

	if (m_layers < layer) {
		return false;
	}

	CGfxProfiler::DecTextureDataSize(m_size[layer][level]);
	{
		m_size[layer][level] = size;

		m_transferLevels = m_levels;
		m_transferLayers = m_layers;
		m_transferBuffers[layer * m_levels + level].region.bufferOffset = 0;
		m_transferBuffers[layer * m_levels + level].region.bufferRowLength = 0;
		m_transferBuffers[layer * m_levels + level].region.bufferImageHeight = 0;
		m_transferBuffers[layer * m_levels + level].region.imageSubresource = { VK_IMAGE_ASPECT_COLOR_BIT, (uint32_t)level, (uint32_t)layer, 1 };
		m_transferBuffers[layer * m_levels + level].region.imageOffset = { xoffset, yoffset, 0 };
		m_transferBuffers[layer * m_levels + level].region.imageExtent = { (uint32_t)width, (uint32_t)height, 1 };
		m_transferBuffers[layer * m_levels + level].buffer.assign((uint8_t *)data, (uint8_t *)data + size);
	}
	CGfxProfiler::IncTextureDataSize(m_size[layer][level]);

	return true;
}

bool CVKTexture2DArray::TransferTexture2DCompressed(GfxPixelFormat pixelFormat, int layer, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void *data)
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

	if (m_layers < layer) {
		return false;
	}

	CGfxProfiler::DecTextureDataSize(m_size[layer][level]);
	{
		m_size[layer][level] = size;

		m_transferLevels = m_levels;
		m_transferLayers = m_layers;
		m_transferBuffers[layer * m_levels + level].region.bufferOffset = 0;
		m_transferBuffers[layer * m_levels + level].region.bufferRowLength = 0;
		m_transferBuffers[layer * m_levels + level].region.bufferImageHeight = 0;
		m_transferBuffers[layer * m_levels + level].region.imageSubresource = { VK_IMAGE_ASPECT_COLOR_BIT, (uint32_t)level, (uint32_t)layer, 1 };
		m_transferBuffers[layer * m_levels + level].region.imageOffset = { xoffset, yoffset, 0 };
		m_transferBuffers[layer * m_levels + level].region.imageExtent = { (uint32_t)width, (uint32_t)height, 1 };
		m_transferBuffers[layer * m_levels + level].buffer.assign((uint8_t *)data, (uint8_t *)data + size);
	}
	CGfxProfiler::IncTextureDataSize(m_size[layer][level]);

	return true;
}
