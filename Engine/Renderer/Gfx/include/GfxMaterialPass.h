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
	virtual bool SetPipeline(const CGfxShader *pVertexShader, const CGfxShader *pFragmentShader, const PipelineState &state) = 0;

	virtual bool SetSampler(const char *szName, GfxMinFilter minFilter, GfxMagFilter magFilter, GfxAddressMode addressMode) = 0;
	virtual bool SetTexture2D(const char *szName, uint32_t externTexture) = 0;
	virtual bool SetTexture2DArray(const char *szName, uint32_t externTexture) = 0;
	virtual bool SetTextureCubeMap(const char *szName, uint32_t externTexture) = 0;
	virtual bool SetTexture2D(const char *szName, const char *szFileName) = 0;
	virtual bool SetTexture2DArray(const char *szName, const char *szFileName) = 0;
	virtual bool SetTextureCubeMap(const char *szName, const char *szFileName) = 0;

	virtual bool SetUniformVec1(const char *szName, float v0) = 0;
	virtual bool SetUniformVec2(const char *szName, float v0, float v1) = 0;
	virtual bool SetUniformVec3(const char *szName, float v0, float v1, float v2) = 0;
	virtual bool SetUniformVec4(const char *szName, float v0, float v1, float v2, float v3) = 0;
	virtual bool SetUniformMat4(const char *szName, const float *value) = 0;

public:
	virtual CGfxPipelineGraphics* GetPipeline(void) const = 0;


private:
	uint32_t m_name;
};
