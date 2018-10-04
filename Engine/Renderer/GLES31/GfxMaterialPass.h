#pragma once
#include <tinyxml.h>
#include <tinystr.h>
#include "GfxRenderer.h"


class CGfxMaterialPass
{
	friend class CGfxRenderer;
	friend class CGfxMaterial;


private:
	CGfxMaterialPass(uint32_t name);
	virtual ~CGfxMaterialPass(void);

public:
	uint32_t GetName(void) const;

	
private:
	bool Load(TiXmlNode *pPassNode);
	bool LoadPipeline(TiXmlNode *pPassNode);
	bool LoadPipelineState(TiXmlNode *pPipelineNode, GLstate &state);
	bool LoadPipelineShader(TiXmlNode *pPipelineNode, CGfxShader *&pShader, shaderc_shader_kind kind);
	bool LoadTexture2D(TiXmlNode *pPassNode);
	bool LoadTexture2DArray(TiXmlNode *pPassNode);
	bool LoadTextureCubeMap(TiXmlNode *pPassNode);
	bool LoadUniformVec1(TiXmlNode *pPassNode);
	bool LoadUniformVec2(TiXmlNode *pPassNode);
	bool LoadUniformVec3(TiXmlNode *pPassNode);
	bool LoadUniformVec4(TiXmlNode *pPassNode);

public:
	bool SetPipeline(const CGfxShader *pVertexShader, const CGfxShader *pFragmentShader, const GLstate &state);

	bool SetSampler(const char *szName, GLenum minFilter, GLenum magFilter, GLenum addressMode);
	bool SetTexture2D(const char *szName, GLuint externTexture);
	bool SetTexture2DArray(const char *szName, GLuint externTexture);
	bool SetTextureCubeMap(const char *szName, GLuint externTexture);
	bool SetTexture2D(const char *szName, const char *szFileName);
	bool SetTexture2DArray(const char *szName, const char *szFileName);
	bool SetTextureCubeMap(const char *szName, const char *szFileName);

	bool SetUniformVec1(const char *szName, float x);
	bool SetUniformVec2(const char *szName, float x, float y);
	bool SetUniformVec3(const char *szName, float x, float y, float z);
	bool SetUniformVec4(const char *szName, float x, float y, float z, float w);
	bool SetUniformMat4(const char *szName, const float *matrix);

public:
	CGfxPipelineGraphics* GetPipeline(void) const;

public:
	static void Bind(const CGfxPipelineBase *pPipeline, const CGfxMaterialPass *pPass, uint32_t &indexTexUnit);
	static void BindUniforms(const CGfxPipelineBase *pPipeline, const CGfxMaterialPass *pPass);
	static void BindTextures(const CGfxPipelineBase *pPipeline, const CGfxMaterialPass *pPass, uint32_t &indexTexUnit);


private:
	uint32_t m_name;

private:
	CGfxPipelineGraphics *m_pPipeline;

	eastl::unordered_map<uint32_t, CGfxSampler*> m_pSamplers;
	eastl::unordered_map<uint32_t, CGfxTexture2DPtr> m_ptrTexture2ds;
	eastl::unordered_map<uint32_t, CGfxTexture2DArrayPtr> m_ptrTexture2dArrays;
	eastl::unordered_map<uint32_t, CGfxTextureCubeMapPtr> m_ptrTextureCubeMaps;

	eastl::unordered_map<uint32_t, CGfxUniformVec1*> m_pUniformVec1s;
	eastl::unordered_map<uint32_t, CGfxUniformVec2*> m_pUniformVec2s;
	eastl::unordered_map<uint32_t, CGfxUniformVec3*> m_pUniformVec3s;
	eastl::unordered_map<uint32_t, CGfxUniformVec4*> m_pUniformVec4s;
	eastl::unordered_map<uint32_t, CGfxUniformMat4*> m_pUniformMat4s;
};
