#pragma once


typedef enum GfxDataFormat {
	GFX_DATAFORMAT_UNDEFINED = 0,

	GFX_DATAFORMAT_UINT = 98,
	GFX_DATAFORMAT_SINT = 99,
	GFX_DATAFORMAT_SFLOAT = 100,

	GFX_DATAFORMAT_VEC2_UINT = 101,
	GFX_DATAFORMAT_VEC2_SINT = 102,
	GFX_DATAFORMAT_VEC2_SFLOAT = 103,

	GFX_DATAFORMAT_VEC3_UINT = 104,
	GFX_DATAFORMAT_VEC3_SINT = 105,
	GFX_DATAFORMAT_VEC3_SFLOAT = 106,

	GFX_DATAFORMAT_VEC4_UINT = 107,
	GFX_DATAFORMAT_VEC4_SINT = 108,
	GFX_DATAFORMAT_VEC4_SFLOAT = 109,
} GfxDataFormat;

typedef enum GfxPixelFormat {
	GFX_PIXELFORMAT_UNDEFINED = 0,

	GFX_PIXELFORMAT_RG4_UNORM_PACK8, GFX_PIXELFORMAT_FIRST = GFX_PIXELFORMAT_RG4_UNORM_PACK8,
	GFX_PIXELFORMAT_RGBA4_UNORM_PACK16,
	GFX_PIXELFORMAT_BGRA4_UNORM_PACK16,
	GFX_PIXELFORMAT_R5G6B5_UNORM_PACK16,
	GFX_PIXELFORMAT_B5G6R5_UNORM_PACK16,
	GFX_PIXELFORMAT_RGB5A1_UNORM_PACK16,
	GFX_PIXELFORMAT_BGR5A1_UNORM_PACK16,
	GFX_PIXELFORMAT_A1RGB5_UNORM_PACK16,

	GFX_PIXELFORMAT_R8_UNORM_PACK8,
	GFX_PIXELFORMAT_R8_SNORM_PACK8,
	GFX_PIXELFORMAT_R8_USCALED_PACK8,
	GFX_PIXELFORMAT_R8_SSCALED_PACK8,
	GFX_PIXELFORMAT_R8_UINT_PACK8,
	GFX_PIXELFORMAT_R8_SINT_PACK8,
	GFX_PIXELFORMAT_R8_SRGB_PACK8,

	GFX_PIXELFORMAT_RG8_UNORM_PACK8,
	GFX_PIXELFORMAT_RG8_SNORM_PACK8,
	GFX_PIXELFORMAT_RG8_USCALED_PACK8,
	GFX_PIXELFORMAT_RG8_SSCALED_PACK8,
	GFX_PIXELFORMAT_RG8_UINT_PACK8,
	GFX_PIXELFORMAT_RG8_SINT_PACK8,
	GFX_PIXELFORMAT_RG8_SRGB_PACK8,

	GFX_PIXELFORMAT_RGB8_UNORM_PACK8,
	GFX_PIXELFORMAT_RGB8_SNORM_PACK8,
	GFX_PIXELFORMAT_RGB8_USCALED_PACK8,
	GFX_PIXELFORMAT_RGB8_SSCALED_PACK8,
	GFX_PIXELFORMAT_RGB8_UINT_PACK8,
	GFX_PIXELFORMAT_RGB8_SINT_PACK8,
	GFX_PIXELFORMAT_RGB8_SRGB_PACK8,

	GFX_PIXELFORMAT_BGR8_UNORM_PACK8,
	GFX_PIXELFORMAT_BGR8_SNORM_PACK8,
	GFX_PIXELFORMAT_BGR8_USCALED_PACK8,
	GFX_PIXELFORMAT_BGR8_SSCALED_PACK8,
	GFX_PIXELFORMAT_BGR8_UINT_PACK8,
	GFX_PIXELFORMAT_BGR8_SINT_PACK8,
	GFX_PIXELFORMAT_BGR8_SRGB_PACK8,

	GFX_PIXELFORMAT_RGBA8_UNORM_PACK8,
	GFX_PIXELFORMAT_RGBA8_SNORM_PACK8,
	GFX_PIXELFORMAT_RGBA8_USCALED_PACK8,
	GFX_PIXELFORMAT_RGBA8_SSCALED_PACK8,
	GFX_PIXELFORMAT_RGBA8_UINT_PACK8,
	GFX_PIXELFORMAT_RGBA8_SINT_PACK8,
	GFX_PIXELFORMAT_RGBA8_SRGB_PACK8,

	GFX_PIXELFORMAT_BGRA8_UNORM_PACK8,
	GFX_PIXELFORMAT_BGRA8_SNORM_PACK8,
	GFX_PIXELFORMAT_BGRA8_USCALED_PACK8,
	GFX_PIXELFORMAT_BGRA8_SSCALED_PACK8,
	GFX_PIXELFORMAT_BGRA8_UINT_PACK8,
	GFX_PIXELFORMAT_BGRA8_SINT_PACK8,
	GFX_PIXELFORMAT_BGRA8_SRGB_PACK8,

	GFX_PIXELFORMAT_RGBA8_UNORM_PACK32,
	GFX_PIXELFORMAT_RGBA8_SNORM_PACK32,
	GFX_PIXELFORMAT_RGBA8_USCALED_PACK32,
	GFX_PIXELFORMAT_RGBA8_SSCALED_PACK32,
	GFX_PIXELFORMAT_RGBA8_UINT_PACK32,
	GFX_PIXELFORMAT_RGBA8_SINT_PACK32,
	GFX_PIXELFORMAT_RGBA8_SRGB_PACK32,

	GFX_PIXELFORMAT_RGB10A2_UNORM_PACK32,
	GFX_PIXELFORMAT_RGB10A2_SNORM_PACK32,
	GFX_PIXELFORMAT_RGB10A2_USCALED_PACK32,
	GFX_PIXELFORMAT_RGB10A2_SSCALED_PACK32,
	GFX_PIXELFORMAT_RGB10A2_UINT_PACK32,
	GFX_PIXELFORMAT_RGB10A2_SINT_PACK32,

	GFX_PIXELFORMAT_BGR10A2_UNORM_PACK32,
	GFX_PIXELFORMAT_BGR10A2_SNORM_PACK32,
	GFX_PIXELFORMAT_BGR10A2_USCALED_PACK32,
	GFX_PIXELFORMAT_BGR10A2_SSCALED_PACK32,
	GFX_PIXELFORMAT_BGR10A2_UINT_PACK32,
	GFX_PIXELFORMAT_BGR10A2_SINT_PACK32,

	GFX_PIXELFORMAT_R16_UNORM_PACK16,
	GFX_PIXELFORMAT_R16_SNORM_PACK16,
	GFX_PIXELFORMAT_R16_USCALED_PACK16,
	GFX_PIXELFORMAT_R16_SSCALED_PACK16,
	GFX_PIXELFORMAT_R16_UINT_PACK16,
	GFX_PIXELFORMAT_R16_SINT_PACK16,
	GFX_PIXELFORMAT_R16_SFLOAT_PACK16,

	GFX_PIXELFORMAT_RG16_UNORM_PACK16,
	GFX_PIXELFORMAT_RG16_SNORM_PACK16,
	GFX_PIXELFORMAT_RG16_USCALED_PACK16,
	GFX_PIXELFORMAT_RG16_SSCALED_PACK16,
	GFX_PIXELFORMAT_RG16_UINT_PACK16,
	GFX_PIXELFORMAT_RG16_SINT_PACK16,
	GFX_PIXELFORMAT_RG16_SFLOAT_PACK16,

	GFX_PIXELFORMAT_RGB16_UNORM_PACK16,
	GFX_PIXELFORMAT_RGB16_SNORM_PACK16,
	GFX_PIXELFORMAT_RGB16_USCALED_PACK16,
	GFX_PIXELFORMAT_RGB16_SSCALED_PACK16,
	GFX_PIXELFORMAT_RGB16_UINT_PACK16,
	GFX_PIXELFORMAT_RGB16_SINT_PACK16,
	GFX_PIXELFORMAT_RGB16_SFLOAT_PACK16,

	GFX_PIXELFORMAT_RGBA16_UNORM_PACK16,
	GFX_PIXELFORMAT_RGBA16_SNORM_PACK16,
	GFX_PIXELFORMAT_RGBA16_USCALED_PACK16,
	GFX_PIXELFORMAT_RGBA16_SSCALED_PACK16,
	GFX_PIXELFORMAT_RGBA16_UINT_PACK16,
	GFX_PIXELFORMAT_RGBA16_SINT_PACK16,
	GFX_PIXELFORMAT_RGBA16_SFLOAT_PACK16,

	GFX_PIXELFORMAT_R32_UINT_PACK32,
	GFX_PIXELFORMAT_R32_SINT_PACK32,
	GFX_PIXELFORMAT_R32_SFLOAT_PACK32,

	GFX_PIXELFORMAT_RG32_UINT_PACK32,
	GFX_PIXELFORMAT_RG32_SINT_PACK32,
	GFX_PIXELFORMAT_RG32_SFLOAT_PACK32,

	GFX_PIXELFORMAT_RGB32_UINT_PACK32,
	GFX_PIXELFORMAT_RGB32_SINT_PACK32,
	GFX_PIXELFORMAT_RGB32_SFLOAT_PACK32,

	GFX_PIXELFORMAT_RGBA32_UINT_PACK32,
	GFX_PIXELFORMAT_RGBA32_SINT_PACK32,
	GFX_PIXELFORMAT_RGBA32_SFLOAT_PACK32,

	GFX_PIXELFORMAT_R64_UINT_PACK64,
	GFX_PIXELFORMAT_R64_SINT_PACK64,
	GFX_PIXELFORMAT_R64_SFLOAT_PACK64,

	GFX_PIXELFORMAT_RG64_UINT_PACK64,
	GFX_PIXELFORMAT_RG64_SINT_PACK64,
	GFX_PIXELFORMAT_RG64_SFLOAT_PACK64,

	GFX_PIXELFORMAT_RGB64_UINT_PACK64,
	GFX_PIXELFORMAT_RGB64_SINT_PACK64,
	GFX_PIXELFORMAT_RGB64_SFLOAT_PACK64,

	GFX_PIXELFORMAT_RGBA64_UINT_PACK64,
	GFX_PIXELFORMAT_RGBA64_SINT_PACK64,
	GFX_PIXELFORMAT_RGBA64_SFLOAT_PACK64,

	GFX_PIXELFORMAT_RG11B10_UFLOAT_PACK32,
	GFX_PIXELFORMAT_RGB9E5_UFLOAT_PACK32,

	GFX_PIXELFORMAT_D16_UNORM_PACK16,
	GFX_PIXELFORMAT_D24_UNORM_PACK32,
	GFX_PIXELFORMAT_D32_SFLOAT_PACK32,
	GFX_PIXELFORMAT_S8_UINT_PACK8,
	GFX_PIXELFORMAT_D16_UNORM_S8_UINT_PACK32,
	GFX_PIXELFORMAT_D24_UNORM_S8_UINT_PACK32,
	GFX_PIXELFORMAT_D32_SFLOAT_S8_UINT_PACK64,

	GFX_PIXELFORMAT_RGB_DXT1_UNORM_BLOCK8,
	GFX_PIXELFORMAT_RGB_DXT1_SRGB_BLOCK8,
	GFX_PIXELFORMAT_RGBA_DXT1_UNORM_BLOCK8,
	GFX_PIXELFORMAT_RGBA_DXT1_SRGB_BLOCK8,
	GFX_PIXELFORMAT_RGBA_DXT3_UNORM_BLOCK16,
	GFX_PIXELFORMAT_RGBA_DXT3_SRGB_BLOCK16,
	GFX_PIXELFORMAT_RGBA_DXT5_UNORM_BLOCK16,
	GFX_PIXELFORMAT_RGBA_DXT5_SRGB_BLOCK16,
	GFX_PIXELFORMAT_R_ATI1N_UNORM_BLOCK8,
	GFX_PIXELFORMAT_R_ATI1N_SNORM_BLOCK8,
	GFX_PIXELFORMAT_RG_ATI2N_UNORM_BLOCK16,
	GFX_PIXELFORMAT_RG_ATI2N_SNORM_BLOCK16,
	GFX_PIXELFORMAT_RGB_BP_UFLOAT_BLOCK16,
	GFX_PIXELFORMAT_RGB_BP_SFLOAT_BLOCK16,
	GFX_PIXELFORMAT_RGBA_BP_UNORM_BLOCK16,
	GFX_PIXELFORMAT_RGBA_BP_SRGB_BLOCK16,

	GFX_PIXELFORMAT_RGB_ETC2_UNORM_BLOCK8,
	GFX_PIXELFORMAT_RGB_ETC2_SRGB_BLOCK8,
	GFX_PIXELFORMAT_RGBA_ETC2_UNORM_BLOCK8,
	GFX_PIXELFORMAT_RGBA_ETC2_SRGB_BLOCK8,
	GFX_PIXELFORMAT_RGBA_ETC2_UNORM_BLOCK16,
	GFX_PIXELFORMAT_RGBA_ETC2_SRGB_BLOCK16,
	GFX_PIXELFORMAT_R_EAC_UNORM_BLOCK8,
	GFX_PIXELFORMAT_R_EAC_SNORM_BLOCK8,
	GFX_PIXELFORMAT_RG_EAC_UNORM_BLOCK16,
	GFX_PIXELFORMAT_RG_EAC_SNORM_BLOCK16,

	GFX_PIXELFORMAT_RGBA_ASTC_4X4_UNORM_BLOCK16,
	GFX_PIXELFORMAT_RGBA_ASTC_4X4_SRGB_BLOCK16,
	GFX_PIXELFORMAT_RGBA_ASTC_5X4_UNORM_BLOCK16,
	GFX_PIXELFORMAT_RGBA_ASTC_5X4_SRGB_BLOCK16,
	GFX_PIXELFORMAT_RGBA_ASTC_5X5_UNORM_BLOCK16,
	GFX_PIXELFORMAT_RGBA_ASTC_5X5_SRGB_BLOCK16,
	GFX_PIXELFORMAT_RGBA_ASTC_6X5_UNORM_BLOCK16,
	GFX_PIXELFORMAT_RGBA_ASTC_6X5_SRGB_BLOCK16,
	GFX_PIXELFORMAT_RGBA_ASTC_6X6_UNORM_BLOCK16,
	GFX_PIXELFORMAT_RGBA_ASTC_6X6_SRGB_BLOCK16,
	GFX_PIXELFORMAT_RGBA_ASTC_8X5_UNORM_BLOCK16,
	GFX_PIXELFORMAT_RGBA_ASTC_8X5_SRGB_BLOCK16,
	GFX_PIXELFORMAT_RGBA_ASTC_8X6_UNORM_BLOCK16,
	GFX_PIXELFORMAT_RGBA_ASTC_8X6_SRGB_BLOCK16,
	GFX_PIXELFORMAT_RGBA_ASTC_8X8_UNORM_BLOCK16,
	GFX_PIXELFORMAT_RGBA_ASTC_8X8_SRGB_BLOCK16,
	GFX_PIXELFORMAT_RGBA_ASTC_10X5_UNORM_BLOCK16,
	GFX_PIXELFORMAT_RGBA_ASTC_10X5_SRGB_BLOCK16,
	GFX_PIXELFORMAT_RGBA_ASTC_10X6_UNORM_BLOCK16,
	GFX_PIXELFORMAT_RGBA_ASTC_10X6_SRGB_BLOCK16,
	GFX_PIXELFORMAT_RGBA_ASTC_10X8_UNORM_BLOCK16,
	GFX_PIXELFORMAT_RGBA_ASTC_10X8_SRGB_BLOCK16,
	GFX_PIXELFORMAT_RGBA_ASTC_10X10_UNORM_BLOCK16,
	GFX_PIXELFORMAT_RGBA_ASTC_10X10_SRGB_BLOCK16,
	GFX_PIXELFORMAT_RGBA_ASTC_12X10_UNORM_BLOCK16,
	GFX_PIXELFORMAT_RGBA_ASTC_12X10_SRGB_BLOCK16,
	GFX_PIXELFORMAT_RGBA_ASTC_12X12_UNORM_BLOCK16,
	GFX_PIXELFORMAT_RGBA_ASTC_12X12_SRGB_BLOCK16,

	GFX_PIXELFORMAT_RGB_PVRTC1_8X8_UNORM_BLOCK32,
	GFX_PIXELFORMAT_RGB_PVRTC1_8X8_SRGB_BLOCK32,
	GFX_PIXELFORMAT_RGB_PVRTC1_16X8_UNORM_BLOCK32,
	GFX_PIXELFORMAT_RGB_PVRTC1_16X8_SRGB_BLOCK32,
	GFX_PIXELFORMAT_RGBA_PVRTC1_8X8_UNORM_BLOCK32,
	GFX_PIXELFORMAT_RGBA_PVRTC1_8X8_SRGB_BLOCK32,
	GFX_PIXELFORMAT_RGBA_PVRTC1_16X8_UNORM_BLOCK32,
	GFX_PIXELFORMAT_RGBA_PVRTC1_16X8_SRGB_BLOCK32,
	GFX_PIXELFORMAT_RGBA_PVRTC2_4X4_UNORM_BLOCK8,
	GFX_PIXELFORMAT_RGBA_PVRTC2_4X4_SRGB_BLOCK8,
	GFX_PIXELFORMAT_RGBA_PVRTC2_8X4_UNORM_BLOCK8,
	GFX_PIXELFORMAT_RGBA_PVRTC2_8X4_SRGB_BLOCK8,

	GFX_PIXELFORMAT_RGB_ETC_UNORM_BLOCK8,
	GFX_PIXELFORMAT_RGB_ATC_UNORM_BLOCK8,
	GFX_PIXELFORMAT_RGBA_ATCA_UNORM_BLOCK16,
	GFX_PIXELFORMAT_RGBA_ATCI_UNORM_BLOCK16,

	GFX_PIXELFORMAT_L8_UNORM_PACK8,
	GFX_PIXELFORMAT_A8_UNORM_PACK8,
	GFX_PIXELFORMAT_LA8_UNORM_PACK8,
	GFX_PIXELFORMAT_L16_UNORM_PACK16,
	GFX_PIXELFORMAT_A16_UNORM_PACK16,
	GFX_PIXELFORMAT_LA16_UNORM_PACK16,

	GFX_PIXELFORMAT_BGR8_UNORM_PACK32,
	GFX_PIXELFORMAT_BGR8_SRGB_PACK32,

	GFX_PIXELFORMAT_RG3B2_UNORM_PACK8, GFX_PIXELFORMAT_LAST = GFX_PIXELFORMAT_RG3B2_UNORM_PACK8
} GfxPixelFormat;

typedef enum GfxTextureType {
	GFX_TEXTURE_INVALID_ENUM = -1,
	GFX_TEXTURE_2D,
	GFX_TEXTURE_2D_MULTISAMPLE,
	GFX_TEXTURE_2D_ARRAY,
	GFX_TEXTURE_CUBE_MAP,
} GfxTextureType;

typedef enum GfxDataType {
	GFX_DATA_INVALID_ENUM = -1,
	GFX_DATA_UNSIGNED_BYTE,
	GFX_DATA_UNSIGNED_SHORT,
	GFX_DATA_UNSIGNED_INT,
} GfxDataType;

typedef enum GfxIndexType {
	GFX_INDEX_INVALID_ENUM = -1,
	GFX_INDEX_UNSIGNED_SHORT,
	GFX_INDEX_UNSIGNED_INT,
} GfxIndexType;

typedef enum GfxPrimitiveTopology {
	GFX_PRIMITIVE_TOPOLOGY_INVALID_ENUM = -1,
	GFX_PRIMITIVE_TOPOLOGY_POINT_LIST,
	GFX_PRIMITIVE_TOPOLOGY_LINE_LIST,
	GFX_PRIMITIVE_TOPOLOGY_LINE_STRIP,
	GFX_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
	GFX_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP,
	GFX_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN,
} GfxPrimitiveTopology;

typedef enum GfxPolygonMode {
	GFX_POLYGON_MODE_INVALID_ENUM = -1,
	GFX_POLYGON_MODE_FILL,
	GFX_POLYGON_MODE_LINE,
	GFX_POLYGON_MODE_POINT,
} GfxPolygonMode;

typedef enum GfxCullFace {
	GFX_CULLFACE_INVALID_ENUM = -1,
	GFX_CULLFACE_FRONT,
	GFX_CULLFACE_BACK,
	GFX_CULLFACE_FRONT_AND_BACK,
} GfxCullFace;

typedef enum GfxFrontFace {
	GFX_FRONTFACE_INVALID_ENUM = -1,
	GFX_FRONTFACE_CW,
	GFX_FRONTFACE_CCW,
} GfxFrontFace;

typedef enum GfxCompareOp {
	GFX_COMPAREOP_INVALID_ENUM = -1,
	GFX_COMPAREOP_NEVER,
	GFX_COMPAREOP_LESS,
	GFX_COMPAREOP_LEQUAL,
	GFX_COMPAREOP_GREATER,
	GFX_COMPAREOP_GEQUAL,
	GFX_COMPAREOP_EQUAL,
	GFX_COMPAREOP_NOTEQUAL,
	GFX_COMPAREOP_ALWAYS,
} GfxCompareOp;

typedef enum GfxStencilOp {
	GFX_STENCILOP_INVALID_ENUM = -1,
	GFX_STENCILOP_KEEP,
	GFX_STENCILOP_ZERO,
	GFX_STENCILOP_REPLACE,
	GFX_STENCILOP_INCR,
	GFX_STENCILOP_INCR_WRAP,
	GFX_STENCILOP_DECR,
	GFX_STENCILOP_DECR_WRAP,
	GFX_STENCILOP_INVERT,
} GfxStencilOp;

typedef enum GfxFilter {
	GFX_FILTER_INVALID_ENUM = -1,
	GFX_FILTER_NEAREST,
	GFX_FILTER_LINEAR,
} GfxFilter;

typedef enum GfxSamplerMipmapMode {
	GFX_SAMPLER_MIPMAP_MODE_INVALID_ENUM = -1,
	GFX_SAMPLER_MIPMAP_MODE_NEAREST,
	GFX_SAMPLER_MIPMAP_MODE_LINEAR,
} GfxSamplerMipmapMode;

typedef enum GfxSamplerAddressMode {
	GFX_SAMPLER_ADDRESS_MODE_INVALID_ENUM = -1,
	GFX_SAMPLER_ADDRESS_MODE_REPEAT,
	GFX_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT,
	GFX_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
	GFX_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER,
	GFX_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE
} GfxSamplerAddressMode;

typedef enum GfxBlendFactor {
	GFX_BLENDFACTOR_INVALID_ENUM = -1,
	GFX_BLENDFACTOR_ZERO,
	GFX_BLENDFACTOR_ONE,
	GFX_BLENDFACTOR_SRC_COLOR,
	GFX_BLENDFACTOR_ONE_MINUS_SRC_COLOR,
	GFX_BLENDFACTOR_DST_COLOR,
	GFX_BLENDFACTOR_ONE_MINUS_DST_COLOR,
	GFX_BLENDFACTOR_SRC_ALPHA,
	GFX_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
	GFX_BLENDFACTOR_DST_ALPHA,
	GFX_BLENDFACTOR_ONE_MINUS_DST_ALPHA,
	GFX_BLENDFACTOR_CONSTANT_COLOR,
	GFX_BLENDFACTOR_ONE_MINUS_CONSTANT_COLOR,
	GFX_BLENDFACTOR_CONSTANT_ALPHA,
	GFX_BLENDFACTOR_ONE_MINUS_CONSTANT_ALPHA,
	GFX_BLENDFACTOR_SRC_ALPHA_SATURATE,
} GfxBlendFactor;

typedef enum GfxBlendOp {
	GFX_BLENDOP_INVALID_ENUM = -1,
	GFX_BLENDOP_ADD,
	GFX_BLENDOP_SUBTRACT,
	GFX_BLENDOP_REVERSE_SUBTRACT,
	GFX_BLENDOP_MIN,
	GFX_BLENDOP_MAX,
} GfxBlendOp;

typedef enum GfxCubemapFace {
	GFX_CUBEMAP_INVALID_ENUM = -1,
	GFX_CUBEMAP_POSITIVE_X,
	GFX_CUBEMAP_NEGATIVE_X,
	GFX_CUBEMAP_POSITIVE_Y,
	GFX_CUBEMAP_NEGATIVE_Y,
	GFX_CUBEMAP_POSITIVE_Z,
	GFX_CUBEMAP_NEGATIVE_Z,
} GfxCubemapFace;

typedef enum GfxImageLayout {
	GFX_IMAGE_LAYOUT_INVALID_ENUM = -1,
	GFX_IMAGE_LAYOUT_GENERAL,
	GFX_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
	GFX_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
	GFX_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
	GFX_IMAGE_LAYOUT_COLOR_READ_ONLY_OPTIMAL,
	GFX_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
	GFX_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL,
	GFX_IMAGE_LAYOUT_PRESENT_SRC_OPTIMAL,
} GfxImageLayout;

typedef enum GfxAccessFlagBits {
	GFX_ACCESS_INDIRECT_COMMAND_READ_BIT = 0x00000001,
	GFX_ACCESS_INDEX_READ_BIT = 0x00000002,
	GFX_ACCESS_VERTEX_ATTRIBUTE_READ_BIT = 0x00000004,
	GFX_ACCESS_UNIFORM_READ_BIT = 0x00000008,
	GFX_ACCESS_INPUT_ATTACHMENT_READ_BIT = 0x00000010,
	GFX_ACCESS_SHADER_READ_BIT = 0x00000020,
	GFX_ACCESS_SHADER_WRITE_BIT = 0x00000040,
	GFX_ACCESS_COLOR_ATTACHMENT_READ_BIT = 0x00000080,
	GFX_ACCESS_COLOR_ATTACHMENT_WRITE_BIT = 0x00000100,
	GFX_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT = 0x00000200,
	GFX_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT = 0x00000400,
	GFX_ACCESS_TRANSFER_READ_BIT = 0x00000800,
	GFX_ACCESS_TRANSFER_WRITE_BIT = 0x00001000,
	GFX_ACCESS_HOST_READ_BIT = 0x00002000,
	GFX_ACCESS_HOST_WRITE_BIT = 0x00004000,
	GFX_ACCESS_MEMORY_READ_BIT = 0x00008000,
	GFX_ACCESS_MEMORY_WRITE_BIT = 0x00010000,
	GFX_ACCESS_TRANSFORM_FEEDBACK_WRITE_BIT_EXT = 0x02000000,
	GFX_ACCESS_TRANSFORM_FEEDBACK_COUNTER_READ_BIT_EXT = 0x04000000,
	GFX_ACCESS_TRANSFORM_FEEDBACK_COUNTER_WRITE_BIT_EXT = 0x08000000,
	GFX_ACCESS_CONDITIONAL_RENDERING_READ_BIT_EXT = 0x00100000,
//	GFX_ACCESS_COMMAND_PROCESS_READ_BIT_NVX = 0x00020000,
//	GFX_ACCESS_COMMAND_PROCESS_WRITE_BIT_NVX = 0x00040000,
	GFX_ACCESS_COLOR_ATTACHMENT_READ_NONCOHERENT_BIT_EXT = 0x00080000,
//	GFX_ACCESS_SHADING_RATE_IMAGE_READ_BIT_NV = 0x00800000,
//	GFX_ACCESS_ACCELERATION_STRUCTURE_READ_BIT_NV = 0x00200000,
//	GFX_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_NV = 0x00400000,
	GFX_ACCESS_FRAGMENT_DENSITY_MAP_READ_BIT_EXT = 0x01000000,
	GFX_ACCESS_FLAG_BITS_MAX_ENUM = 0x7FFFFFFF
} GfxAccessFlagBits;
typedef uint32_t GfxAccessFlags;

typedef enum GfxPipelineStageFlagBits {
	GFX_PIPELINE_STAGE_TOP_OF_PIPE_BIT = 0x00000001,
	GFX_PIPELINE_STAGE_DRAW_INDIRECT_BIT = 0x00000002,
	GFX_PIPELINE_STAGE_VERTEX_INPUT_BIT = 0x00000004,
	GFX_PIPELINE_STAGE_VERTEX_SHADER_BIT = 0x00000008,
	GFX_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT = 0x00000010,
	GFX_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT = 0x00000020,
	GFX_PIPELINE_STAGE_GEOMETRY_SHADER_BIT = 0x00000040,
	GFX_PIPELINE_STAGE_FRAGMENT_SHADER_BIT = 0x00000080,
	GFX_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT = 0x00000100,
	GFX_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT = 0x00000200,
	GFX_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT = 0x00000400,
	GFX_PIPELINE_STAGE_COMPUTE_SHADER_BIT = 0x00000800,
	GFX_PIPELINE_STAGE_TRANSFER_BIT = 0x00001000,
	GFX_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT = 0x00002000,
	GFX_PIPELINE_STAGE_HOST_BIT = 0x00004000,
	GFX_PIPELINE_STAGE_ALL_GRAPHICS_BIT = 0x00008000,
	GFX_PIPELINE_STAGE_ALL_COMMANDS_BIT = 0x00010000,
	GFX_PIPELINE_STAGE_TRANSFORM_FEEDBACK_BIT_EXT = 0x01000000,
	GFX_PIPELINE_STAGE_CONDITIONAL_RENDERING_BIT_EXT = 0x00040000,
//	GFX_PIPELINE_STAGE_COMMAND_PROCESS_BIT_NVX = 0x00020000,
//	GFX_PIPELINE_STAGE_SHADING_RATE_IMAGE_BIT_NV = 0x00400000,
//	GFX_PIPELINE_STAGE_RAY_TRACING_SHADER_BIT_NV = 0x00200000,
//	GFX_PIPELINE_STAGE_ACCELERATION_STRUCTURE_BUILD_BIT_NV = 0x02000000,
//	GFX_PIPELINE_STAGE_TASK_SHADER_BIT_NV = 0x00080000,
//	GFX_PIPELINE_STAGE_MESH_SHADER_BIT_NV = 0x00100000,
	GFX_PIPELINE_STAGE_FRAGMENT_DENSITY_PROCESS_BIT_EXT = 0x00800000,
	GFX_PIPELINE_STAGE_FLAG_BITS_MAX_ENUM = 0x7FFFFFFF
} GfxPipelineStageFlagBits;
typedef uint32_t GfxPipelineStageFlags;
