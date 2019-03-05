#include "VKRenderer.h"


CVKTexture2D::CVKTexture2D(CVKDevice *pDevice, CVKTexture2DManager *pManager, uint32_t name)
	: CVKTexture(pDevice)
	, CGfxTexture2D(name)
	, m_name(name)
	, m_pManager(pManager)

	, m_format(GFX_PIXELFORMAT_UNDEFINED)
	, m_type(GFX_TEXTURE_INVALID_ENUM)

	, m_width(0)
	, m_height(0)
	, m_levels(0)
	, m_samples(0)
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

uint32_t CVKTexture2D::GetName(void) const
{
	return m_name;
}

HANDLE CVKTexture2D::GetTexture(void) const
{
	return (HANDLE)m_vkImageView;
}

GfxPixelFormat CVKTexture2D::GetFormat(void) const
{
	return m_format;
}

GfxTextureType CVKTexture2D::GetType(void) const
{
	return m_type;
}

int CVKTexture2D::GetWidth(void) const
{
	return m_width;
}

int CVKTexture2D::GetHeight(void) const
{
	return m_height;
}

int CVKTexture2D::GetLevels(void) const
{
	return m_levels;
}

int CVKTexture2D::GetSamples(void) const
{
	return m_samples;
}

bool CVKTexture2D::Create(HANDLE hExternTexture)
{
	Destroy();
	{
		do {
			CALL_BOOL_FUNCTION_BREAK(CVKTexture::CreateView((VkImageView)hExternTexture));
			m_type = GFX_TEXTURE_2D;

			return true;
		} while (false);
	}
	Destroy();
	return false;
}

bool CVKTexture2D::Create(GfxPixelFormat pixelFormat, int width, int height, int levels, int samples)
{
	Destroy();
	{
		do {
			CALL_BOOL_FUNCTION_BREAK(CVKHelper::IsFormatSupported((VkFormat)pixelFormat));
			CALL_BOOL_FUNCTION_BREAK(CreateImage(VK_IMAGE_TYPE_2D, VK_IMAGE_VIEW_TYPE_2D, (VkFormat)pixelFormat, width, height, levels, 1, CVKHelper::TranslateSampleCount(samples), VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT));
			CALL_BOOL_FUNCTION_BREAK(CreateView(VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_ASPECT_COLOR_BIT, (VkFormat)pixelFormat, levels, 1));

			m_format = pixelFormat;
			m_type = GFX_TEXTURE_2D;

			m_width = width;
			m_height = height;
			m_levels = levels;
			m_samples = std::max(samples, 1);

			return true;
		} while (false);
	}
	Destroy();
	return false;
}

void CVKTexture2D::Destroy(void)
{
	CVKTexture::Destroy();

	for (const auto &itLevelSize : m_size) {
		CGfxProfiler::DecTextureDataSize(itLevelSize.second);
	}

	m_format = GFX_PIXELFORMAT_UNDEFINED;
	m_type = GFX_TEXTURE_INVALID_ENUM;

	m_width = 0;
	m_height = 0;
	m_levels = 0;
	m_samples = 0;

	m_size.clear();
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

	CGfxProfiler::DecTextureDataSize(m_size[level]);
	{
		m_size[level] = size;

		m_transferLevels = m_levels;
		m_transferBuffers[level].region.bufferOffset = 0;
		m_transferBuffers[level].region.bufferRowLength = 0;
		m_transferBuffers[level].region.bufferImageHeight = 0;
		m_transferBuffers[level].region.imageSubresource = { VK_IMAGE_ASPECT_COLOR_BIT, (uint32_t)level, 0, 1 };
		m_transferBuffers[level].region.imageOffset = { xoffset, yoffset, 0 };
		m_transferBuffers[level].region.imageExtent = { (uint32_t)width, (uint32_t)height, 1 };
		m_transferBuffers[level].buffer.assign((uint8_t *)data, (uint8_t *)data + size);
	}
	CGfxProfiler::IncTextureDataSize(m_size[level]);

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

	CGfxProfiler::DecTextureDataSize(m_size[level]);
	{
		m_size[level] = size;

		m_transferLevels = m_levels;
		m_transferBuffers[level].region.bufferOffset = 0;
		m_transferBuffers[level].region.bufferRowLength = 0;
		m_transferBuffers[level].region.bufferImageHeight = 0;
		m_transferBuffers[level].region.imageSubresource = { VK_IMAGE_ASPECT_COLOR_BIT, (uint32_t)level, 0, 1 };
		m_transferBuffers[level].region.imageOffset = { xoffset, yoffset, 0 };
		m_transferBuffers[level].region.imageExtent = { (uint32_t)width, (uint32_t)height, 1 };
		m_transferBuffers[level].buffer.assign((uint8_t *)data, (uint8_t *)data + size);
	}
	CGfxProfiler::IncTextureDataSize(m_size[level]);

	return true;
}
