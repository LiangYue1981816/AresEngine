#pragma once
#include "Application.h"
#include "VKRenderer.h"


class CALL_API CApplicationVulkan : public CApplication
{
public:
	CApplicationVulkan(void);
	virtual ~CApplicationVulkan(void);


public:
	virtual bool Create(void* hInstance, void* hWnd, void* hDC, int width, int height);
	virtual void Destroy(void);

private:
	virtual void UpdateInternal(float deltaTime);


private:
	void* m_hDC;
	int m_width;
	int m_height;

private:
	VkDescriptorPool m_vkDescriptorPool;
	VkRenderPass m_vkRenderPass;
	VkImageView m_vkImageViews[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];
	VkFramebuffer m_vkFramebuffers[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];
};
