#include "GLES3Renderer.h"


GLenum CGLES3Helper::TranslateDataType(GfxDataType dataType)
{
	switch ((int)dataType) {
	case GFX_DATA_UNSIGNED_BYTE:
		return GL_UNSIGNED_BYTE;
	case GFX_DATA_UNSIGNED_SHORT:
		return GL_UNSIGNED_SHORT;
	case GFX_DATA_UNSIGNED_INT:
		return GL_UNSIGNED_INT;
	}

	return GL_INVALID_ENUM;
}

GLenum CGLES3Helper::TranslateIndexType(GfxIndexType indexType)
{
	switch ((int)indexType) {
	case GFX_INDEX_UNSIGNED_SHORT:
		return GL_UNSIGNED_SHORT;
	case GFX_INDEX_UNSIGNED_INT:
		return GL_UNSIGNED_INT;
	}

	return GL_INVALID_ENUM;
}

GLenum CGLES3Helper::TranslateCullFace(GfxCullFace cullFace)
{
	switch ((int)cullFace) {
	case GFX_CULLFACE_FRONT:
		return GL_FRONT;
	case GFX_CULLFACE_BACK:
		return GL_BACK;
	case GFX_CULLFACE_FRONT_AND_BACK:
		return GL_FRONT_AND_BACK;
	}

	return GL_INVALID_ENUM;
}

GLenum CGLES3Helper::TranslateFrontFace(GfxFrontFace frontFace)
{
	switch ((int)frontFace) {
	case GFX_FRONTFACE_CW:
		return GL_CW;
	case GFX_FRONTFACE_CCW:
		return GL_CCW;
	}

	return GL_INVALID_ENUM;
}

GLenum CGLES3Helper::TranslateFunc(GfxFunc func)
{
	switch ((int)func) {
	case GFX_FUNC_NEVER:
		return GL_NEVER;
	case GFX_FUNC_LESS:
		return GL_LESS;
	case GFX_FUNC_LEQUAL:
		return GL_LEQUAL;
	case GFX_FUNC_GREATER:
		return GL_GREATER;
	case GFX_FUNC_GEQUAL:
		return GL_GEQUAL;
	case GFX_FUNC_EQUAL:
		return GL_EQUAL;
	case GFX_FUNC_NOTEQUAL:
		return GL_NOTEQUAL;
	case GFX_FUNC_ALWAYS:
		return GL_ALWAYS;
	}

	return GL_INVALID_ENUM;
}

GLenum CGLES3Helper::TranslateStencilOp(GfxStencilOp stencilOp)
{
	switch ((int)stencilOp) {
	case GFX_STENCILOP_KEEP:
		return GL_KEEP;
	case GFX_STENCILOP_ZERO:
		return GL_ZERO;
	case GFX_STENCILOP_REPLACE:
		return GL_REPLACE;
	case GFX_STENCILOP_INCR:
		return GL_INCR;
	case GFX_STENCILOP_INCR_WRAP:
		return GL_INCR_WRAP;
	case GFX_STENCILOP_DECR:
		return GL_DECR;
	case GFX_STENCILOP_DECR_WRAP:
		return GL_DECR_WRAP;
	case GFX_STENCILOP_INVERT:
		return GL_INVERT;
	}

	return GL_INVALID_ENUM;
}

GLenum CGLES3Helper::TranslateMinFilter(GfxMinFilter minFilter)
{
	switch ((int)minFilter) {
	case GFX_MINFILTER_LINEAR:
		return GL_LINEAR;
	case GFX_MINFILTER_LINEAR_MIPMAP_LINEAR:
		return GL_LINEAR_MIPMAP_LINEAR;
	case GFX_MINFILTER_LINEAR_MIPMAP_NEAREST:
		return GL_LINEAR_MIPMAP_NEAREST;
	case GFX_MINFILTER_NEAREST:
		return GL_NEAREST;
	case GFX_MINFILTER_NEAREST_MIPMAP_LINEAR:
		return GL_NEAREST_MIPMAP_LINEAR;
	case GFX_MINFILTER_NEAREST_MIPMAP_NEAREST:
		return GL_NEAREST_MIPMAP_NEAREST;
	}

	return GL_INVALID_ENUM;
}

GLenum CGLES3Helper::TranslateMagFilter(GfxMagFilter magFilter)
{
	switch ((int)magFilter) {
	case GFX_MAGFILTER_LINEAR:
		return GL_LINEAR;
	case GFX_MAGFILTER_NEAREST:
		return GL_NEAREST;
	}

	return GL_INVALID_ENUM;
}

GLenum CGLES3Helper::TranslateAddressMode(GfxAddressMode addressMode)
{
	switch ((int)addressMode) {
	case GFX_ADDRESS_REPEAT:
		return GL_REPEAT;
	case GFX_ADDRESS_CLAMP_TO_EDGE:
		return GL_CLAMP_TO_EDGE;
	}

	return GL_INVALID_ENUM;
}

GLenum CGLES3Helper::TranslateBlendFactor(GfxBlendFactor blendFactor)
{
	switch ((int)blendFactor) {
	case GFX_BLENDFACTOR_ZERO:
		return GL_ZERO;
	case GFX_BLENDFACTOR_ONE:
		return GL_ONE;
	case GFX_BLENDFACTOR_SRC_COLOR:
		return GL_SRC_COLOR;
	case GFX_BLENDFACTOR_ONE_MINUS_SRC_COLOR:
		return GL_ONE_MINUS_SRC_COLOR;
	case GFX_BLENDFACTOR_DST_COLOR:
		return GL_DST_COLOR;
	case GFX_BLENDFACTOR_ONE_MINUS_DST_COLOR:
		return GL_ONE_MINUS_DST_COLOR;
	case GFX_BLENDFACTOR_SRC_ALPHA:
		return GL_SRC_ALPHA;
	case GFX_BLENDFACTOR_ONE_MINUS_SRC_ALPHA:
		return GL_ONE_MINUS_SRC_ALPHA;
	case GFX_BLENDFACTOR_DST_ALPHA:
		return GL_DST_ALPHA;
	case GFX_BLENDFACTOR_ONE_MINUS_DST_ALPHA:
		return GL_ONE_MINUS_DST_ALPHA;
	case GFX_BLENDFACTOR_CONSTANT_COLOR:
		return GL_CONSTANT_COLOR;
	case GFX_BLENDFACTOR_ONE_MINUS_CONSTANT_COLOR:
		return GL_ONE_MINUS_CONSTANT_COLOR;
	case GFX_BLENDFACTOR_CONSTANT_ALPHA:
		return GL_CONSTANT_ALPHA;
	case GFX_BLENDFACTOR_ONE_MINUS_CONSTANT_ALPHA:
		return GL_ONE_MINUS_CONSTANT_ALPHA;
	case GFX_BLENDFACTOR_SRC_ALPHA_SATURATE:
		return GL_SRC_ALPHA_SATURATE;
	}

	return GL_INVALID_ENUM;
}

GLenum CGLES3Helper::TranslateBlendEquation(GfxBlendEquation blendEquation)
{
	switch ((int)blendEquation) {
	case GFX_BLENDEQUATION_FUNC_ADD:
		return GL_FUNC_ADD;
	case GFX_BLENDEQUATION_FUNC_SUBTRACT:
		return GL_FUNC_SUBTRACT;
	case GFX_BLENDEQUATION_FUNC_REVERSE_SUBTRACT:
		return GL_FUNC_REVERSE_SUBTRACT;
	case GFX_BLENDEQUATION_MIN:
		return GL_MIN;
	case GFX_BLENDEQUATION_MAX:
		return GL_MAX;
	}

	return GL_INVALID_ENUM;
}

GLenum CGLES3Helper::TranslateTextureTarget(GfxTextureType type)
{
	switch ((int)type) {
	case GFX_TEXTURE_2D:
		return GL_TEXTURE_2D;
	case GFX_TEXTURE_2D_MULTISAMPLE:
		return GL_TEXTURE_2D_MULTISAMPLE;
	case GFX_TEXTURE_2D_ARRAY:
		return GL_TEXTURE_2D_ARRAY;
	case GFX_TEXTURE_CUBE_MAP:
		return GL_TEXTURE_CUBE_MAP;
	}

	return GL_INVALID_ENUM;
}

GLenum CGLES3Helper::TranslateTextureCubeMapFace(GfxTextureCubeMapFace face)
{
	switch ((int)face) {
	case GFX_TEXTURE_CUBEMAP_POSITIVE_X:
		return GL_TEXTURE_CUBE_MAP_POSITIVE_X;
	case GFX_TEXTURE_CUBEMAP_NEGATIVE_X:
		return GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
	case GFX_TEXTURE_CUBEMAP_POSITIVE_Y:
		return GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
	case GFX_TEXTURE_CUBEMAP_NEGATIVE_Y:
		return GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
	case GFX_TEXTURE_CUBEMAP_POSITIVE_Z:
		return GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
	case GFX_TEXTURE_CUBEMAP_NEGATIVE_Z:
		return GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
	}

	return GL_INVALID_ENUM;
}

GLenum CGLES3Helper::TranslateDrawMode(GfxDrawMode drawMode)
{
	switch ((int)drawMode) {
	case GFX_DRAWMODE_POINTS:
		return GL_POINTS;
	case GFX_DRAWMODE_LINE_STRIP:
		return GL_LINE_STRIP;
	case GFX_DRAWMODE_LINE_LOOP:
		return GL_LINE_LOOP;
	case GFX_DRAWMODE_LINES:
		return GL_LINES;
	case GFX_DRAWMODE_TRIANGLE_STRIP:
		return GL_TRIANGLE_STRIP;
	case GFX_DRAWMODE_TRIANGLE_FAN:
		return GL_TRIANGLE_FAN;
	case GFX_DRAWMODE_TRIANGLES:
		return GL_TRIANGLES;
	}

	return GL_INVALID_ENUM;
}
