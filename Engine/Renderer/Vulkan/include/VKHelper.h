#pragma once
#include "VKRenderer.h"


class CVKHelper
{
public:
	static void SetupFormat(VkPhysicalDevice vkPhysicalDevice);
	static const VkComponentMapping& GetFormatComponentMapping(VkFormat format);

	static VkBool32 IsFormatSupported(VkFormat format);
	static VkBool32 IsFormatDepthOnly(VkFormat format);
	static VkBool32 IsFormatStencilOnly(VkFormat format);
	static VkBool32 IsFormatDepthStencil(VkFormat format);

	static VkAccessFlags GetAccessMask(VkImageLayout layout);
	static VkPipelineStageFlags GetPipelineStageFlags(VkAccessFlags access);


private:
	static VkFormatProperties vkFormatProperties[VK_FORMAT_RANGE_SIZE];
	static VkComponentMapping vkFormatComponentMapping[VK_FORMAT_RANGE_SIZE];
};
