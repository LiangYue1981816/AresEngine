#pragma once
#include "GLES3Renderer.h"


class CGLES3Helper
{
public:
	static void SetupExtensions(void);
	static bool IsSupportExtension(const char *extension);

	static GLenum TranslateDataType(GfxDataType dataType);
	static GLenum TranslateIndexType(GfxIndexType indexType);
	static GLenum TranslateCullFace(GfxCullFace cullFace);
	static GLenum TranslateFrontFace(GfxFrontFace frontFace);
	static GLenum TranslateFunc(GfxFunc func);
	static GLenum TranslateStencilOp(GfxStencilOp stencilOp);
	static GLenum TranslateBlendFactor(GfxBlendFactor blendFactor);
	static GLenum TranslateBlendEquation(GfxBlendEquation blendEquation);
	static GLenum TranslateMagFilter(GfxFilter magFilter);
	static GLenum TranslateMinFilter(GfxFilter minFilter, GfxSamplerMipmapMode mipmapMode);
	static GLenum TranslateAddressMode(GfxSamplerAddressMode addressMode);
	static GLenum TranslateTextureTarget(GfxTextureType type);
	static GLenum TranslateTextureCubeMapFace(GfxTextureCubeMapFace face);
	static GLenum TranslateDrawMode(GfxDrawMode drawMode);


private:
	static eastl::unordered_map<uint32_t, eastl::string> extensions;
};
