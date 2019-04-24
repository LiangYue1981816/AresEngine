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

	static VkSampleCountFlagBits TranslateSampleCount(int samples);
	static VkPrimitiveTopology TranslatePrimitiveTopology(GfxPrimitiveTopology topology);
	static VkPolygonMode TranslatePolytonMode(GfxPolygonMode polygonMode);
	static VkCullModeFlags TranslateCullModeFlags(bool bEnableCullFace, GfxCullFace cullFace);
	static VkFrontFace TranslateFrontFace(GfxFrontFace frontFace);
	static VkCompareOp TranslateCompareOp(GfxCompareOp op);
	static VkStencilOp TranslateStencilOp(GfxStencilOp op);
	static VkBlendOp TranslateBlendOp(GfxBlendOp op);
	static VkBlendFactor TranslateBlendFactor(GfxBlendFactor blendFactor);
	static VkImageViewType TranslateImageViewType(GfxTextureType type);


private:
	static VkFormatProperties vkFormatProperties[VK_FORMAT_RANGE_SIZE];
	static VkComponentMapping vkFormatComponentMapping[VK_FORMAT_RANGE_SIZE];
};
