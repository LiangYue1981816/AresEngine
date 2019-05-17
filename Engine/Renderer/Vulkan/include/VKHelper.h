#pragma once
#include "VKRenderer.h"


class CVKHelper
{
public:
	static void SetupFormat(VkPhysicalDevice vkPhysicalDevice);
	static const VkComponentMapping& GetFormatComponentMapping(VkFormat format);

	static bool IsFormatSupported(VkFormat format);

	static VkAccessFlags GetAccessMaskByImageLayout(VkImageLayout layout);
	static VkAccessFlags GetAccessMaskByBufferUsage(VkBufferUsageFlags usage);
	static VkPipelineStageFlags GetPipelineStageFlagsByImageLayout(VkImageLayout layout);
	static VkPipelineStageFlags GetPipelineStageFlagsByAccessFlags(VkAccessFlags access);
	static VkPipelineStageFlags GetPipelineStageFlagsByBufferUsage(VkBufferUsageFlags usage);

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
	static VkIndexType TranslateIndexType(GfxIndexType type);
	static VkFilter TranslateFilter(GfxFilter filter);
	static VkSamplerMipmapMode TranslateSamplerMipmapMode(GfxSamplerMipmapMode mipmapMode);
	static VkSamplerAddressMode TranslateSamplerAddressMode(GfxSamplerAddressMode addressMode);


private:
	static VkFormatProperties vkFormatProperties[VK_FORMAT_RANGE_SIZE];
	static VkComponentMapping vkFormatComponentMapping[VK_FORMAT_RANGE_SIZE];
};
