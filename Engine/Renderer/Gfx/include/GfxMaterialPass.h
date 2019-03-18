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
	virtual CGfxPipelineGraphics* GetPipeline(void) const = 0;
	virtual CGfxDescriptorSetPtr GetDescriptorSet(void) const = 0;

public:
	virtual bool SetPipeline(const CGfxRenderPass *pRenderPass, const CGfxShader *pVertexShader, const CGfxShader *pFragmentShader, const PipelineState &state, uint32_t indexSubpass, uint32_t vertexBinding, uint32_t instanceBinding) = 0;

	virtual bool SetSampler(uint32_t name, GfxFilter minFilter, GfxFilter magFilter, GfxSamplerMipmapMode mipmapMode, GfxSamplerAddressMode addressMode) = 0;
	virtual bool SetTexture2D(uint32_t name, const CGfxTexture2DPtr ptrTexture) = 0;
	virtual bool SetTexture2DArray(uint32_t name, const CGfxTexture2DArrayPtr ptrTexture) = 0;
	virtual bool SetTextureCubeMap(uint32_t name, const CGfxTextureCubeMapPtr ptrTexture) = 0;
	virtual bool SetTexture2D(uint32_t name, const char *szFileName) = 0;
	virtual bool SetTexture2DArray(uint32_t name, const char *szFileName) = 0;
	virtual bool SetTextureCubeMap(uint32_t name, const char *szFileName) = 0;

	virtual bool SetUniformVec1(uint32_t name, float v0) = 0;
	virtual bool SetUniformVec2(uint32_t name, float v0, float v1) = 0;
	virtual bool SetUniformVec3(uint32_t name, float v0, float v1, float v2) = 0;
	virtual bool SetUniformVec4(uint32_t name, float v0, float v1, float v2, float v3) = 0;
	virtual bool SetUniformMat4(uint32_t name, const float *values) = 0;

public:
	virtual CGfxTexture2DPtr GetTexture2D(uint32_t name) const = 0;
	virtual CGfxTexture2DArrayPtr GetTexture2DArray(uint32_t name) const = 0;
	virtual CGfxTextureCubeMapPtr GetTextureCubeMap(uint32_t name) const = 0;
};
