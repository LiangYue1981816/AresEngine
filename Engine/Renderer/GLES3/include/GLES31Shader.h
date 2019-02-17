#if GLES_VER == 310

#pragma once
#include "GLES3Renderer.h"


class CGLES3Shader : public CGfxShader
{
	friend class CGLES3ShaderManager;


private:
	CGLES3Shader(uint32_t name);
	virtual ~CGLES3Shader(void);


public:
	uint32_t GetName(void) const;

public:
	HANDLE GetShader(void) const;

public:
	uint32_t GetKind(void) const;

public:
	bool Create(const uint32_t *words, size_t numWords, shader_kind kind);
	void Destroy(void);

private:
	bool CreateLayouts(void);
	void SetUniformLocation(const char *szName);
	void SetUniformBlockBinding(const char *szName, uint32_t binding);
	void SetSampledImageLocation(const char *szName);

public:
	bool BindTexture(uint32_t name, CGLES3Texture *pTexture, CGLES3Sampler *pSampler, uint32_t unit);
	bool BindUniformBuffer(uint32_t name, CGLES3UniformBuffer *pUniformBuffer, uint32_t size, int offset = 0);

	bool Uniform1i(uint32_t name, int v0) const;
	bool Uniform2i(uint32_t name, int v0, int v1) const;
	bool Uniform3i(uint32_t name, int v0, int v1, int v2) const;
	bool Uniform4i(uint32_t name, int v0, int v1, int v2, int v3) const;

	bool Uniform1f(uint32_t name, float v0) const;
	bool Uniform2f(uint32_t name, float v0, float v1) const;
	bool Uniform3f(uint32_t name, float v0, float v1, float v2) const;
	bool Uniform4f(uint32_t name, float v0, float v1, float v2, float v3) const;

	bool Uniform1iv(uint32_t name, int count, const int *value) const;
	bool Uniform2iv(uint32_t name, int count, const int *value) const;
	bool Uniform3iv(uint32_t name, int count, const int *value) const;
	bool Uniform4iv(uint32_t name, int count, const int *value) const;

	bool Uniform1fv(uint32_t name, int count, const float *value) const;
	bool Uniform2fv(uint32_t name, int count, const float *value) const;
	bool Uniform3fv(uint32_t name, int count, const float *value) const;
	bool Uniform4fv(uint32_t name, int count, const float *value) const;

	bool UniformMatrix2fv(uint32_t name, int count, const float *value) const;
	bool UniformMatrix3fv(uint32_t name, int count, const float *value) const;
	bool UniformMatrix4fv(uint32_t name, int count, const float *value) const;

public:
	bool IsValid(void) const;
	bool IsTextureValid(uint32_t name) const;
	bool IsUniformValid(uint32_t name) const;
	bool IsUniformBlockValid(uint32_t name) const;


private:
	uint32_t m_name;

private:
	CGfxSprivCross m_spriv;
	eastl::unordered_map<uint32_t, uint32_t> m_uniformLocations;      // [name, location]
	eastl::unordered_map<uint32_t, uint32_t> m_uniformBlockBindings;  // [name, binding]
	eastl::unordered_map<uint32_t, uint32_t> m_sampledImageLocations; // [name, location]

private:
	uint32_t m_kind;
	uint32_t m_program;
};

#endif
