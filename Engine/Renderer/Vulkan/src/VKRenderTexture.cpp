#include "VKRenderer.h"


CVKRenderTexture::CVKRenderTexture(CVKDevice* pDevice, CVKRenderTextureManager* pManager, uint32_t name)
	: CGfxRenderTexture(name)
	, m_pDevice(pDevice)
	, m_pManager(pManager)
	, m_pTexture(nullptr)
{
	m_pTexture = new CVKTexture(m_pDevice);
}

CVKRenderTexture::~CVKRenderTexture(void)
{
	delete m_pTexture;
}

void CVKRenderTexture::Release(void)
{
	m_pManager->Destroy(this);
}

VkImageView CVKRenderTexture::GetImageView(void) const
{
	return m_pTexture->GetImageView();
}

VkImageLayout CVKRenderTexture::GetImageLayout(void) const
{
	return m_pTexture->GetImageLayout();
}

VkImageAspectFlags CVKRenderTexture::GetImageAspectFlags(void) const
{
	return m_pTexture->GetImageAspectFlags();
}

GfxTextureType CVKRenderTexture::GetType(void) const
{
	return m_pTexture->GetType();
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

bool CVKRenderTexture::Create(HANDLE hExternalTexture, GfxPixelFormat format, int width, int height, int samples)
{
	if (CVKHelper::IsFormatSupported((VkFormat)format)) {
		samples = std::max(samples, 1);

		if (CGfxHelper::IsFormatDepthOnly(format)) {
			return m_pTexture->Create(samples == 1 ? GFX_TEXTURE_2D : GFX_TEXTURE_2D_MULTISAMPLE, format, width, height, 1, 1, samples, VK_IMAGE_ASPECT_DEPTH_BIT, (VkImage)hExternalTexture, VK_IMAGE_LAYOUT_GENERAL);
		}

		if (CGfxHelper::IsFormatStencilOnly(format)) {
			return m_pTexture->Create(samples == 1 ? GFX_TEXTURE_2D : GFX_TEXTURE_2D_MULTISAMPLE, format, width, height, 1, 1, samples, VK_IMAGE_ASPECT_STENCIL_BIT, (VkImage)hExternalTexture, VK_IMAGE_LAYOUT_GENERAL);
		}

		if (CGfxHelper::IsFormatDepthAndStencil(format)) {
			return m_pTexture->Create(samples == 1 ? GFX_TEXTURE_2D : GFX_TEXTURE_2D_MULTISAMPLE, format, width, height, 1, 1, samples, VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT, (VkImage)hExternalTexture, VK_IMAGE_LAYOUT_GENERAL);
		}

		return m_pTexture->Create(samples == 1 ? GFX_TEXTURE_2D : GFX_TEXTURE_2D_MULTISAMPLE, format, width, height, 1, 1, samples, VK_IMAGE_ASPECT_COLOR_BIT, (VkImage)hExternalTexture, VK_IMAGE_LAYOUT_GENERAL);
	}
	else {
		return false;
	}
}

bool CVKRenderTexture::Create(GfxPixelFormat format, int width, int height, int samples, bool bTransient)
{
	if (CVKHelper::IsFormatSupported((VkFormat)format)) {
		samples = std::max(samples, 1);

		if (CGfxHelper::IsFormatDepthOnly(format)) {
			return m_pTexture->Create(samples == 1 ? GFX_TEXTURE_2D : GFX_TEXTURE_2D_MULTISAMPLE, format, width, height, 1, 1, samples, VK_IMAGE_ASPECT_DEPTH_BIT, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT | VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | (bTransient ? VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT : VK_IMAGE_USAGE_TRANSFER_SRC_BIT), VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_LAYOUT_GENERAL);
		}

		if (CGfxHelper::IsFormatStencilOnly(format)) {
			return m_pTexture->Create(samples == 1 ? GFX_TEXTURE_2D : GFX_TEXTURE_2D_MULTISAMPLE, format, width, height, 1, 1, samples, VK_IMAGE_ASPECT_STENCIL_BIT, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT | VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | (bTransient ? VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT : VK_IMAGE_USAGE_TRANSFER_SRC_BIT), VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_LAYOUT_GENERAL);
		}

		if (CGfxHelper::IsFormatDepthAndStencil(format)) {
			return m_pTexture->Create(samples == 1 ? GFX_TEXTURE_2D : GFX_TEXTURE_2D_MULTISAMPLE, format, width, height, 1, 1, samples, VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT | VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | (bTransient ? VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT : VK_IMAGE_USAGE_TRANSFER_SRC_BIT), VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_LAYOUT_GENERAL);
		}

		return m_pTexture->Create(samples == 1 ? GFX_TEXTURE_2D : GFX_TEXTURE_2D_MULTISAMPLE, format, width, height, 1, 1, samples, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | (bTransient ? VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT : VK_IMAGE_USAGE_TRANSFER_SRC_BIT), VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_LAYOUT_GENERAL);
	}
	else {
		return false;
	}
}

void CVKRenderTexture::Destroy(void)
{
	m_pTexture->Destroy();
}

bool CVKRenderTexture::PipelineBarrier(VkCommandBuffer vkCommandBuffer, VkImageLayout imageLayout)
{
	return m_pTexture->PipelineBarrier(vkCommandBuffer, imageLayout);
}

bool CVKRenderTexture::PipelineBarrier(VkCommandBuffer vkCommandBuffer, VkImageLayout imageLayout, VkAccessFlags srcAccessFlags, VkAccessFlags dstAccessFlags, VkPipelineStageFlags srcPipelineStageFlags, VkPipelineStageFlags dstPipelineStageFlags)
{
	return m_pTexture->PipelineBarrier(vkCommandBuffer, imageLayout, srcAccessFlags, dstAccessFlags, srcPipelineStageFlags, dstPipelineStageFlags);
}
