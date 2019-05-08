#include "VKRenderer.h"


CVKRenderTexture::CVKRenderTexture(CVKDevice* pDevice, CVKRenderTextureManager* pManager, uint32_t name)
	: CGfxRenderTexture(name)
	, m_pDevice(pDevice)
	, m_pManager(pManager)
{

}

CVKRenderTexture::~CVKRenderTexture(void)
{
	Destroy();
}

void CVKRenderTexture::Release(void)
{
	m_pManager->Destroy(this);
}

VkImageView CVKRenderTexture::GetImageView(void) const
{
	if (m_ptrTexture) {
		return m_ptrTexture->GetImageView();
	}
	else {
		return VK_NULL_HANDLE;
	}
}

VkImageLayout CVKRenderTexture::GetImageLayout(void) const
{
	if (m_ptrTexture) {
		return m_ptrTexture->GetImageLayout();
	}
	else {
		return VK_IMAGE_LAYOUT_UNDEFINED;
	}
}

VkImageAspectFlags CVKRenderTexture::GetImageAspectFlags(void) const
{
	if (m_ptrTexture) {
		return m_ptrTexture->GetImageAspectFlags();
	}
	else {
		return 0;
	}
}

GfxTextureType CVKRenderTexture::GetType(void) const
{
	if (m_ptrTexture) {
		return m_ptrTexture->GetType();
	}
	else {
		return GFX_TEXTURE_2D;
	}
}

GfxPixelFormat CVKRenderTexture::GetFormat(void) const
{
	if (m_ptrTexture) {
		return m_ptrTexture->GetFormat();
	}
	else {
		return GFX_PIXELFORMAT_UNDEFINED;
	}
}

int CVKRenderTexture::GetWidth(void) const
{
	if (m_ptrTexture) {
		return m_ptrTexture->GetWidth();
	}
	else {
		return 0;
	}
}

int CVKRenderTexture::GetHeight(void) const
{
	if (m_ptrTexture) {
		return m_ptrTexture->GetHeight();
	}
	else {
		return 0;
	}
}

int CVKRenderTexture::GetSamples(void) const
{
	if (m_ptrTexture) {
		return m_ptrTexture->GetSamples();
	}
	else {
		return 0;
	}
}

bool CVKRenderTexture::Create(HANDLE hExternalTexture, GfxPixelFormat format, int width, int height, int samples)
{
	if (m_ptrTexture && CVKHelper::IsFormatSupported((VkFormat)format)) {
		samples = std::max(samples, 1);

		if (CVKHelper::IsFormatDepthOnly((VkFormat)format)) {
			return m_ptrTexture->Create(samples == 1 ? GFX_TEXTURE_2D : GFX_TEXTURE_2D_MULTISAMPLE, format, width, height, 1, 1, samples, VK_IMAGE_ASPECT_DEPTH_BIT, (VkImageView)hExternalTexture);
		}

		if (CVKHelper::IsFormatStencilOnly((VkFormat)format)) {
			return m_ptrTexture->Create(samples == 1 ? GFX_TEXTURE_2D : GFX_TEXTURE_2D_MULTISAMPLE, format, width, height, 1, 1, samples, VK_IMAGE_ASPECT_STENCIL_BIT, (VkImageView)hExternalTexture);
		}

		if (CVKHelper::IsFormatDepthStencil((VkFormat)format)) {
			return m_ptrTexture->Create(samples == 1 ? GFX_TEXTURE_2D : GFX_TEXTURE_2D_MULTISAMPLE, format, width, height, 1, 1, samples, VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT, (VkImageView)hExternalTexture);
		}

		return m_ptrTexture->Create(samples == 1 ? GFX_TEXTURE_2D : GFX_TEXTURE_2D_MULTISAMPLE, format, width, height, 1, 1, samples, VK_IMAGE_ASPECT_COLOR_BIT, (VkImageView)hExternalTexture);
	}
	else {
		return false;
	}
}

bool CVKRenderTexture::Create(GfxPixelFormat format, int width, int height, int samples, bool bTransient)
{
	if (m_ptrTexture && CVKHelper::IsFormatSupported((VkFormat)format)) {
		samples = std::max(samples, 1);

		if (CVKHelper::IsFormatDepthOnly((VkFormat)format)) {
			return m_ptrTexture->Create(samples == 1 ? GFX_TEXTURE_2D : GFX_TEXTURE_2D_MULTISAMPLE, format, width, height, 1, 1, samples, VK_IMAGE_ASPECT_DEPTH_BIT, VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT | VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | (bTransient ? VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT : VK_IMAGE_USAGE_TRANSFER_SRC_BIT), VK_IMAGE_TILING_OPTIMAL);
		}

		if (CVKHelper::IsFormatStencilOnly((VkFormat)format)) {
			return m_ptrTexture->Create(samples == 1 ? GFX_TEXTURE_2D : GFX_TEXTURE_2D_MULTISAMPLE, format, width, height, 1, 1, samples, VK_IMAGE_ASPECT_STENCIL_BIT, VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT | VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | (bTransient ? VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT : VK_IMAGE_USAGE_TRANSFER_SRC_BIT), VK_IMAGE_TILING_OPTIMAL);
		}

		if (CVKHelper::IsFormatDepthStencil((VkFormat)format)) {
			return m_ptrTexture->Create(samples == 1 ? GFX_TEXTURE_2D : GFX_TEXTURE_2D_MULTISAMPLE, format, width, height, 1, 1, samples, VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT, VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT | VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | (bTransient ? VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT : VK_IMAGE_USAGE_TRANSFER_SRC_BIT), VK_IMAGE_TILING_OPTIMAL);
		}

		return m_ptrTexture->Create(samples == 1 ? GFX_TEXTURE_2D : GFX_TEXTURE_2D_MULTISAMPLE, format, width, height, 1, 1, samples, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | (bTransient ? VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT : VK_IMAGE_USAGE_TRANSFER_SRC_BIT), VK_IMAGE_TILING_OPTIMAL);
	}
	else {
		return false;
	}
}

void CVKRenderTexture::Destroy(void)
{
	if (m_ptrTexture) {
		m_ptrTexture->Destroy();
	}
}
