#include "VKRenderer.h"


CVKTexture2DArray::CVKTexture2DArray(CVKDevice* pDevice, CVKTexture2DArrayManager* pManager, uint32_t name, GfxPixelFormat format, int width, int height, int layers, int levels)
	: CGfxTexture2DArray(name)
	, m_pManager(pManager)
	, m_pDevice(pDevice)
	, m_pTexture(nullptr)
{
	ASSERT(CVKHelper::IsFormatSupported((VkFormat)format));
	ASSERT(CGfxHelper::IsFormatColor(format));

	m_pTexture = new CVKTexture(
		pDevice,
		GFX_TEXTURE_2D_ARRAY,
		format,
		width,
		height,
		layers,
		levels,
		1,
		VK_IMAGE_ASPECT_COLOR_BIT,
		VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_STORAGE_BIT,
		VK_IMAGE_TILING_OPTIMAL);
}

CVKTexture2DArray::~CVKTexture2DArray(void)
{
	delete m_pTexture;
}

void CVKTexture2DArray::Release(void)
{
	if (m_pManager) {
		m_pManager->Destroy(this);
	}
	else {
		delete this;
	}
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
