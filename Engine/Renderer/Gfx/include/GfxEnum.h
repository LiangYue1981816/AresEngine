#pragma once


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

typedef enum GfxFunc {
	GFX_FUNC_INVALID_ENUM = -1,
	GFX_FUNC_NEVER,
	GFX_FUNC_LESS,
	GFX_FUNC_LEQUAL,
	GFX_FUNC_GREATER,
	GFX_FUNC_GEQUAL,
	GFX_FUNC_EQUAL,
	GFX_FUNC_NOTEQUAL,
	GFX_FUNC_ALWAYS,
} GfxFunc;

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

typedef enum GfxBlendEquation {
	GFX_BLENDEQUATION_INVALID_ENUM = -1,
	GFX_BLENDEQUATION_FUNC_ADD,
	GFX_BLENDEQUATION_FUNC_SUBTRACT,
	GFX_BLENDEQUATION_FUNC_REVERSE_SUBTRACT,
	GFX_BLENDEQUATION_MIN,
	GFX_BLENDEQUATION_MAX,
} GfxBlendEquation;

typedef enum GfxTextureCubeMapFace {
	GFX_TEXTURE_CUBEMAP_INVALID_ENUM = -1,
	GFX_TEXTURE_CUBEMAP_POSITIVE_X,
	GFX_TEXTURE_CUBEMAP_NEGATIVE_X,
	GFX_TEXTURE_CUBEMAP_POSITIVE_Y,
	GFX_TEXTURE_CUBEMAP_NEGATIVE_Y,
	GFX_TEXTURE_CUBEMAP_POSITIVE_Z,
	GFX_TEXTURE_CUBEMAP_NEGATIVE_Z,
} GfxTextureCubeMapFace;

typedef enum GfxDrawMode {
	GFX_DRAWMODE_INVALID_ENUM = -1,
	GFX_DRAWMODE_POINTS,
	GFX_DRAWMODE_LINE_STRIP,
	GFX_DRAWMODE_LINE_LOOP,
	GFX_DRAWMODE_LINES,
	GFX_DRAWMODE_TRIANGLE_STRIP,
	GFX_DRAWMODE_TRIANGLE_FAN,
	GFX_DRAWMODE_TRIANGLES,
} GfxDrawMode;
