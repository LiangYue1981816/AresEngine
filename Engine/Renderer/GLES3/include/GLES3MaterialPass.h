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
	uint32_t GetName(void) const;

public:
	bool SetPipeline(const CGfxShader *pVertexShader, const CGfxShader *pFragmentShader, const PipelineState &state);
	bool SetSampler(const char *szName, int mipLevels, GfxFilter minFilter, GfxFilter magFilter, GfxSamplerMipmapMode mipmapMode, GfxSamplerAddressMode addressMode);
	bool SetTexture2D(const char *szName, HANDLE externTexture);
	bool SetTexture2DArray(const char *szName, HANDLE externTexture);
	bool SetTextureCubeMap(const char *szName, HANDLE externTexture);
	bool SetTexture2D(const char *szName, const char *szFileName);
	bool SetTexture2DArray(const char *szName, const char *szFileName);
	bool SetTextureCubeMap(const char *szName, const char *szFileName);
	bool SetUniformVec1(const char *szName, float v0);
	bool SetUniformVec2(const char *szName, float v0, float v1);
	bool SetUniformVec3(const char *szName, float v0, float v1, float v2);
	bool SetUniformVec4(const char *szName, float v0, float v1, float v2, float v3);
	bool SetUniformMat4(const char *szName, const float *value);

public:
	CGfxPipelineGraphics* GetPipeline(void) const;
	CGfxTexture2DPtr GetTexture2D(const char *szName) const;
	CGfxTexture2DArrayPtr GetTexture2DArray(const char *szName) const;
	CGfxTextureCubeMapPtr GetTextureCubeMap(const char *szName) const;

public:
	static void Bind(CGLES3Pipeline *pPipeline, const CGLES3MaterialPass *pPass, uint32_t &indexTexUnit);
	static void BindUniforms(CGLES3Pipeline *pPipeline, const CGLES3MaterialPass *pPass);
	static void BindTextures(CGLES3Pipeline *pPipeline, const CGLES3MaterialPass *pPass, uint32_t &indexTexUnit);


private:
	uint32_t m_name;

private:
	CGfxPipelineGraphics *m_pPipeline;

	eastl::unordered_map<uint32_t, CGfxSampler*> m_pSamplers;
	eastl::unordered_map<uint32_t, CGfxTexture2DPtr> m_ptrTexture2Ds;
	eastl::unordered_map<uint32_t, CGfxTexture2DArrayPtr> m_ptrTexture2DArrays;
	eastl::unordered_map<uint32_t, CGfxTextureCubeMapPtr> m_ptrTextureCubeMaps;

	eastl::unordered_map<uint32_t, CGfxUniformVec1*> m_pUniformVec1s;
	eastl::unordered_map<uint32_t, CGfxUniformVec2*> m_pUniformVec2s;
	eastl::unordered_map<uint32_t, CGfxUniformVec3*> m_pUniformVec3s;
	eastl::unordered_map<uint32_t, CGfxUniformVec4*> m_pUniformVec4s;
	eastl::unordered_map<uint32_t, CGfxUniformMat4*> m_pUniformMat4s;
};
