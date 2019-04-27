#pragma once
#include "GLES3Renderer.h"


class CGLES3Helper
{
public:
	static void SetupExtensions(void);
	static bool IsSupportExtension(const char* extension);

	static GLenum TranslateDataType(GfxDataType dataType);
	static GLenum TranslateIndexType(GfxIndexType indexType);
	static GLenum TranslatePrimitiveTopology(GfxPrimitiveTopology topology);
	static GLenum TranslatePolytonMode(GfxPolygonMode polygonMode);
	static GLenum TranslateCullFace(GfxCullFace cullFace);
	static GLenum TranslateFrontFace(GfxFrontFace frontFace);
	static GLenum TranslateCompareOp(GfxCompareOp op);
	static GLenum TranslateStencilOp(GfxStencilOp op);
	static GLenum TranslateBlendOp(GfxBlendOp op);
	static GLenum TranslateBlendFactor(GfxBlendFactor blendFactor);
	static GLenum TranslateMagFilter(GfxFilter magFilter);
	static GLenum TranslateMinFilter(GfxFilter minFilter, GfxSamplerMipmapMode mipmapMode);
	static GLenum TranslateAddressMode(GfxSamplerAddressMode addressMode);
	static GLenum TranslateTextureTarget(GfxTextureType type);
	static GLenum TranslateTextureTarget(GfxCubemapFace face);


private:
	static eastl::unordered_map<uint32_t, eastl::string> extensions;
};
