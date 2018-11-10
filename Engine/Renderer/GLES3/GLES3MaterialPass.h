#pragma once
#include <tinyxml.h>
#include <tinystr.h>
#include "GLES3Renderer.h"


class CGLES3MaterialPass : public CGfxMaterialPass
{
	friend class CGLES3Renderer;
	friend class CGLES3Material;


private:
	CGLES3MaterialPass(uint32_t name);
	virtual ~CGLES3MaterialPass(void);

	
private:
	bool Load(TiXmlNode *pPassNode);
	bool LoadPipeline(TiXmlNode *pPassNode);
	bool LoadPipelineState(TiXmlNode *pPipelineNode, PipelineState &state);
	bool LoadPipelineShader(TiXmlNode *pPipelineNode, CGfxShader *&pShader, shaderc_shader_kind kind);
	bool LoadTexture2D(TiXmlNode *pPassNode);
	bool LoadTexture2DArray(TiXmlNode *pPassNode);
	bool LoadTextureCubeMap(TiXmlNode *pPassNode);
	bool LoadUniformVec1(TiXmlNode *pPassNode);
	bool LoadUniformVec2(TiXmlNode *pPassNode);
	bool LoadUniformVec3(TiXmlNode *pPassNode);
	bool LoadUniformVec4(TiXmlNode *pPassNode);

public:
	bool SetPipeline(const CGfxShader *pVertexShader, const CGfxShader *pFragmentShader, const PipelineState &state);

	bool SetSampler(const char *szName, uint32_t minFilter, uint32_t magFilter, uint32_t addressMode);
	bool SetTexture2D(const char *szName, uint32_t externTexture);
	bool SetTexture2DArray(const char *szName, uint32_t externTexture);
	bool SetTextureCubeMap(const char *szName, uint32_t externTexture);
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

public:
	static void Bind(CGLES3PipelineBase *pPipeline, const CGLES3MaterialPass *pPass, uint32_t &indexTexUnit);
	static void BindUniforms(CGLES3PipelineBase *pPipeline, const CGLES3MaterialPass *pPass);
	static void BindTextures(CGLES3PipelineBase *pPipeline, const CGLES3MaterialPass *pPass, uint32_t &indexTexUnit);


private:
	CGLES3PipelineGraphics *m_pPipeline;

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
