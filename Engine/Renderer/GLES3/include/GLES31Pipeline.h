#if GLES_VER == 310

#pragma once
#include "GLES3Renderer.h"


class CGLES3Pipeline
{
protected:
	CGLES3Pipeline(void);
	virtual ~CGLES3Pipeline(void);


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
	bool IsTextureValid(uint32_t name) const;
	bool IsUniformValid(uint32_t name) const;
	bool IsUniformBlockValid(uint32_t name) const;


protected:
	uint32_t m_pipeline;
	CGLES3Shader *m_pShaders[compute_shader - vertex_shader + 1];
};

#endif
