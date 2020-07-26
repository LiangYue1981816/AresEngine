#pragma once
#include "VKRenderer.h"


class CVKSwapChain : public CGfxSwapChain
{
	friend class CVKRenderer;


private:
	CVKSwapChain(CVKDevice* pDevice, int width, int height, GfxPixelFormat format);
	virtual ~CVKSwapChain(void);


private:
	bool IsSurfaceSupport(void) const;
	bool EnumDeviceSurfaceModes(eastl::vector<VkPresentModeKHR>& modes) const;
	bool EnumDeviceSurfaceFormats(eastl::vector<VkSurfaceFormatKHR>& formats) const;
	bool EnumDeviceSurfaceCapabilities(VkSurfaceCapabilitiesKHR& capabilities) const;

	bool CreateSwapChain(const eastl::vector<VkPresentModeKHR>& modes, const eastl::vector<VkSurfaceFormatKHR>& formats, const VkSurfaceCapabilitiesKHR& capabilities);
	bool CreateRenderTextures(void);

	void DestroySwapChain(void);
	void DestroyRenderTextures(void);

public:
	VkImage GetImage(int index) const;
	VkSwapchain GetSwapchain(void) const;
	VkSemaphore GetSemaphore(void) const;

	const CGfxSemaphore* GetAcquireSemaphore(void) const;

public:
	GfxPixelFormat GetFormat(void) const;

	int GetWidth(void) const;
	int GetHeight(void) const;

public:
	int GetFrameIndex(void) const;
	const CGfxRenderTexturePtr GetFrameTexture(int index) const;

public:
	void Present(const CGfxSemaphore* pWaitSemaphore);
	void AcquireNextFrame(void);


private:
	VkSwapchain m_vkSwapchain;

private:
	VkImage m_vkImages[SWAPCHAIN_FRAME_COUNT];
	CGfxRenderTexturePtr m_ptrRenderTextures[SWAPCHAIN_FRAME_COUNT];

private:
	GfxPixelFormat m_format;

	int m_width;
	int m_height;

	int m_indexFrame;

private:
	CGfxSemaphore* m_pAcquireSemaphore;

private:
	CVKDevice* m_pDevice;
};
