#pragma once
#include "GLES3Renderer.h"


class CGLES3Helper
{
public:
	static void SetupExtensions(void);
	static bool IsSupportExtension(const char* extension);

	static GLenum TranslateDataType(GfxDataType type);
	static GLenum TranslateIndexType(GfxIndexType type);
	static GLenum TranslatePrimitiveTopology(GfxPrimitiveTopology topology);
	static GLenum TranslatePolytonMode(GfxPolygonMode mode);
	static GLenum TranslateCullFace(GfxCullFace face);
	static GLenum TranslateFrontFace(GfxFrontFace face);
	static GLenum TranslateCompareOp(GfxCompareOp op);
	static GLenum TranslateStencilOp(GfxStencilOp op);
	static GLenum TranslateBlendOp(GfxBlendOp op);
	static GLenum TranslateBlendFactor(GfxBlendFactor factor);
	static GLenum TranslateMagFilter(GfxFilter filter);
	static GLenum TranslateMinFilter(GfxFilter filter, GfxSamplerMipmapMode mode);
	static GLenum TranslateAddressMode(GfxSamplerAddressMode mode);
	static GLenum TranslateTextureTarget(GfxTextureType type);
	static GLenum TranslateTextureTarget(GfxCubemapFace face);
	static GLenum TranslateBarrier(GfxAccessFlags flags);


private:
	static eastl::unordered_map<uint32_t, eastl::string> extensions;
};
