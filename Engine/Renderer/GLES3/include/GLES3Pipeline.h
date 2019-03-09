#pragma once
#include "GLES3Renderer.h"


class CGLES3Pipeline
{
protected:
	CGLES3Pipeline(void);
	virtual ~CGLES3Pipeline(void);


protected:
	bool CreateLayouts(void);
	void SetUniformLocation(const char *szName);
	void SetUniformBlockBinding(const char *szName, uint32_t binding);
	void SetSampledImageLocation(const char *szName);

public:
	void BindTexture(uint32_t name, CGLES3Texture *pTexture, CGLES3Sampler *pSampler, uint32_t unit) const;
	void BindRenderTexture(uint32_t name, CGLES3RenderTexture *pRenderTexture, CGLES3Sampler *pSampler, uint32_t unit) const;
	void BindUniformBuffer(uint32_t name, CGLES3UniformBuffer *pUniformBuffer, uint32_t size, int offset = 0) const;

	void Uniform1i(uint32_t name, int v0) const;
	void Uniform2i(uint32_t name, int v0, int v1) const;
	void Uniform3i(uint32_t name, int v0, int v1, int v2) const;
	void Uniform4i(uint32_t name, int v0, int v1, int v2, int v3) const;

	void Uniform1f(uint32_t name, float v0) const;
	void Uniform2f(uint32_t name, float v0, float v1) const;
	void Uniform3f(uint32_t name, float v0, float v1, float v2) const;
	void Uniform4f(uint32_t name, float v0, float v1, float v2, float v3) const;

	void Uniform1iv(uint32_t name, int count, const int *value) const;
	void Uniform2iv(uint32_t name, int count, const int *value) const;
	void Uniform3iv(uint32_t name, int count, const int *value) const;
	void Uniform4iv(uint32_t name, int count, const int *value) const;

	void Uniform1fv(uint32_t name, int count, const float *value) const;
	void Uniform2fv(uint32_t name, int count, const float *value) const;
	void Uniform3fv(uint32_t name, int count, const float *value) const;
	void Uniform4fv(uint32_t name, int count, const float *value) const;

	void UniformMatrix2fv(uint32_t name, int count, const float *value) const;
	void UniformMatrix3fv(uint32_t name, int count, const float *value) const;
	void UniformMatrix4fv(uint32_t name, int count, const float *value) const;

public:
	bool IsTextureValid(uint32_t name) const;
	bool IsUniformValid(uint32_t name) const;
	bool IsUniformBlockValid(uint32_t name) const;


protected:
	uint32_t m_program;
	CGLES3Shader *m_pShaders[compute_shader - vertex_shader + 1];

protected:
	eastl::unordered_map<uint32_t, uint32_t> m_uniformLocations;      // [name, location]
	eastl::unordered_map<uint32_t, uint32_t> m_uniformBlockBindings;  // [name, binding]
	eastl::unordered_map<uint32_t, uint32_t> m_sampledImageLocations; // [name, location]
};
