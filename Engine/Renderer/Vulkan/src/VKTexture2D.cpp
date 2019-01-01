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

bool CVKTexture2D::Create(uint32_t texture)
{
	return false;
}

bool CVKTexture2D::Create(GfxPixelFormat pixelFormat, int width, int height, int levels, int samples)
{
	m_ptrImage = CVKImagePtr(new CVKImage(m_pDevice, VK_IMAGE_TYPE_2D, (VkFormat)pixelFormat, width, height, levels, 1, (VkSampleCountFlagBits)samples, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT));
	return true;
}

void CVKTexture2D::Destroy(void)
{
	m_ptrImage.Release();
}

bool CVKTexture2D::TransferTexture2D(GfxPixelFormat pixelFormat, int level, int xoffset, int yoffset, int width, int height, GfxDataType type, uint32_t size, const void *data)
{
	return false;
}

bool CVKTexture2D::TransferTexture2DCompressed(GfxPixelFormat pixelFormat, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void *data)
{
	return false;
}
