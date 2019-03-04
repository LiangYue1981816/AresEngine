#pragma once
#include "VKRenderer.h"


class CVKSwapChain : public CGfxSwapChain
{
	friend class CVKRenderer;


private:
	CVKSwapChain(CVKDevice *pDevice, int width, int height, GfxPixelFormat pixelFormat);
	virtual ~CVKSwapChain(void);


private:
	bool IsSurfaceSupport(void) const;
	bool EnumDeviceSurfaceModes(eastl::vector<VkPresentModeKHR> &modes) const;
	bool EnumDeviceSurfaceFormats(eastl::vector<VkSurfaceFormatKHR> &formats) const;
	bool EnumDeviceSurfaceCapabilities(VkSurfaceCapabilitiesKHR &capabilities) const;

	bool CreateSwapChain(const eastl::vector<VkPresentModeKHR> &modes, const eastl::vector<VkSurfaceFormatKHR> &formats, const VkSurfaceCapabilitiesKHR &capabilities);
	bool CreateImagesAndImageViews(void);
	void DestroySwapChain(void);
	void DestroyImagesAndImageViews(void);

public:
	GfxPixelFormat GetPixelFormat(void) const;

	int GetWidth(void) const;
	int GetHeight(void) const;

public:
	int GetFrameIndex(void) const;
	CGfxRenderTexturePtr GetFrameTexture(int index) const;

public:
	void Present(void);
	void AcquireNextFrame(void);

	VkSemaphore GetAcquireSemaphore(void) const;
	VkSemaphore GetRenderDoneSemaphore(void) const;


private:
	GfxPixelFormat m_pixelFormat;

	int m_width;
	int m_height;

private:
	uint32_t m_indexImage;
	VkImage m_vkImages[SWAPCHAIN_FRAME_COUNT];
	VkImageView m_vkImageViews[SWAPCHAIN_FRAME_COUNT];
	CGfxRenderTexturePtr m_ptrRenderTextures[SWAPCHAIN_FRAME_COUNT];

private:
	VkSwapchainKHR m_vkSwapchain;
	VkSemaphore m_vkAcquireSemaphore;
	VkSemaphore m_vkRenderDoneSemaphores[SWAPCHAIN_FRAME_COUNT];

private:
	CVKDevice *m_pDevice;
};
