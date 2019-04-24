#pragma once
#include "GfxRenderer.h"


class CALL_API CGfxMaterialPass
{
public:
	CGfxMaterialPass(uint32_t name)
		: m_name(name)
	{

	}
	virtual ~CGfxMaterialPass(void)
	{

	}

	uint32_t GetName(void) const
	{
		return m_name;
	}


public:
	virtual const CGfxDescriptorSetPtr GetDescriptorSet(void) const = 0;

public:
	virtual bool SetPipeline(const CGfxRenderPass* pRenderPass, const CGfxShader* pVertexShader, const CGfxShader* pFragmentShader, const PipelineState& state, int indexSubpass, uint32_t vertexBinding, uint32_t instanceBinding) = 0;
	virtual const CGfxPipelineGraphics* GetPipeline(void) const = 0;

public:
	virtual bool SetSampler(uint32_t name, GfxFilter minFilter, GfxFilter magFilter, GfxSamplerMipmapMode mipmapMode, GfxSamplerAddressMode addressMode) = 0;
	virtual bool SetTexture2D(uint32_t name, const CGfxTexture2DPtr ptrTexture) = 0;
	virtual bool SetTexture2DArray(uint32_t name, const CGfxTexture2DArrayPtr ptrTexture) = 0;
	virtual bool SetTextureCubeMap(uint32_t name, const CGfxTextureCubeMapPtr ptrTexture) = 0;
	virtual bool SetTexture2D(uint32_t name, const char* szFileName, int baseLevel = 0) = 0;
	virtual bool SetTexture2DArray(uint32_t name, const char* szFileName, int baseLevel = 0) = 0;
	virtual bool SetTextureCubeMap(uint32_t name, const char* szFileName, int baseLevel = 0) = 0;

	virtual bool SetUniformVec1(uint32_t name, float v0) = 0;
	virtual bool SetUniformVec2(uint32_t name, float v0, float v1) = 0;
	virtual bool SetUniformVec3(uint32_t name, float v0, float v1, float v2) = 0;
	virtual bool SetUniformVec4(uint32_t name, float v0, float v1, float v2, float v3) = 0;
	virtual bool SetUniformMat4(uint32_t name, const float* value) = 0;


private:
	uint32_t m_name;
};
