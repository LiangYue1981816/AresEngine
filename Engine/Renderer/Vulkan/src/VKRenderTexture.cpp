#include "VKRenderer.h"


CVKRenderTexture::CVKRenderTexture(CVKDevice *pDevice, CVKRenderTextureManager *pManager, uint32_t name)
	: CGfxRenderTexture(name)
	, m_name(name)
	, m_pDevice(pDevice)
	, m_pManager(pManager)

	, m_format(GFX_PIXELFORMAT_UNDEFINED)
	, m_type(GFX_TEXTURE_INVALID_ENUM)

	, m_width(0)
	, m_height(0)
	, m_samples(0)

	, m_vkImageView(VK_NULL_HANDLE)
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

uint32_t CVKRenderTexture::GetName(void) const
{
	return m_name;
}

HANDLE CVKRenderTexture::GetTexture(void) const
{
	return (HANDLE)m_vkImageView;
}

GfxPixelFormat CVKRenderTexture::GetFormat(void) const
{
	return m_format;
}

GfxTextureType CVKRenderTexture::GetType(void) const
{
	return m_type;
}

int CVKRenderTexture::GetWidth(void) const
{
	return m_width;
}

int CVKRenderTexture::GetHeight(void) const
{
	return m_height;
}

int CVKRenderTexture::GetSamples(void) const
{
	return m_samples;
}

bool CVKRenderTexture::Create(HANDLE hExternTexture, GfxPixelFormat pixelFormat, int width, int height, int samples)
{
	Destroy();

	m_bExtern = true;
	m_vkImageView = (VkImageView)hExternTexture;

	m_width = width;
	m_height = height;
	m_samples = std::max(samples, 1);

	m_format = pixelFormat;
	m_type = m_samples == 1 ? GFX_TEXTURE_2D : GFX_TEXTURE_2D_MULTISAMPLE;

	return true;
}

bool CVKRenderTexture::Create(GfxPixelFormat pixelFormat, int width, int height, int samples, bool bTransient)
{
	Destroy();

	m_width = width;
	m_height = height;
	m_samples = std::max(samples, 1);

	m_format = pixelFormat;
	m_type = m_samples == 1 ? GFX_TEXTURE_2D : GFX_TEXTURE_2D_MULTISAMPLE;

	if (CVKHelper::IsFormatSupported((VkFormat)pixelFormat) && CVKHelper::IsFormatDepthOnly((VkFormat)pixelFormat)) {
		CALL_BOOL_FUNCTION_RETURN_BOOL(CreateImage(VK_IMAGE_TYPE_2D, VK_IMAGE_VIEW_TYPE_2D, (VkFormat)pixelFormat, width, height, (VkSampleCountFlagBits)samples, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT | VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | (bTransient ? VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT : VK_IMAGE_USAGE_TRANSFER_SRC_BIT)));
		CALL_BOOL_FUNCTION_RETURN_BOOL(CreateView(VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_ASPECT_DEPTH_BIT, (VkFormat)pixelFormat));
		return true;
	}

	if (CVKHelper::IsFormatSupported((VkFormat)pixelFormat) && CVKHelper::IsFormatStencilOnly((VkFormat)pixelFormat)) {
		CALL_BOOL_FUNCTION_RETURN_BOOL(CreateImage(VK_IMAGE_TYPE_2D, VK_IMAGE_VIEW_TYPE_2D, (VkFormat)pixelFormat, width, height, (VkSampleCountFlagBits)samples, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT | VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | (bTransient ? VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT : VK_IMAGE_USAGE_TRANSFER_SRC_BIT)));
		CALL_BOOL_FUNCTION_RETURN_BOOL(CreateView(VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_ASPECT_STENCIL_BIT, (VkFormat)pixelFormat));
		return true;
	}

	if (CVKHelper::IsFormatSupported((VkFormat)pixelFormat) && CVKHelper::IsFormatDepthStencil((VkFormat)pixelFormat)) {
		CALL_BOOL_FUNCTION_RETURN_BOOL(CreateImage(VK_IMAGE_TYPE_2D, VK_IMAGE_VIEW_TYPE_2D, (VkFormat)pixelFormat, width, height, (VkSampleCountFlagBits)samples, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT | VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | (bTransient ? VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT : VK_IMAGE_USAGE_TRANSFER_SRC_BIT)));
		CALL_BOOL_FUNCTION_RETURN_BOOL(CreateView(VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT, (VkFormat)pixelFormat));
		return true;
	}

	if (CVKHelper::IsFormatSupported((VkFormat)pixelFormat)) {
		CALL_BOOL_FUNCTION_RETURN_BOOL(CreateImage(VK_IMAGE_TYPE_2D, VK_IMAGE_VIEW_TYPE_2D, (VkFormat)pixelFormat, width, height, (VkSampleCountFlagBits)samples, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | (bTransient ? VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT : VK_IMAGE_USAGE_TRANSFER_SRC_BIT)));
		CALL_BOOL_FUNCTION_RETURN_BOOL(CreateView(VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_ASPECT_COLOR_BIT, (VkFormat)pixelFormat));
		return true;
	}

	return false;
}

bool CVKRenderTexture::CreateView(VkImageViewType viewType, VkImageAspectFlags aspectMask, VkFormat format)
{
	VkImageViewCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.flags = 0;
	createInfo.image = m_ptrImage->GetImage();
	createInfo.viewType = viewType;
	createInfo.format = format;
	createInfo.components = CVKHelper::GetFormatComponentMapping(format);
	createInfo.subresourceRange = { aspectMask, 0, 1, 0, 1 };
	CALL_VK_FUNCTION_RETURN_BOOL(vkCreateImageView(m_pDevice->GetDevice(), &createInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkImageView));
	return true;
}

bool CVKRenderTexture::CreateImage(VkImageType imageType, VkImageViewType viewType, VkFormat format, int width, int height, VkSampleCountFlagBits samples, VkImageTiling imageTiling, VkImageUsageFlags imageUsageFlags)
{
	m_ptrImage = CVKImagePtr(new CVKImage(m_pDevice, imageType, viewType, format, width, height, 1, 1, samples, imageTiling, imageUsageFlags));
	return m_ptrImage->GetImage() != VK_NULL_HANDLE;
}

void CVKRenderTexture::Destroy(void)
{
	if (m_bExtern == false) {
		if (m_vkImageView) {
			vkDestroyImageView(m_pDevice->GetDevice(), m_vkImageView, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
		}
	}

	m_bExtern = false;
	m_vkImageView = VK_NULL_HANDLE;
	m_ptrImage.Release();

	m_format = GFX_PIXELFORMAT_UNDEFINED;
	m_type = GFX_TEXTURE_INVALID_ENUM;

	m_width = 0;
	m_height = 0;
	m_samples = 0;
}
