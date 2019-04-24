#pragma once
#include "GLES3Renderer.h"


class CGLES3PipelineGraphics : public CGfxPipelineGraphics
{
	friend class CGLES3PipelineGraphicsManager;


private:
	CGLES3PipelineGraphics(uint32_t name);
	virtual ~CGLES3PipelineGraphics(void);


public:
	const CGfxDescriptorLayoutPtr GetDescriptorLayout(int indexDescriptorSet) const;
	const uint32_t GetInputAttachmentName(uint32_t inputAttachmentIndex) const;

public:
	bool Create(const CGfxRenderPass* pRenderPass, const CGfxShader* pVertexShader, const CGfxShader* pFragmentShader, const PipelineState& state, int indexSubpass, int vertexBinding, int instanceBinding);
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

	void Uniform1iv(uint32_t name, int count, const int* value) const;
	void Uniform2iv(uint32_t name, int count, const int* value) const;
	void Uniform3iv(uint32_t name, int count, const int* value) const;
	void Uniform4iv(uint32_t name, int count, const int* value) const;

	void Uniform1fv(uint32_t name, int count, const float* value) const;
	void Uniform2fv(uint32_t name, int count, const float* value) const;
	void Uniform3fv(uint32_t name, int count, const float* value) const;
	void Uniform4fv(uint32_t name, int count, const float* value) const;

	void UniformMatrix2fv(uint32_t name, int count, const float* value) const;
	void UniformMatrix3fv(uint32_t name, int count, const float* value) const;
	void UniformMatrix4fv(uint32_t name, int count, const float* value) const;


private:
	PipelineState m_state;
	CGLES3Pipeline* m_pPipeline;
};
