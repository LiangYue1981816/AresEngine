#pragma once
#include "GfxRenderer.h"


class CALL_API CGfxMaterialPass
{
public:
	CGfxMaterialPass(uint32_t name)
	{

	}
	virtual ~CGfxMaterialPass(void)
	{

	}


public:
	virtual uint32_t GetName(void) const = 0;

public:
	virtual bool SetPipeline(const CGfxRenderPass *pRenderPass, const CGfxShader *pVertexShader, const CGfxShader *pFragmentShader, const PipelineState &state, uint32_t indexSubpass, uint32_t vertexBinding, uint32_t instanceBinding) = 0;

	virtual bool SetSampler(const char *szName, GfxFilter minFilter, GfxFilter magFilter, GfxSamplerMipmapMode mipmapMode, GfxSamplerAddressMode addressMode) = 0;
	virtual bool SetTexture2D(const char *szName, HANDLE hExternTexture) = 0;
	virtual bool SetTexture2DArray(const char *szName, HANDLE hExternTexture) = 0;
	virtual bool SetTextureCubeMap(const char *szName, HANDLE hExternTexture) = 0;
	virtual bool SetTexture2D(const char *szName, const CGfxTexture2DPtr &ptrTexture) = 0;
	virtual bool SetTexture2DArray(const char *szName, const CGfxTexture2DArrayPtr &ptrTexture) = 0;
	virtual bool SetTextureCubeMap(const char *szName, const CGfxTextureCubeMapPtr &ptrTexture) = 0;
	virtual bool SetTexture2D(const char *szName, const char *szFileName) = 0;
	virtual bool SetTexture2DArray(const char *szName, const char *szFileName) = 0;
	virtual bool SetTextureCubeMap(const char *szName, const char *szFileName) = 0;

	virtual bool SetUniformVec1(const char *szName, float v0) = 0;
	virtual bool SetUniformVec2(const char *szName, float v0, float v1) = 0;
	virtual bool SetUniformVec3(const char *szName, float v0, float v1, float v2) = 0;
	virtual bool SetUniformVec4(const char *szName, float v0, float v1, float v2, float v3) = 0;
	virtual bool SetUniformMat4(const char *szName, const float *value) = 0;

	virtual void Update(void) = 0;

public:
	virtual CGfxPipelineGraphics* GetPipeline(void) const = 0;

	virtual CGfxTexture2DPtr GetTexture2D(const char *szName) const = 0;
	virtual CGfxTexture2DArrayPtr GetTexture2DArray(const char *szName) const = 0;
	virtual CGfxTextureCubeMapPtr GetTextureCubeMap(const char *szName) const = 0;
};
