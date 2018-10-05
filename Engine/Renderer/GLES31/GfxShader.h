#pragma once
#include "GfxRenderer.h"


class CGfxShader
{
	friend class CGfxRenderer;
	friend class CGfxShaderManager;


private:
	CGfxShader(uint32_t name);
	virtual ~CGfxShader(void);

public:
	uint32_t GetName(void) const;


private:
	bool Load(const char *szFileName, shaderc_shader_kind kind);
	bool Create(const uint32_t *words, size_t numWords, shaderc_shader_kind kind);
	bool CreateLayouts(void);
	void Destroy(void);

private:
	void SetUniformBlockBinding(const char *szName, uint32_t binding);
	void SetSampledImageLocation(const char *szName, uint32_t binding);

public:
	bool BindTexture2D(uint32_t name, uint32_t texture, uint32_t sampler, uint32_t unit) const;
	bool BindTexture2DArray(uint32_t name, uint32_t texture, uint32_t sampler, uint32_t unit) const;
	bool BindTextureCubeMap(uint32_t name, uint32_t texture, uint32_t sampler, uint32_t unit) const;
	bool BindUniformBuffer(uint32_t name, uint32_t buffer, uint32_t size, int offset = 0) const;

public:
	bool IsValid(void) const;
	bool IsUniformValid(uint32_t name) const;
	bool IsTextureValid(uint32_t name) const;

public:
	uint32_t GetKind(void) const;
	uint32_t GetProgram(void) const;


private:
	uint32_t m_name;

private:
	uint32_t m_kind;
	uint32_t m_program;

private:
	eastl::unordered_map<uint32_t, uint32_t> m_uniformBlockBindings; // [name, binding]
	eastl::unordered_map<uint32_t, uint32_t> m_sampledImageLocations;// [name, location]

private:
	spirv_cross::CompilerGLSL *m_pShaderCompiler;
};
