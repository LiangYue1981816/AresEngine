#include "VKRenderer.h"


CVKTexture2D::CVKTexture2D(CVKDevice* pDevice, CVKTexture2DManager* pManager, uint32_t name)
	: CGfxTexture2D(name)
	, m_pDevice(pDevice)
	, m_pManager(pManager)
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

VkImageView CVKTexture2D::GetImageView(void) const
{
	if (m_ptrTexture) {
		return m_ptrTexture->GetImageView();
	}
	else {
		return VK_NULL_HANDLE;
	}
}

VkImageLayout CVKTexture2D::GetImageLayout(void) const
{
	if (m_ptrTexture) {
		return m_ptrTexture->GetImageLayout();
	}
	else {
		return VK_IMAGE_LAYOUT_UNDEFINED;
	}
}

VkImageAspectFlags CVKTexture2D::GetImageAspectFlags(void) const
{
	if (m_ptrTexture) {
		return m_ptrTexture->GetImageAspectFlags();
	}
	else {
		return 0;
	}
}

GfxTextureType CVKTexture2D::GetType(void) const
{
	if (m_ptrTexture) {
		return m_ptrTexture->GetType();
	}
	else {
		return GFX_TEXTURE_2D;
	}
}

GfxPixelFormat CVKTexture2D::GetFormat(void) const
{
	if (m_ptrTexture) {
		return m_ptrTexture->GetFormat();
	}
	else {
		return GFX_PIXELFORMAT_UNDEFINED;
	}
}

int CVKTexture2D::GetWidth(void) const
{
	if (m_ptrTexture) {
		return m_ptrTexture->GetWidth();
	}
	else {
		return 0;
	}
}

int CVKTexture2D::GetHeight(void) const
{
	if (m_ptrTexture) {
		return m_ptrTexture->GetHeight();
	}
	else {
		return 0;
	}
}

int CVKTexture2D::GetLevels(void) const
{
	if (m_ptrTexture) {
		return m_ptrTexture->GetLevels();
	}
	else {
		return 0;
	}
}

int CVKTexture2D::GetSamples(void) const
{
	if (m_ptrTexture) {
		return m_ptrTexture->GetSamples();
	}
	else {
		return 0;
	}
}

bool CVKTexture2D::Create(GfxPixelFormat format, int width, int height, int levels, int samples)
{
	if (m_ptrTexture && CVKHelper::IsFormatSupported((VkFormat)format) && CVKHelper::IsFormatDepthOrStencil((VkFormat)format) == false) {
		samples = std::max(samples, 1);
		return m_ptrTexture->Create(samples == 1 ? GFX_TEXTURE_2D : GFX_TEXTURE_2D_MULTISAMPLE, format, width, height, 1, levels, samples, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_IMAGE_TILING_OPTIMAL);
	}
	else {
		return false;
	}
}

void CVKTexture2D::Destroy(void)
{
	if (m_ptrTexture) {
		m_ptrTexture->Destroy();
	}
}

bool CVKTexture2D::Texture2DData(GfxPixelFormat format, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	if (m_ptrTexture) {
		return m_ptrTexture->Texture2DData(format, level, xoffset, yoffset, width, height, size, data);
	}
	else {
		return false;
	}
}

bool CVKTexture2D::Texture2DDataCompressed(GfxPixelFormat format, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	if (m_ptrTexture) {
		return m_ptrTexture->Texture2DData(format, level, xoffset, yoffset, width, height, size, data);
	}
	else {
		return false;
	}
}
