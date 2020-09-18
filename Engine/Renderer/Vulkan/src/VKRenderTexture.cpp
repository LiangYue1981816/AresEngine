#include "VKRenderer.h"


CVKRenderTexture::CVKRenderTexture(CVKDevice* pDevice, CVKRenderTextureManager* pManager, uint32_t name, HANDLE hExternalTexture, GfxPixelFormat format, int width, int height, int samples /*= 1*/)
	: CGfxRenderTexture(name)
	, m_pManager(pManager)
	, m_pDevice(pDevice)
	, m_pTexture(nullptr)
{
	VkImageAspectFlags imageAspectFlags;

	if (CGfxHelper::IsFormatDepthOnly(format)) {
		imageAspectFlags = VK_IMAGE_ASPECT_DEPTH_BIT;
	}
	else if (CGfxHelper::IsFormatStencilOnly(format)) {
		imageAspectFlags = VK_IMAGE_ASPECT_STENCIL_BIT;
	}
	else if (CGfxHelper::IsFormatDepthAndStencil(format)) {
		imageAspectFlags = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
	}
	else {
		imageAspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
	}

	m_pTexture = new CVKTexture(
		pDevice,
		samples == 1 ? GFX_TEXTURE_2D : GFX_TEXTURE_2D_MULTISAMPLE,
		format,
		width,
		height,
		1,
		1,
		samples,
		imageAspectFlags,
		(VkImage)hExternalTexture);
}

CVKRenderTexture::CVKRenderTexture(CVKDevice* pDevice, CVKRenderTextureManager* pManager, uint32_t name, GfxPixelFormat format, int width, int height, int samples /*= 1*/, bool bTransient /*= false*/)
	: CGfxRenderTexture(name)
	, m_pManager(pManager)
	, m_pDevice(pDevice)
	, m_pTexture(nullptr)
{
	VkImageAspectFlags imageAspectFlags;
	VkImageUsageFlags imageUsageFlags;

	if (CGfxHelper::IsFormatDepthOnly(format)) {
		imageAspectFlags = VK_IMAGE_ASPECT_DEPTH_BIT;
		imageUsageFlags = (bTransient ? VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT : VK_IMAGE_USAGE_TRANSFER_SRC_BIT) | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT | VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
	}
	else if (CGfxHelper::IsFormatStencilOnly(format)) {
		imageAspectFlags = VK_IMAGE_ASPECT_STENCIL_BIT;
		imageUsageFlags = (bTransient ? VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT : VK_IMAGE_USAGE_TRANSFER_SRC_BIT) | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT | VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
	}
	else if (CGfxHelper::IsFormatDepthAndStencil(format)) {
		imageAspectFlags = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
		imageUsageFlags = (bTransient ? VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT : VK_IMAGE_USAGE_TRANSFER_SRC_BIT) | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT | VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
	}
	else {
		imageAspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
		imageUsageFlags = (bTransient ? VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT : VK_IMAGE_USAGE_TRANSFER_SRC_BIT) | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_STORAGE_BIT;
	}

	m_pTexture = new CVKTexture(
		pDevice,
		samples == 1 ? GFX_TEXTURE_2D : GFX_TEXTURE_2D_MULTISAMPLE,
		format,
		width,
		height,
		1,
		1,
		samples,
		imageAspectFlags,
		imageUsageFlags,
		VK_IMAGE_TILING_OPTIMAL);
}

CVKRenderTexture::~CVKRenderTexture(void)
{
	delete m_pTexture;
}

void CVKRenderTexture::Release(void)
{
	if (m_pManager) {
		m_pManager->Destroy(this);
	}
	else {
		delete this;
	}
}

VkImage CVKRenderTexture::GetImage(void) const
{
	return m_pTexture->GetImage();
}

VkImageView CVKRenderTexture::GetImageView(void) const
{
	return m_pTexture->GetImageView();
}

VkImageAspectFlags CVKRenderTexture::GetImageAspectFlags(void) const
{
	return m_pTexture->GetImageAspectFlags();
}

GfxPixelFormat CVKRenderTexture::GetFormat(void) const
{
	return m_pTexture->GetFormat();
}

int CVKRenderTexture::GetWidth(void) const
{
	return m_pTexture->GetWidth();
}

int CVKRenderTexture::GetHeight(void) const
{
	return m_pTexture->GetHeight();
}

int CVKRenderTexture::GetSamples(void) const
{
	return m_pTexture->GetSamples();
}
