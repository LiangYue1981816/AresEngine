#include "GLES3Renderer.h"


eastl::unordered_map<uint32_t, eastl::string> CGLES3Helper::extensions;

void CGLES3Helper::SetupExtensions(void)
{
	int numExtensions;
	glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);

	for (int indexExtension = 0; indexExtension < numExtensions; indexExtension++) {
		const char* extension = (const char*)glGetStringi(GL_EXTENSIONS, indexExtension);
		extensions[HashValue(extension)] = extension;
	}
}

bool CGLES3Helper::IsSupportExtension(const char* extension)
{
	return extensions.find(HashValue(extension)) != extensions.end();
}

GLenum CGLES3Helper::TranslateDataType(GfxDataType type)
{
	switch ((int)type) {
	case GFX_DATA_UNSIGNED_BYTE:
		return GL_UNSIGNED_BYTE;

	case GFX_DATA_UNSIGNED_SHORT:
		return GL_UNSIGNED_SHORT;

	case GFX_DATA_UNSIGNED_INT:
		return GL_UNSIGNED_INT;

	default:
		return GL_INVALID_ENUM;
	}
}

GLenum CGLES3Helper::TranslateIndexType(GfxIndexType type)
{
	switch ((int)type) {
	case GFX_INDEX_UNSIGNED_SHORT:
		return GL_UNSIGNED_SHORT;

	case GFX_INDEX_UNSIGNED_INT:
		return GL_UNSIGNED_INT;

	default:
		return GL_INVALID_ENUM;
	}
}

GLenum CGLES3Helper::TranslatePrimitiveTopology(GfxPrimitiveTopology topology)
{
	switch ((int)topology) {
	case GFX_PRIMITIVE_TOPOLOGY_POINT_LIST:
		return GL_POINTS;

	case GFX_PRIMITIVE_TOPOLOGY_LINE_LIST:
		return GL_LINES;

	case GFX_PRIMITIVE_TOPOLOGY_LINE_STRIP:
		return GL_LINE_STRIP;

	case GFX_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST:
		return GL_TRIANGLES;

	case GFX_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP:
		return GL_TRIANGLE_STRIP;

	case GFX_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN:
		return GL_TRIANGLE_FAN;

	default:
		return GL_TRIANGLES;
	}
}

GLenum CGLES3Helper::TranslatePolytonMode(GfxPolygonMode mode)
{
	switch ((int)mode) {
	case GFX_POLYGON_MODE_FILL:
		return GL_FILL;

	case GFX_POLYGON_MODE_LINE:
		return GL_LINE;

	case GFX_POLYGON_MODE_POINT:
		return GL_POINT;

	default:
		return GL_FILL;
	}
}

GLenum CGLES3Helper::TranslateCullFace(GfxCullFace face)
{
	switch ((int)face) {
	case GFX_CULLFACE_FRONT:
		return GL_FRONT;

	case GFX_CULLFACE_BACK:
		return GL_BACK;

	case GFX_CULLFACE_FRONT_AND_BACK:
		return GL_FRONT_AND_BACK;

	default:
		return GL_BACK;
	}
}

GLenum CGLES3Helper::TranslateFrontFace(GfxFrontFace face)
{
	switch ((int)face) {
	case GFX_FRONTFACE_CW:
		return GL_CW;

	case GFX_FRONTFACE_CCW:
		return GL_CCW;

	default:
		return GL_CCW;
	}
}

GLenum CGLES3Helper::TranslateCompareOp(GfxCompareOp op)
{
	switch ((int)op) {
	case GFX_COMPAREOP_NEVER:
		return GL_NEVER;

	case GFX_COMPAREOP_LESS:
		return GL_LESS;

	case GFX_COMPAREOP_LEQUAL:
		return GL_LEQUAL;

	case GFX_COMPAREOP_GREATER:
		return GL_GREATER;

	case GFX_COMPAREOP_GEQUAL:
		return GL_GEQUAL;

	case GFX_COMPAREOP_EQUAL:
		return GL_EQUAL;

	case GFX_COMPAREOP_NOTEQUAL:
		return GL_NOTEQUAL;

	case GFX_COMPAREOP_ALWAYS:
		return GL_ALWAYS;

	default:
		return GL_INVALID_ENUM;
	}
}

GLenum CGLES3Helper::TranslateStencilOp(GfxStencilOp op)
{
	switch ((int)op) {
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

	default:
		return GL_INVALID_ENUM;
	}
}

GLenum CGLES3Helper::TranslateBlendOp(GfxBlendOp op)
{
	switch ((int)op) {
	case GFX_BLENDOP_ADD:
		return GL_FUNC_ADD;

	case GFX_BLENDOP_SUBTRACT:
		return GL_FUNC_SUBTRACT;

	case GFX_BLENDOP_REVERSE_SUBTRACT:
		return GL_FUNC_REVERSE_SUBTRACT;

	case GFX_BLENDOP_MIN:
		return GL_MIN;

	case GFX_BLENDOP_MAX:
		return GL_MAX;

	default:
		return GL_INVALID_ENUM;
	}
}

GLenum CGLES3Helper::TranslateBlendFactor(GfxBlendFactor factor)
{
	switch ((int)factor) {
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

	default:
		return GL_INVALID_ENUM;
	}
}

GLenum CGLES3Helper::TranslateMagFilter(GfxFilter filter)
{
	switch ((int)filter) {
	case GFX_FILTER_NEAREST:
		return GL_NEAREST;

	case GFX_FILTER_LINEAR:
		return GL_LINEAR;

	default:
		return GL_LINEAR;
	}
}

GLenum CGLES3Helper::TranslateMinFilter(GfxFilter filter, GfxSamplerMipmapMode mode)
{
	switch ((int)filter) {
	case GFX_FILTER_NEAREST:
		switch ((int)mode) {
		case GFX_SAMPLER_MIPMAP_MODE_NEAREST:
			return GL_NEAREST_MIPMAP_NEAREST;

		case GFX_SAMPLER_MIPMAP_MODE_LINEAR:
			return GL_NEAREST_MIPMAP_LINEAR;

		default:
			return GL_NEAREST_MIPMAP_LINEAR;
		}
		break;
	case GFX_FILTER_LINEAR:
		switch ((int)mode) {
		case GFX_SAMPLER_MIPMAP_MODE_NEAREST:
			return GL_LINEAR_MIPMAP_NEAREST;

		case GFX_SAMPLER_MIPMAP_MODE_LINEAR:
			return GL_LINEAR_MIPMAP_LINEAR;

		default:
			return GL_NEAREST_MIPMAP_LINEAR;
		}
		break;

	default:
		return GL_NEAREST_MIPMAP_LINEAR;
	}
}

GLenum CGLES3Helper::TranslateAddressMode(GfxSamplerAddressMode mode)
{
	switch ((int)mode) {
	case GFX_SAMPLER_ADDRESS_MODE_REPEAT:
		return GL_REPEAT;

	case GFX_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE:
		return GL_CLAMP_TO_EDGE;

	default:
		return GL_CLAMP_TO_EDGE;
	}
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

	default:
		return GL_INVALID_ENUM;
	}
}

GLenum CGLES3Helper::TranslateTextureTarget(GfxCubemapFace face)
{
	switch ((int)face) {
	case GFX_CUBEMAP_POSITIVE_X:
		return GL_TEXTURE_CUBE_MAP_POSITIVE_X;

	case GFX_CUBEMAP_NEGATIVE_X:
		return GL_TEXTURE_CUBE_MAP_NEGATIVE_X;

	case GFX_CUBEMAP_POSITIVE_Y:
		return GL_TEXTURE_CUBE_MAP_POSITIVE_Y;

	case GFX_CUBEMAP_NEGATIVE_Y:
		return GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;

	case GFX_CUBEMAP_POSITIVE_Z:
		return GL_TEXTURE_CUBE_MAP_POSITIVE_Z;

	case GFX_CUBEMAP_NEGATIVE_Z:
		return GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;

	default:
		return GL_INVALID_ENUM;
	}
}

GLenum CGLES3Helper::TranslateBarrier(GfxAccessFlags flags)
{
	GLenum barrier = 0;

	if (flags & GFX_ACCESS_VERTEX_ATTRIBUTE_READ_BIT) {
		barrier |= GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT;
	}

	if (flags & GFX_ACCESS_INDEX_READ_BIT) {
		barrier |= GL_ELEMENT_ARRAY_BARRIER_BIT;
	}

	if (flags & GFX_ACCESS_INDIRECT_COMMAND_READ_BIT) {
		barrier |= GL_COMMAND_BARRIER_BIT;
	}

	if (flags & GFX_ACCESS_UNIFORM_READ_BIT) {
		barrier |= GL_UNIFORM_BARRIER_BIT;
	}

	if (flags & (GFX_ACCESS_SHADER_READ_BIT | GFX_ACCESS_SHADER_WRITE_BIT)) {
		barrier |= GL_TEXTURE_FETCH_BARRIER_BIT | GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT;
	}

	if (flags & (GFX_ACCESS_TRANSFER_READ_BIT | GFX_ACCESS_TRANSFER_WRITE_BIT)) {
		barrier |= GL_PIXEL_BUFFER_BARRIER_BIT | GL_TEXTURE_UPDATE_BARRIER_BIT | GL_BUFFER_UPDATE_BARRIER_BIT;
	}

	if (flags & (GFX_ACCESS_COLOR_ATTACHMENT_READ_BIT | GFX_ACCESS_COLOR_ATTACHMENT_WRITE_BIT)) {
		barrier |= GL_FRAMEBUFFER_BARRIER_BIT;
	}

	if (flags & GFX_ACCESS_TRANSFORM_FEEDBACK_WRITE_BIT_EXT) {
		barrier |= GL_TRANSFORM_FEEDBACK_BARRIER_BIT;
	}

	return barrier ? barrier : GL_ALL_BARRIER_BITS;
}
