#include "VKRenderer.h"


static VkExtent2D GetSwapchainExtent(const VkSurfaceCapabilitiesKHR& capabilities, uint32_t width, uint32_t height)
{
	VkExtent2D extent = { width, height };

	extent.width = std::max(extent.width, capabilities.minImageExtent.width);
	extent.width = std::min(extent.width, capabilities.maxImageExtent.width);
	extent.height = std::max(extent.height, capabilities.minImageExtent.height);
	extent.height = std::min(extent.height, capabilities.maxImageExtent.height);

	return extent;
}

static VkImageUsageFlags GetSwapchainUsageFlags(const VkSurfaceCapabilitiesKHR& capabilities)
{
	VkImageUsageFlags usageFlags = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
	return usageFlags & capabilities.supportedUsageFlags;
}

static VkSurfaceTransformFlagBitsKHR GetSwapchainTransform(const VkSurfaceCapabilitiesKHR& capabilities, VkSurfaceTransformFlagBitsKHR transform)
{
	if (capabilities.supportedTransforms & transform) {
		return transform;
	}
	else {
		return capabilities.currentTransform;
	}
}

static VkPresentModeKHR GetSwapchainPresentMode(const eastl::vector<VkPresentModeKHR>& modes)
{
	for (int indexMode = 0; indexMode < modes.size(); indexMode++) {
		if (modes[indexMode] == VK_PRESENT_MODE_FIFO_KHR) {
			return modes[indexMode];
		}
	}

	for (int indexMode = 0; indexMode < modes.size(); indexMode++) {
		if (modes[indexMode] == VK_PRESENT_MODE_MAILBOX_KHR) {
			return modes[indexMode];
		}
	}

	return VK_PRESENT_MODE_IMMEDIATE_KHR;
}

static VkSurfaceFormatKHR GetSwapchainFormat(const eastl::vector<VkSurfaceFormatKHR>& formats, VkFormat format)
{
	for (int indexFormat = 0; indexFormat < formats.size(); indexFormat++) {
		if (formats[indexFormat].format == format) {
			return formats[indexFormat];
		}
	}

	return { VK_FORMAT_B8G8R8A8_UNORM, VK_COLORSPACE_SRGB_NONLINEAR_KHR };
}


CVKSwapChain::CVKSwapChain(CVKDevice* pDevice, int width, int height, GfxPixelFormat format)
	: CGfxSwapChain(width, height, format)
	, m_pDevice(pDevice)

	, m_vkImages{ VK_NULL_HANDLE }
	, m_vkSwapchain(VK_NULL_HANDLE)
	, m_vkAcquireSemaphore(VK_NULL_HANDLE)

	, m_format(format)
	, m_width(width)
	, m_height(height)

	, m_indexFrame(0)
{
	VkSurfaceCapabilitiesKHR capabilities;
	eastl::vector<VkPresentModeKHR> modes;
	eastl::vector<VkSurfaceFormatKHR> formats;
	CALL_BOOL_FUNCTION_RETURN(IsSurfaceSupport());
	CALL_BOOL_FUNCTION_RETURN(EnumDeviceSurfaceModes(modes));
	CALL_BOOL_FUNCTION_RETURN(EnumDeviceSurfaceFormats(formats));
	CALL_BOOL_FUNCTION_RETURN(EnumDeviceSurfaceCapabilities(capabilities));
	CALL_BOOL_FUNCTION_RETURN(CreateSwapChain(modes, formats, capabilities));
	CALL_BOOL_FUNCTION_RETURN(CreateRenderTextures());
}

CVKSwapChain::~CVKSwapChain(void)
{
	DestroyRenderTextures();
	DestroySwapChain();
}

bool CVKSwapChain::IsSurfaceSupport(void) const
{
	VkBool32 surfaceSupported;
	CALL_VK_FUNCTION_RETURN_BOOL(vkGetPhysicalDeviceSurfaceSupportKHR(m_pDevice->GetPhysicalDevice(), m_pDevice->GetQueue()->GetQueueFamilyIndex(), m_pDevice->GetInstance()->GetSurface(), &surfaceSupported));
	return surfaceSupported == VK_TRUE;
}

bool CVKSwapChain::EnumDeviceSurfaceModes(eastl::vector<VkPresentModeKHR>& modes) const
{
	uint32_t numModes;

	CALL_VK_FUNCTION_RETURN_BOOL(vkGetPhysicalDeviceSurfacePresentModesKHR(m_pDevice->GetPhysicalDevice(), m_pDevice->GetInstance()->GetSurface(), &numModes, nullptr));
	if (numModes == 0) return false;

	modes.resize(numModes);
	CALL_VK_FUNCTION_RETURN_BOOL(vkGetPhysicalDeviceSurfacePresentModesKHR(m_pDevice->GetPhysicalDevice(), m_pDevice->GetInstance()->GetSurface(), &numModes, modes.data()));

	return true;
}

bool CVKSwapChain::EnumDeviceSurfaceFormats(eastl::vector<VkSurfaceFormatKHR>& formats) const
{
	uint32_t numFormats;

	CALL_VK_FUNCTION_RETURN_BOOL(vkGetPhysicalDeviceSurfaceFormatsKHR(m_pDevice->GetPhysicalDevice(), m_pDevice->GetInstance()->GetSurface(), &numFormats, nullptr));
	if (numFormats == 0) return false;

	formats.resize(numFormats);
	CALL_VK_FUNCTION_RETURN_BOOL(vkGetPhysicalDeviceSurfaceFormatsKHR(m_pDevice->GetPhysicalDevice(), m_pDevice->GetInstance()->GetSurface(), &numFormats, formats.data()));

	return true;
}

bool CVKSwapChain::EnumDeviceSurfaceCapabilities(VkSurfaceCapabilitiesKHR& capabilities) const
{
	CALL_VK_FUNCTION_RETURN_BOOL(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_pDevice->GetPhysicalDevice(), m_pDevice->GetInstance()->GetSurface(), &capabilities));
	return true;
}

bool CVKSwapChain::CreateSwapChain(const eastl::vector<VkPresentModeKHR>& modes, const eastl::vector<VkSurfaceFormatKHR>& formats, const VkSurfaceCapabilitiesKHR& capabilities)
{
	VkExtent2D imageExtent = GetSwapchainExtent(capabilities, m_width, m_height);
	VkImageUsageFlags imageUsage = GetSwapchainUsageFlags(capabilities);
	VkSurfaceTransformFlagBitsKHR preTransform = GetSwapchainTransform(capabilities, VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR);
	VkPresentModeKHR presentMode = GetSwapchainPresentMode(modes);
	VkSurfaceFormatKHR imageFormat = GetSwapchainFormat(formats, (VkFormat)m_format);

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

	return true;
}

bool CVKSwapChain::CreateRenderTextures(void)
{
	uint32_t numImages;

	CALL_VK_FUNCTION_RETURN_BOOL(vkGetSwapchainImagesKHR(m_pDevice->GetDevice(), m_vkSwapchain, &numImages, nullptr));
	ASSERT(numImages == SWAPCHAIN_FRAME_COUNT);
	CALL_VK_FUNCTION_RETURN_BOOL(vkGetSwapchainImagesKHR(m_pDevice->GetDevice(), m_vkSwapchain, &numImages, m_vkImages));

	for (int indexFrame = 0; indexFrame < SWAPCHAIN_FRAME_COUNT; indexFrame++) {
		m_ptrRenderTextures[indexFrame] = VKRenderer()->NewRenderTexture(HashValueFormat("SwapChain Frame RenderTexture %d", indexFrame));
		CALL_BOOL_FUNCTION_RETURN_BOOL(m_ptrRenderTextures[indexFrame]->Create((HANDLE)m_vkImages[indexFrame], m_format, m_width, m_height));
	}

	return true;
}

void CVKSwapChain::DestroySwapChain(void)
{
	ASSERT(m_vkSwapchain);
	ASSERT(m_vkAcquireSemaphore);

	vkDestroySwapchainKHR(m_pDevice->GetDevice(), m_vkSwapchain, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
	vkDestroySemaphore(m_pDevice->GetDevice(), m_vkAcquireSemaphore, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());

	m_vkSwapchain = VK_NULL_HANDLE;
	m_vkAcquireSemaphore = VK_NULL_HANDLE;
}

void CVKSwapChain::DestroyRenderTextures(void)
{
	for (int indexFrame = 0; indexFrame < SWAPCHAIN_FRAME_COUNT; indexFrame++) {
		m_ptrRenderTextures[indexFrame].Release();
	}
}

VkSemaphore CVKSwapChain::GetAcquireSemaphore(void) const
{
	ASSERT(m_vkAcquireSemaphore);
	return m_vkAcquireSemaphore;
}

GfxPixelFormat CVKSwapChain::GetFormat(void) const
{
	return m_format;
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
	return m_indexFrame;
}

const CGfxRenderTexturePtr CVKSwapChain::GetFrameTexture(int index) const
{
	ASSERT(index >= 0 && index < SWAPCHAIN_FRAME_COUNT);
	ASSERT(m_ptrRenderTextures[index]);
	return m_ptrRenderTextures[index];
}

void CVKSwapChain::Present(CGfxSemaphore* pWaitSemaphore)
{
	ASSERT(m_vkSwapchain);

	uint32_t indexFrame = m_indexFrame;
	VkSemaphore vkWaitSemaphore = ((CVKSemaphore *)pWaitSemaphore)->GetSemaphore();

	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.pNext = nullptr;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = &vkWaitSemaphore;
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = &m_vkSwapchain;
	presentInfo.pImageIndices = &indexFrame;
	presentInfo.pResults = nullptr;
	vkQueuePresentKHR(m_pDevice->GetQueue()->GetQueue(), &presentInfo);
}

void CVKSwapChain::AcquireNextFrame(void)
{
	ASSERT(m_vkSwapchain);
	ASSERT(m_vkAcquireSemaphore);

	uint32_t indexFrame;
	vkAcquireNextImageKHR(m_pDevice->GetDevice(), m_vkSwapchain, UINT64_MAX, m_vkAcquireSemaphore, VK_NULL_HANDLE, &indexFrame);

	m_indexFrame = indexFrame;
}
