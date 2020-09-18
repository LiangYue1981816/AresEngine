#include "VKRenderer.h"


CVKTexture2D::CVKTexture2D(CVKDevice* pDevice, CVKTexture2DManager* pManager, uint32_t name, GfxPixelFormat format, int width, int height, int levels, int samples)
	: CGfxTexture2D(name)
	, m_pManager(pManager)
	, m_pDevice(pDevice)
	, m_pTexture(nullptr)
{
	m_pTexture = new CVKTexture(
		pDevice, 
		samples == 1 ? GFX_TEXTURE_2D : GFX_TEXTURE_2D_MULTISAMPLE, 
		format, 
		width, 
		height, 
		1, 
		levels, 
		samples, 
		VK_IMAGE_ASPECT_COLOR_BIT, 
		VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT, 
		VK_IMAGE_TILING_OPTIMAL);
}

CVKTexture2D::~CVKTexture2D(void)
{
	delete m_pTexture;
}

void CVKTexture2D::Release(void)
{
	if (m_pManager) {
		m_pManager->Destroy(this);
	}
	else {
		delete this;
	}
}

VkImage CVKTexture2D::GetImage(void) const
{
	if (m_pTexture) {
		return m_pTexture->GetImage();
	}
	else {
		return VK_NULL_HANDLE;
	}
}

VkImageView CVKTexture2D::GetImageView(void) const
{
	if (m_pTexture) {
		return m_pTexture->GetImageView();
	}
	else {
		return VK_NULL_HANDLE;
	}
}

VkImageAspectFlags CVKTexture2D::GetImageAspectFlags(void) const
{
	if (m_pTexture) {
		return m_pTexture->GetImageAspectFlags();
	}
	else {
		return 0;
	}
}

GfxPixelFormat CVKTexture2D::GetFormat(void) const
{
	if (m_pTexture) {
		return m_pTexture->GetFormat();
	}
	else {
		return GFX_PIXELFORMAT_UNDEFINED;
	}
}

int CVKTexture2D::GetWidth(void) const
{
	if (m_pTexture) {
		return m_pTexture->GetWidth();
	}
	else {
		return 0;
	}
}

int CVKTexture2D::GetHeight(void) const
{
	if (m_pTexture) {
		return m_pTexture->GetHeight();
	}
	else {
		return 0;
	}
}

int CVKTexture2D::GetLevels(void) const
{
	if (m_pTexture) {
		return m_pTexture->GetLevels();
	}
	else {
		return 0;
	}
}

int CVKTexture2D::GetSamples(void) const
{
	if (m_pTexture) {
		return m_pTexture->GetSamples();
	}
	else {
		return 0;
	}
}
