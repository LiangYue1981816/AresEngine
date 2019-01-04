#pragma once
#include "GLES3Renderer.h"


class CGLES3Helper
{
public:
	static GLenum TranslateDataType(GfxDataType dataType);
	static GLenum TranslateIndexType(GfxIndexType indexType);
	static GLenum TranslateCullFace(GfxCullFace cullFace);
	static GLenum TranslateFrontFace(GfxFrontFace frontFace);
	static GLenum TranslateFunc(GfxFunc func);
	static GLenum TranslateStencilOp(GfxStencilOp stencilOp);
	static GLenum TranslateMinFilter(GfxMinFilter minFilter);
	static GLenum TranslateMagFilter(GfxMagFilter magFilter);
	static GLenum TranslateAddressMode(GfxAddressMode addressMode);
	static GLenum TranslateBlendFactor(GfxBlendFactor blendFactor);
	static GLenum TranslateBlendEquation(GfxBlendEquation blendEquation);
	static GLenum TranslateTextureTarget(GfxTextureType type);
	static GLenum TranslateTextureCubeMapFace(GfxTextureCubeMapFace face);
	static GLenum TranslateMagFilter(GfxFilter magFilter);
	static GLenum TranslateMinFilter(GfxFilter minFilter, GfxMipmapMode mipmapMode);
	static GLenum TranslateDrawMode(GfxDrawMode drawMode);
};
