#include "VKRenderer.h"


CVKTexture2DArray::CVKTexture2DArray(CVKDevice* pDevice, CVKTexture2DArrayManager* pManager, uint32_t name)
	: CGfxTexture2DArray(name)
	, m_pDevice(pDevice)
	, m_pManager(pManager)
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

VkImageView CVKTexture2DArray::GetImageView(void) const
{
	if (m_ptrTexture) {
		return m_ptrTexture->GetImageView();
	}
	else {
		return VK_NULL_HANDLE;
	}
}

VkImageLayout CVKTexture2DArray::GetImageLayout(void) const
{
	if (m_ptrTexture) {
		return m_ptrTexture->GetImageLayout();
	}
	else {
		return VK_IMAGE_LAYOUT_UNDEFINED;
	}
}

VkImageAspectFlags CVKTexture2DArray::GetImageAspectFlags(void) const
{
	if (m_ptrTexture) {
		return m_ptrTexture->GetImageAspectFlags();
	}
	else {
		return 0;
	}
}

GfxTextureType CVKTexture2DArray::GetType(void) const
{
	if (m_ptrTexture) {
		return m_ptrTexture->GetType();
	}
	else {
		return GFX_TEXTURE_2D_ARRAY;
	}
}

GfxPixelFormat CVKTexture2DArray::GetFormat(void) const
{
	if (m_ptrTexture) {
		return m_ptrTexture->GetFormat();
	}
	else {
		return GFX_PIXELFORMAT_UNDEFINED;
	}
}

int CVKTexture2DArray::GetWidth(void) const
{
	if (m_ptrTexture) {
		return m_ptrTexture->GetWidth();
	}
	else {
		return 0;
	}
}

int CVKTexture2DArray::GetHeight(void) const
{
	if (m_ptrTexture) {
		return m_ptrTexture->GetHeight();
	}
	else {
		return 0;
	}
}

int CVKTexture2DArray::GetLayers(void) const
{
	if (m_ptrTexture) {
		return m_ptrTexture->GetLayers();
	}
	else {
		return 0;
	}
}

int CVKTexture2DArray::GetLevels(void) const
{
	if (m_ptrTexture) {
		return m_ptrTexture->GetLevels();
	}
	else {
		return 0;
	}
}

bool CVKTexture2DArray::Create(GfxPixelFormat format, int width, int height, int layers, int levels)
{
	if (m_ptrTexture && CVKHelper::IsFormatSupported((VkFormat)format) && CVKHelper::IsFormatDepthOrStencil((VkFormat)format) == false) {
		return m_ptrTexture->Create(GFX_TEXTURE_2D_ARRAY, format, width, height, layers, levels, 1, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_IMAGE_TILING_OPTIMAL);
	}
	else {
		return false;
	}
}

void CVKTexture2DArray::Destroy(void)
{
	if (m_ptrTexture) {
		m_ptrTexture->Destroy();
	}
}

bool CVKTexture2DArray::Texture2DArrayData(GfxPixelFormat format, int layer, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	if (m_ptrTexture) {
		return m_ptrTexture->Texture2DArrayData(format, layer, level, xoffset, yoffset, width, height, size, data);
	}
	else {
		return false;
	}
}

bool CVKTexture2DArray::Texture2DArrayDataCompressed(GfxPixelFormat format, int layer, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{
	if (m_ptrTexture) {
		return m_ptrTexture->Texture2DArrayData(format, layer, level, xoffset, yoffset, width, height, size, data);
	}
	else {
		return false;
	}
}
