#include "VKRenderer.h"


CVKRenderTexture::CVKRenderTexture(CVKDevice *pDevice, CVKRenderTextureManager *pManager, uint32_t name)
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

bool CVKRenderTexture::Create(GfxPixelFormat pixelFormat, int width, int height, int samples)
{
	if (CVKHelper::IsFormatDepthOnly((VkFormat)pixelFormat)) {
		CALL_BOOL_FUNCTION_RETURN_BOOL(CreateImage(VK_IMAGE_TYPE_2D, VK_IMAGE_VIEW_TYPE_2D, (VkFormat)pixelFormat, width, height, (VkSampleCountFlagBits)samples, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT));
		CALL_BOOL_FUNCTION_RETURN_BOOL(CreateView(VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_ASPECT_DEPTH_BIT, (VkFormat)pixelFormat));
		return true;
	}

	if (CVKHelper::IsFormatStencilOnly((VkFormat)pixelFormat)) {
		CALL_BOOL_FUNCTION_RETURN_BOOL(CreateImage(VK_IMAGE_TYPE_2D, VK_IMAGE_VIEW_TYPE_2D, (VkFormat)pixelFormat, width, height, (VkSampleCountFlagBits)samples, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT));
		CALL_BOOL_FUNCTION_RETURN_BOOL(CreateView(VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_ASPECT_STENCIL_BIT, (VkFormat)pixelFormat));
		return true;
	}

	if (CVKHelper::IsFormatDepthStencil((VkFormat)pixelFormat)) {
		CALL_BOOL_FUNCTION_RETURN_BOOL(CreateImage(VK_IMAGE_TYPE_2D, VK_IMAGE_VIEW_TYPE_2D, (VkFormat)pixelFormat, width, height, (VkSampleCountFlagBits)samples, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT));
		CALL_BOOL_FUNCTION_RETURN_BOOL(CreateView(VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT, (VkFormat)pixelFormat));
		return true;
	}

	if (CVKHelper::IsFormatSupported((VkFormat)pixelFormat)) {
		CALL_BOOL_FUNCTION_RETURN_BOOL(CreateImage(VK_IMAGE_TYPE_2D, VK_IMAGE_VIEW_TYPE_2D, (VkFormat)pixelFormat, width, height, (VkSampleCountFlagBits)samples, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT));
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
	if (m_vkImageView) {
		vkDestroyImageView(m_pDevice->GetDevice(), m_vkImageView, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
	}

	m_vkImageView = VK_NULL_HANDLE;
	m_ptrImage.Release();
}
