#pragma once
#include "GLES3Renderer.h"


class CGLES3MaterialPass : public CGfxMaterialPass
{
	friend class CGLES3Renderer;
	friend class CGLES3Material;


private:
	CGLES3MaterialPass(uint32_t name);
	virtual ~CGLES3MaterialPass(void);


public:
	const CGfxDescriptorSetPtr GetDescriptorSet(void) const;

public:
	bool SetPipeline(const CGfxRenderPass* pRenderPass, const CGfxShader* pVertexShader, const CGfxShader* pFragmentShader, const PipelineState& state, uint32_t indexSubpass, uint32_t vertexBinding, uint32_t instanceBinding);
	const CGfxPipelineGraphics* GetPipeline(void) const;

public:
	bool SetSampler(uint32_t name, GfxFilter minFilter, GfxFilter magFilter, GfxSamplerMipmapMode mipmapMode, GfxSamplerAddressMode addressMode);
	bool SetTexture2D(uint32_t name, const CGfxTexture2DPtr ptrTexture);
	bool SetTexture2DArray(uint32_t name, const CGfxTexture2DArrayPtr ptrTexture);
	bool SetTextureCubeMap(uint32_t name, const CGfxTextureCubeMapPtr ptrTexture);
	bool SetTexture2D(uint32_t name, const char* szFileName, int baseLevel = 0);
	bool SetTexture2DArray(uint32_t name, const char* szFileName, int baseLevel = 0);
	bool SetTextureCubeMap(uint32_t name, const char* szFileName, int baseLevel = 0);

	bool SetUniformVec1(uint32_t name, float v0);
	bool SetUniformVec2(uint32_t name, float v0, float v1);
	bool SetUniformVec3(uint32_t name, float v0, float v1, float v2);
	bool SetUniformVec4(uint32_t name, float v0, float v1, float v2, float v3);
	bool SetUniformMat4(uint32_t name, const float* value);

public:
	void Bind(const CGLES3PipelineGraphics* pPipeline) const;


private:
	CGfxPipelineGraphics* m_pPipeline;
	CGfxDescriptorSetPtr m_ptrDescriptorSet;

private:
	eastl::unordered_map<uint32_t, CGfxSampler*> m_pSamplers;
	eastl::unordered_map<uint32_t, CGfxUniformVec1*> m_pUniformVec1s;
	eastl::unordered_map<uint32_t, CGfxUniformVec2*> m_pUniformVec2s;
	eastl::unordered_map<uint32_t, CGfxUniformVec3*> m_pUniformVec3s;
	eastl::unordered_map<uint32_t, CGfxUniformVec4*> m_pUniformVec4s;
	eastl::unordered_map<uint32_t, CGfxUniformMat4*> m_pUniformMat4s;
};
