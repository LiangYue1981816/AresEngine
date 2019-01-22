#pragma once
#include "VKRenderer.h"


class CVKHelper
{
public:
	static void SetupFormat(VkPhysicalDevice vkPhysicalDevice);
	static const VkComponentMapping& GetFormatComponentMapping(VkFormat format);

	static bool IsFormatSupported(VkFormat format);
	static bool IsFormatDepthOnly(VkFormat format);
	static bool IsFormatStencilOnly(VkFormat format);
	static bool IsFormatDepthStencil(VkFormat format);
	static bool IsFormatDepthOrStencil(VkFormat format);

	static VkAccessFlags GetAccessMask(VkImageLayout layout);
	static VkPipelineStageFlags GetPipelineStageFlags(VkAccessFlags access);

	static VkCullModeFlags TranslateCullModeFlags(bool bEnableCullFace, GfxCullFace cullFace);
	static VkFrontFace TranslateFrontFace(GfxFrontFace frontFace);


private:
	static VkFormatProperties vkFormatProperties[VK_FORMAT_RANGE_SIZE];
	static VkComponentMapping vkFormatComponentMapping[VK_FORMAT_RANGE_SIZE];
};
