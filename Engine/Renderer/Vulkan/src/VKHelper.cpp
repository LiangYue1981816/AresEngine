#include "VKRenderer.h"


VkFormatProperties CVKHelper::vkFormatProperties[VK_FORMAT_RANGE_SIZE];
VkComponentMapping CVKHelper::vkFormatComponentMapping[VK_FORMAT_RANGE_SIZE];

void CVKHelper::SetupFormat(VkPhysicalDevice vkPhysicalDevice)
{
	for (uint32_t indexFormat = 0; indexFormat < VK_FORMAT_RANGE_SIZE; indexFormat++) {
		vkGetPhysicalDeviceFormatProperties(vkPhysicalDevice, (VkFormat)indexFormat, &vkFormatProperties[indexFormat]);
		vkFormatComponentMapping[indexFormat].r = VK_COMPONENT_SWIZZLE_R;
		vkFormatComponentMapping[indexFormat].g = VK_COMPONENT_SWIZZLE_G;
		vkFormatComponentMapping[indexFormat].b = VK_COMPONENT_SWIZZLE_B;
		vkFormatComponentMapping[indexFormat].a = VK_COMPONENT_SWIZZLE_A;
	}

	vkFormatComponentMapping[VK_FORMAT_B8G8R8A8_UNORM] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
	vkFormatComponentMapping[VK_FORMAT_R8_UNORM] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO };
	vkFormatComponentMapping[VK_FORMAT_R16_UNORM] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO };
	vkFormatComponentMapping[VK_FORMAT_B10G11R11_UFLOAT_PACK32] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_ZERO };
	vkFormatComponentMapping[VK_FORMAT_R16G16B16A16_SFLOAT] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
	vkFormatComponentMapping[VK_FORMAT_D24_UNORM_S8_UINT] = { VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY };
	vkFormatComponentMapping[VK_FORMAT_D32_SFLOAT_S8_UINT] = { VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY };
	vkFormatComponentMapping[VK_FORMAT_D16_UNORM_S8_UINT] = { VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY };
	vkFormatComponentMapping[VK_FORMAT_D16_UNORM] = { VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY };
	vkFormatComponentMapping[VK_FORMAT_R32G32_SFLOAT] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO };
	vkFormatComponentMapping[VK_FORMAT_R32G32B32A32_SFLOAT] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
	vkFormatComponentMapping[VK_FORMAT_R16G16_UNORM] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO };
	vkFormatComponentMapping[VK_FORMAT_R16G16_SFLOAT] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO };
	vkFormatComponentMapping[VK_FORMAT_R16_UINT] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO };
	vkFormatComponentMapping[VK_FORMAT_R16_SINT] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO };
	vkFormatComponentMapping[VK_FORMAT_R32_UINT] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO };
	vkFormatComponentMapping[VK_FORMAT_R32_SINT] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO };
	vkFormatComponentMapping[VK_FORMAT_R8_UINT] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO };
	vkFormatComponentMapping[VK_FORMAT_X8_D24_UNORM_PACK32] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO };
	vkFormatComponentMapping[VK_FORMAT_D32_SFLOAT] = { VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY };
	vkFormatComponentMapping[VK_FORMAT_R16_SFLOAT] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO };
	vkFormatComponentMapping[VK_FORMAT_A2B10G10R10_UNORM_PACK32] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
	vkFormatComponentMapping[VK_FORMAT_R16G16B16A16_UNORM] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
	vkFormatComponentMapping[VK_FORMAT_R5G6B5_UNORM_PACK16] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_ZERO };
	vkFormatComponentMapping[VK_FORMAT_R8G8B8A8_UNORM] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
	vkFormatComponentMapping[VK_FORMAT_R16G16_UINT] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO };
	vkFormatComponentMapping[VK_FORMAT_R16G16B16A16_UINT] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
	vkFormatComponentMapping[VK_FORMAT_R16G16B16A16_SINT] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
	vkFormatComponentMapping[VK_FORMAT_R32G32B32_UINT] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_ZERO };
	vkFormatComponentMapping[VK_FORMAT_R8G8_UNORM] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO };
	vkFormatComponentMapping[VK_FORMAT_R32_SFLOAT] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO };
	vkFormatComponentMapping[VK_FORMAT_BC1_RGB_UNORM_BLOCK] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_ZERO };
	vkFormatComponentMapping[VK_FORMAT_BC2_UNORM_BLOCK] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
	vkFormatComponentMapping[VK_FORMAT_BC3_UNORM_BLOCK] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
	vkFormatComponentMapping[VK_FORMAT_BC4_UNORM_BLOCK] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
	vkFormatComponentMapping[VK_FORMAT_BC5_UNORM_BLOCK] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
	vkFormatComponentMapping[VK_FORMAT_BC6H_UFLOAT_BLOCK] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
	vkFormatComponentMapping[VK_FORMAT_BC7_UNORM_BLOCK] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
	vkFormatComponentMapping[VK_FORMAT_ASTC_4x4_UNORM_BLOCK] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
	vkFormatComponentMapping[VK_FORMAT_ASTC_6x6_UNORM_BLOCK] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
	vkFormatComponentMapping[VK_FORMAT_ASTC_8x8_UNORM_BLOCK] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
	vkFormatComponentMapping[VK_FORMAT_ASTC_10x10_UNORM_BLOCK] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
	vkFormatComponentMapping[VK_FORMAT_ASTC_12x12_UNORM_BLOCK] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
	vkFormatComponentMapping[VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_ZERO };
	vkFormatComponentMapping[VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK] = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
}

const VkComponentMapping& CVKHelper::GetFormatComponentMapping(VkFormat format)
{
	return vkFormatComponentMapping[format];
}

bool CVKHelper::IsFormatSupported(VkFormat format)
{
	return vkFormatProperties[format].bufferFeatures || vkFormatProperties[format].linearTilingFeatures || vkFormatProperties[format].optimalTilingFeatures;
}

bool CVKHelper::IsFormatDepthOnly(VkFormat format)
{
	switch ((int)format) {
	case VK_FORMAT_D16_UNORM:
	case VK_FORMAT_X8_D24_UNORM_PACK32:
	case VK_FORMAT_D32_SFLOAT:
		return true;

	default:
		return false;
	}
}

bool CVKHelper::IsFormatStencilOnly(VkFormat format)
{
	switch ((int)format) {
	case VK_FORMAT_S8_UINT:
		return true;

	default:
		return false;
	}
}

bool CVKHelper::IsFormatDepthStencil(VkFormat format)
{
	switch ((int)format) {
	case VK_FORMAT_D16_UNORM_S8_UINT:
	case VK_FORMAT_D24_UNORM_S8_UINT:
	case VK_FORMAT_D32_SFLOAT_S8_UINT:
		return true;

	default:
		return false;
	}
}

bool CVKHelper::IsFormatDepthOrStencil(VkFormat format)
{
	return IsFormatStencilOnly(format) || IsFormatDepthOnly(format) || IsFormatDepthStencil(format);
}

VkAccessFlags CVKHelper::GetAccessMask(VkImageLayout layout)
{
	switch ((int)layout) {
	case VK_IMAGE_LAYOUT_PREINITIALIZED:
		return VK_ACCESS_HOST_WRITE_BIT;

	case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
		return VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
		return VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

	case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
		return VK_ACCESS_TRANSFER_READ_BIT;

	case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
		return VK_ACCESS_TRANSFER_WRITE_BIT;

	case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
		return VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_INPUT_ATTACHMENT_READ_BIT;

	case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR:
		return VK_ACCESS_MEMORY_READ_BIT;

	default:
		return 0;
	}
}

VkPipelineStageFlags CVKHelper::GetPipelineStageFlags(VkAccessFlags access)
{
	switch ((int)access) {
	case VK_ACCESS_INDIRECT_COMMAND_READ_BIT:
		return VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT;

	case VK_ACCESS_INDEX_READ_BIT:
		return VK_PIPELINE_STAGE_VERTEX_INPUT_BIT;

	case VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT:
		return VK_PIPELINE_STAGE_VERTEX_INPUT_BIT;

	case VK_ACCESS_UNIFORM_READ_BIT:
		return VK_PIPELINE_STAGE_VERTEX_SHADER_BIT | VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;

	case VK_ACCESS_INPUT_ATTACHMENT_READ_BIT:
		return VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;

	case VK_ACCESS_SHADER_READ_BIT:
		return VK_PIPELINE_STAGE_VERTEX_SHADER_BIT | VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;

	case VK_ACCESS_SHADER_WRITE_BIT:
		return VK_PIPELINE_STAGE_VERTEX_SHADER_BIT | VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;

	case VK_ACCESS_COLOR_ATTACHMENT_READ_BIT:
		return VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

	case VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT:
		return VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

	case VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT:
		return VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;

	case VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT:
		return VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;

	case VK_ACCESS_TRANSFER_READ_BIT:
		return VK_PIPELINE_STAGE_TRANSFER_BIT;

	case VK_ACCESS_TRANSFER_WRITE_BIT:
		return VK_PIPELINE_STAGE_TRANSFER_BIT;

	case VK_ACCESS_HOST_READ_BIT:
		return VK_PIPELINE_STAGE_HOST_BIT;

	case VK_ACCESS_HOST_WRITE_BIT:
		return VK_PIPELINE_STAGE_HOST_BIT;

	default:
		return VK_PIPELINE_STAGE_FLAG_BITS_MAX_ENUM;
	}
}

VkPrimitiveTopology CVKHelper::TranslatePrimitiveTopology(GfxPrimitiveTopology topology)
{
	switch ((int)topology) {
	case GFX_PRIMITIVE_TOPOLOGY_POINT_LIST:
		return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;

	case GFX_PRIMITIVE_TOPOLOGY_LINE_LIST:
		return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;

	case GFX_PRIMITIVE_TOPOLOGY_LINE_STRIP:
		return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;

	case GFX_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST:
		return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

	case GFX_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP:
		return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;

	case GFX_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN:
		return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;

	default:
		return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	}
}

VkPolygonMode CVKHelper::TranslatePolytonMode(GfxPolygonMode polygonMode)
{
	switch ((int)polygonMode) {
	case GFX_POLYGON_MODE_FILL:
		return VK_POLYGON_MODE_FILL;

	case GFX_POLYGON_MODE_LINE:
		return VK_POLYGON_MODE_LINE;

	case GFX_POLYGON_MODE_POINT:
		return VK_POLYGON_MODE_POINT;

	default:
		return VK_POLYGON_MODE_FILL;
	}
}

VkCullModeFlags CVKHelper::TranslateCullModeFlags(bool bEnableCullFace, GfxCullFace cullFace)
{
	if (bEnableCullFace) {
		switch ((int)cullFace) {
		case GFX_CULLFACE_FRONT:
			return VK_CULL_MODE_FRONT_BIT;

		case GFX_CULLFACE_BACK:
			return VK_CULL_MODE_BACK_BIT;

		case GFX_CULLFACE_FRONT_AND_BACK:
			return VK_CULL_MODE_FRONT_AND_BACK;

		default:
			return VK_CULL_MODE_BACK_BIT;
		}
	}
	else {
		return VK_CULL_MODE_NONE;
	}
}

VkFrontFace CVKHelper::TranslateFrontFace(GfxFrontFace frontFace)
{
	switch ((int)frontFace) {
	case GFX_FRONTFACE_CW:
		return VK_FRONT_FACE_CLOCKWISE;

	case GFX_FRONTFACE_CCW:
		return VK_FRONT_FACE_COUNTER_CLOCKWISE;

	default:
		return VK_FRONT_FACE_COUNTER_CLOCKWISE;
	}
}

VkCompareOp CVKHelper::TranslateCompareOp(GfxCompareOp op)
{
	switch ((int)op) {
	case GFX_COMPAREOP_NEVER:
		return VK_COMPARE_OP_NEVER;

	case GFX_COMPAREOP_LESS:
		return VK_COMPARE_OP_LESS;

	case GFX_COMPAREOP_LEQUAL:
		return VK_COMPARE_OP_LESS_OR_EQUAL;

	case GFX_COMPAREOP_GREATER:
		return VK_COMPARE_OP_GREATER;

	case GFX_COMPAREOP_GEQUAL:
		return VK_COMPARE_OP_GREATER_OR_EQUAL;

	case GFX_COMPAREOP_EQUAL:
		return VK_COMPARE_OP_EQUAL;

	case GFX_COMPAREOP_NOTEQUAL:
		return VK_COMPARE_OP_NOT_EQUAL;

	case GFX_COMPAREOP_ALWAYS:
		return VK_COMPARE_OP_ALWAYS;

	default:
		return VK_COMPARE_OP_MAX_ENUM;
	}
}

VkStencilOp CVKHelper::TranslateStencilOp(GfxStencilOp op)
{
	switch ((int)op) {
	case GFX_STENCILOP_KEEP:
		return VK_STENCIL_OP_KEEP;

	case GFX_STENCILOP_ZERO:
		return VK_STENCIL_OP_ZERO;

	case GFX_STENCILOP_REPLACE:
		return VK_STENCIL_OP_REPLACE;

	case GFX_STENCILOP_INCR:
		return VK_STENCIL_OP_INCREMENT_AND_CLAMP;

	case GFX_STENCILOP_INCR_WRAP:
		return VK_STENCIL_OP_INCREMENT_AND_WRAP;

	case GFX_STENCILOP_DECR:
		return VK_STENCIL_OP_DECREMENT_AND_CLAMP;

	case GFX_STENCILOP_DECR_WRAP:
		return VK_STENCIL_OP_DECREMENT_AND_WRAP;

	case GFX_STENCILOP_INVERT:
		return VK_STENCIL_OP_INVERT;

	default:
		return VK_STENCIL_OP_MAX_ENUM;
	}
}
