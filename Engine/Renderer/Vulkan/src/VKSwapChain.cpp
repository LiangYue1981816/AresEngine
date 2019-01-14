#include "VKRenderer.h"


static VkExtent2D GetSwapchainExtent(const VkSurfaceCapabilitiesKHR &capabilities, uint32_t width, uint32_t height)
{
	VkExtent2D extent = { width, height };

	if (extent.width < capabilities.minImageExtent.width) extent.width = capabilities.minImageExtent.width;
	if (extent.width > capabilities.maxImageExtent.width) extent.width = capabilities.maxImageExtent.width;
	if (extent.height < capabilities.minImageExtent.height) extent.height = capabilities.minImageExtent.height;
	if (extent.height > capabilities.maxImageExtent.height) extent.height = capabilities.maxImageExtent.height;

	return extent;
}

static VkImageUsageFlags GetSwapchainUsageFlags(const VkSurfaceCapabilitiesKHR &capabilities)
{
	ASSERT((capabilities.supportedUsageFlags & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT) == VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);
	return VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT;
}

static VkSurfaceTransformFlagBitsKHR GetSwapchainTransform(const VkSurfaceCapabilitiesKHR &capabilities, VkSurfaceTransformFlagBitsKHR transform)
{
	if (capabilities.supportedTransforms & transform) {
		return transform;
	}
	else {
		return capabilities.currentTransform;
	}
}

static VkPresentModeKHR GetSwapchainPresentMode(const eastl::vector<VkPresentModeKHR> &modes)
{
	for (uint32_t index = 0; index < modes.size(); index++) {
		if (modes[index] == VK_PRESENT_MODE_FIFO_KHR) {
			return modes[index];
		}
	}

	for (uint32_t index = 0; index < modes.size(); index++) {
		if (modes[index] == VK_PRESENT_MODE_MAILBOX_KHR) {
			return modes[index];
		}
	}

	return VK_PRESENT_MODE_IMMEDIATE_KHR;
}

static VkSurfaceFormatKHR GetSwapchainFormat(const eastl::vector<VkSurfaceFormatKHR> &formats)
{
	if ((formats.size() == 1) && (formats[0].format == VK_FORMAT_UNDEFINED)) {
		return { VK_FORMAT_B8G8R8A8_UNORM, VK_COLORSPACE_SRGB_NONLINEAR_KHR };
	}

	for (uint32_t index = 0; index < formats.size(); index++) {
		if (formats[index].format == VK_FORMAT_B8G8R8A8_UNORM) {
			return formats[index];
		}
	}

	return formats[0];
}


CVKSwapChain::CVKSwapChain(CVKDevice *pDevice, int width, int height, GfxPixelFormat pixelFormat)
	: CGfxSwapChain(width, height, pixelFormat)
	, m_pDevice(pDevice)

	, m_pixelFormat(pixelFormat)
	, m_width(width)
	, m_height(height)

	, m_vkImages{ VK_NULL_HANDLE }
	, m_vkImageViews{ VK_NULL_HANDLE }

	, m_vkSwapchain(VK_NULL_HANDLE)
	, m_vkAcquireSemaphore(VK_NULL_HANDLE)
	, m_vkRenderDoneSemaphores{ VK_NULL_HANDLE }
{

}

CVKSwapChain::~CVKSwapChain(void)
{

}

bool CVKSwapChain::EnumDeviceSurfaceModes(eastl::vector<VkPresentModeKHR> &modes) const
{
	modes.clear();

	uint32_t numModes;
	CALL_VK_FUNCTION_RETURN_BOOL(vkGetPhysicalDeviceSurfacePresentModesKHR(m_pDevice->GetPhysicalDevice(), m_pDevice->GetInstance()->GetSurface(), &numModes, nullptr));
	if (numModes == 0) return false;

	modes.resize(numModes);
	CALL_VK_FUNCTION_RETURN_BOOL(vkGetPhysicalDeviceSurfacePresentModesKHR(m_pDevice->GetPhysicalDevice(), m_pDevice->GetInstance()->GetSurface(), &numModes, modes.data()));

	return true;
}

bool CVKSwapChain::EnumDeviceSurfaceFormats(eastl::vector<VkSurfaceFormatKHR> &formats) const
{
	formats.clear();

	uint32_t numFormats;
	CALL_VK_FUNCTION_RETURN_BOOL(vkGetPhysicalDeviceSurfaceFormatsKHR(m_pDevice->GetPhysicalDevice(), m_pDevice->GetInstance()->GetSurface(), &numFormats, nullptr));
	if (numFormats == 0) return false;

	formats.resize(numFormats);
	CALL_VK_FUNCTION_RETURN_BOOL(vkGetPhysicalDeviceSurfaceFormatsKHR(m_pDevice->GetPhysicalDevice(), m_pDevice->GetInstance()->GetSurface(), &numFormats, formats.data()));

	return true;
}

bool CVKSwapChain::EnumDeviceSurfaceCapabilities(VkSurfaceCapabilitiesKHR &capabilities) const
{
	CALL_VK_FUNCTION_RETURN_BOOL(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_pDevice->GetPhysicalDevice(), m_pDevice->GetInstance()->GetSurface(), &capabilities));
	return true;
}

bool CVKSwapChain::CreateSwapChain(const eastl::vector<VkPresentModeKHR> &modes, const eastl::vector<VkSurfaceFormatKHR> &formats, const VkSurfaceCapabilitiesKHR &capabilities)
{
	VkExtent2D imageExtent = GetSwapchainExtent(capabilities, m_width, m_height);
	VkImageUsageFlags imageUsage = GetSwapchainUsageFlags(capabilities);
	VkSurfaceTransformFlagBitsKHR preTransform = GetSwapchainTransform(capabilities, VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR);
	VkPresentModeKHR presentMode = GetSwapchainPresentMode(modes);
	VkSurfaceFormatKHR imageFormat = GetSwapchainFormat(formats);

	VkSwapchainCreateInfoKHR swapchainCreateInfo = {};
	swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchainCreateInfo.pNext = nullptr;
	swapchainCreateInfo.flags = 0;
	swapchainCreateInfo.surface = m_pDevice->GetInstance()->GetSurface();
	swapchainCreateInfo.minImageCount = SWAPCHAIN_FRAME_COUNT;
	swapchainCreateInfo.imageFormat = imageFormat.format;
	swapchainCreateInfo.imageColorSpace = imageFormat.colorSpace;
	swapchainCreateInfo.imageExtent = imageExtent;
	swapchainCreateInfo.imageArrayLayers = 1;
	swapchainCreateInfo.imageUsage = imageUsage;
	swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	swapchainCreateInfo.queueFamilyIndexCount = 0;
	swapchainCreateInfo.pQueueFamilyIndices = nullptr;
	swapchainCreateInfo.preTransform = preTransform;
	swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapchainCreateInfo.presentMode = presentMode;
	swapchainCreateInfo.clipped = VK_TRUE;
	swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;
	CALL_VK_FUNCTION_RETURN_BOOL(vkCreateSwapchainKHR(m_pDevice->GetDevice(), &swapchainCreateInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkSwapchain));

	VkSemaphoreCreateInfo semaphoreCreateInfo = {};
	semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	semaphoreCreateInfo.pNext = nullptr;
	semaphoreCreateInfo.flags = 0;
	CALL_VK_FUNCTION_RETURN_BOOL(vkCreateSemaphore(m_pDevice->GetDevice(), &semaphoreCreateInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkAcquireSemaphore));
	CALL_VK_FUNCTION_RETURN_BOOL(vkCreateSemaphore(m_pDevice->GetDevice(), &semaphoreCreateInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkRenderDoneSemaphores[0]));
	CALL_VK_FUNCTION_RETURN_BOOL(vkCreateSemaphore(m_pDevice->GetDevice(), &semaphoreCreateInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkRenderDoneSemaphores[1]));
	CALL_VK_FUNCTION_RETURN_BOOL(vkCreateSemaphore(m_pDevice->GetDevice(), &semaphoreCreateInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkRenderDoneSemaphores[2]));

	return true;
}

bool CVKSwapChain::CreateImagesAndImageViews(void)
{
	uint32_t numImages;
	CALL_VK_FUNCTION_RETURN_BOOL(vkGetSwapchainImagesKHR(m_pDevice->GetDevice(), m_vkSwapchain, &numImages, nullptr));
	if (numImages != SWAPCHAIN_FRAME_COUNT) return false;

	CALL_VK_FUNCTION_RETURN_BOOL(vkGetSwapchainImagesKHR(m_pDevice->GetDevice(), m_vkSwapchain, &numImages, m_vkImages));

	VkImageViewCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.flags = 0;
	createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	createInfo.format = (VkFormat)m_pixelFormat;
	createInfo.components = CVKHelper::GetFormatComponentMapping((VkFormat)m_pixelFormat);
	createInfo.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };

	for (int index = 0; index < SWAPCHAIN_FRAME_COUNT; index++) {
		createInfo.image = m_vkImages[index];
		CALL_VK_FUNCTION_RETURN_BOOL(vkCreateImageView(m_pDevice->GetDevice(), &createInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkImageViews[index]));

		char szName[_MAX_STRING] = { 0 };
		sprintf(szName, "SwapChain Frame Texture %d", index);
		m_ptrRenderTextures[index] = VKRenderer()->NewRenderTexture(HashValue(szName));
		CALL_BOOL_FUNCTION_RETURN_BOOL(m_ptrRenderTextures[index]->Create(m_vkImageViews[index], m_pixelFormat, m_width, m_height));
	}

	return true;
}

void CVKSwapChain::DestroySwapChain(void)
{
	if (m_vkSwapchain) {
		vkDestroySwapchainKHR(m_pDevice->GetDevice(), m_vkSwapchain, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
	}

	if (m_vkAcquireSemaphore) {
		vkDestroySemaphore(m_pDevice->GetDevice(), m_vkAcquireSemaphore, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
	}

	if (m_vkRenderDoneSemaphores[0]) {
		vkDestroySemaphore(m_pDevice->GetDevice(), m_vkRenderDoneSemaphores[0], m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
	}

	if (m_vkRenderDoneSemaphores[1]) {
		vkDestroySemaphore(m_pDevice->GetDevice(), m_vkRenderDoneSemaphores[1], m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
	}

	if (m_vkRenderDoneSemaphores[2]) {
		vkDestroySemaphore(m_pDevice->GetDevice(), m_vkRenderDoneSemaphores[2], m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
	}

	m_vkSwapchain = VK_NULL_HANDLE;
	m_vkAcquireSemaphore = VK_NULL_HANDLE;
	m_vkRenderDoneSemaphores[0] = VK_NULL_HANDLE;
	m_vkRenderDoneSemaphores[1] = VK_NULL_HANDLE;
	m_vkRenderDoneSemaphores[2] = VK_NULL_HANDLE;
}

void CVKSwapChain::DestroyImagesAndImageViews(void)
{
	if (m_vkImageViews[0]) {
		vkDestroyImageView(m_pDevice->GetDevice(), m_vkImageViews[0], m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
	}

	if (m_vkImageViews[1]) {
		vkDestroyImageView(m_pDevice->GetDevice(), m_vkImageViews[1], m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
	}

	if (m_vkImageViews[2]) {
		vkDestroyImageView(m_pDevice->GetDevice(), m_vkImageViews[2], m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
	}

	m_vkImages[0] = VK_NULL_HANDLE;
	m_vkImages[1] = VK_NULL_HANDLE;
	m_vkImages[2] = VK_NULL_HANDLE;
	m_vkImageViews[0] = VK_NULL_HANDLE;
	m_vkImageViews[1] = VK_NULL_HANDLE;
	m_vkImageViews[2] = VK_NULL_HANDLE;
	m_ptrRenderTextures[0].Release();
	m_ptrRenderTextures[1].Release();
	m_ptrRenderTextures[2].Release();
}

GfxPixelFormat CVKSwapChain::GetPixelFormat(void) const
{
	return m_pixelFormat;
}

int CVKSwapChain::GetWidth(void) const
{
	return m_width;
}

int CVKSwapChain::GetHeight(void) const
{
	return m_height;
}

int CVKSwapChain::GetFrameIndex(void) const
{
	return 0;
}

const CGfxRenderTexturePtr& CVKSwapChain::GetFrameTexture(int index) const
{
	return m_ptrRenderTextures[index];
}

void CVKSwapChain::Present(void)
{

}

void CVKSwapChain::AcquireNextFrame(void)
{

}
