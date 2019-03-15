#include "VKRenderer.h"


CVKTexture2DArray::CVKTexture2DArray(CVKDevice *pDevice, CVKTexture2DArrayManager *pManager, uint32_t name)
	: CVKTexture(pDevice)
	, CGfxTexture2DArray(name)
	, m_name(name)
	, m_pManager(pManager)

	, m_format(GFX_PIXELFORMAT_UNDEFINED)
	, m_type(GFX_TEXTURE_INVALID_ENUM)

	, m_width(0)
	, m_height(0)
	, m_levels(0)
	, m_layers(0)
{

}

CVKTexture2DArray::~CVKTexture2DArray(void)
{
	Destroy();
}

void CVKTexture2DArray::Release(void)
{
	m_pManager->Destroy(this);
}

uint32_t CVKTexture2DArray::GetName(void) const
{
	return m_name;
}

HANDLE CVKTexture2DArray::GetTexture(void) const
{
	return (HANDLE)m_vkImageView;
}

GfxPixelFormat CVKTexture2DArray::GetFormat(void) const
{
	return m_format;
}

GfxTextureType CVKTexture2DArray::GetType(void) const
{
	return m_type;
}

int CVKTexture2DArray::GetWidth(void) const
{
	return m_width;
}

int CVKTexture2DArray::GetHeight(void) const
{
	return m_height;
}

int CVKTexture2DArray::GetLevels(void) const
{
	return m_levels;
}

int CVKTexture2DArray::GetLayers(void) const
{
	return m_layers;
}

bool CVKTexture2DArray::Create(HANDLE hExternTexture)
{
	Destroy();
	{
		do {
			CALL_BOOL_FUNCTION_BREAK(CVKTexture::CreateView((VkImageView)hExternTexture));
			m_type = GFX_TEXTURE_2D_ARRAY;

			return true;
		} while (false);
	}
	Destroy();
	return false;
}

bool CVKTexture2DArray::Create(GfxPixelFormat format, int width, int height, int levels, int layers)
{
	Destroy();
	{
		do {
			CALL_BOOL_FUNCTION_BREAK(CVKHelper::IsFormatSupported((VkFormat)format));
			CALL_BOOL_FUNCTION_BREAK(CreateImage(VK_IMAGE_TYPE_2D, VK_IMAGE_VIEW_TYPE_2D_ARRAY, (VkFormat)format, width, height, levels, layers, VK_SAMPLE_COUNT_1_BIT, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT));
			CALL_BOOL_FUNCTION_BREAK(CreateView(VK_IMAGE_VIEW_TYPE_2D_ARRAY, VK_IMAGE_ASPECT_COLOR_BIT, (VkFormat)format, levels, layers));

			m_format = format;
			m_type = GFX_TEXTURE_2D_ARRAY;

			m_width = width;
			m_height = height;
			m_levels = levels;
			m_layers = layers;

			return true;
		} while (false);
	}
	Destroy();
	return false;
}

void CVKTexture2DArray::Destroy(void)
{
	CVKTexture::Destroy();

	for (const auto &itLayerSize : m_size) {
		for (const auto &itLevelSize : itLayerSize.second) {
			CGfxProfiler::DecTextureDataSize(itLevelSize.second);
		}
	}

	m_format = GFX_PIXELFORMAT_UNDEFINED;
	m_type = GFX_TEXTURE_INVALID_ENUM;

	m_width = 0;
	m_height = 0;
	m_levels = 0;
	m_layers = 0;

	m_size.clear();
}

bool CVKTexture2DArray::TransferTexture2D(GfxPixelFormat format, int layer, int level, int xoffset, int yoffset, int width, int height, GfxDataType type, uint32_t size, const void *data)
{
	if (m_format != format) {
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

bool CVKTexture2DArray::TransferTexture2DCompressed(GfxPixelFormat format, int layer, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void *data)
{
	if (m_format != format) {
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
