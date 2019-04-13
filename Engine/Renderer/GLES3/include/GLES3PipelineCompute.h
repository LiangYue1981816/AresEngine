#pragma once
#include "GLES3Renderer.h"


class CGLES3PipelineCompute : public CGfxPipelineCompute
{
	friend class CGLES3PipelineComputeManager;


private:
	CGLES3PipelineCompute(uint32_t name);
	virtual ~CGLES3PipelineCompute(void);


public:
	const CGfxDescriptorLayoutPtr GetDescriptorLayout(uint32_t indexDescriptorSet) const;

public:
	bool Create(const CGfxShader *pComputeShader);
	void Destroy(void);

public:
	void Bind(void) const;
	bool BindDescriptorSet(const CGfxDescriptorSetPtr ptrDescriptorSet) const;

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


private:
	CGLES3Pipeline *m_pPipeline;
};
