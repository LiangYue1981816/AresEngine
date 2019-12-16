#pragma once
#include "GLES3Renderer.h"


class CGLES3Pipeline
{
	friend class CGLES3PipelineCompute;
	friend class CGLES3PipelineGraphics;


private:
	CGLES3Pipeline(void);
	virtual ~CGLES3Pipeline(void);


private:
	const CGfxDescriptorLayoutPtr GetDescriptorLayout(int indexDescriptorSet) const;

private:
	bool CreateProgram(const CGLES3Shader* pVertexShader, const CGLES3Shader* pFragmentShader, const CGLES3Shader* pComputeShader);
	bool CreateLayouts(void);
	bool CreateVertexFormat(int vertexBinding, int instanceBinding);

	bool Create(const CGfxShader* pComputeShader);
	bool Create(const CGfxRenderPass* pRenderPass, const CGfxShader* pVertexShader, const CGfxShader* pFragmentShader, const PipelineState& state, int indexSubpass, int vertexBinding, int instanceBinding);
	void Destroy(void);

private:
	void SetStorageBlockBinding(const char* szName, uint32_t binding);
	void SetUniformBlockBinding(const char* szName, uint32_t binding);
	void SetUniformLocation(const char* szName);
	void SetSampledImageLocation(const char* szName);
	void SetInputAttachmentLocation(const char* szName, uint32_t inputAttachmentIndex);

private:
	uint32_t GetInputAttachmentName(uint32_t inputAttachmentIndex) const;
	bool IsCompatibleVertexFormat(uint32_t binding, uint32_t format) const;

private:
	void Bind(const PipelineState* pState) const;
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
	uint32_t m_program;
	CGLES3Shader* m_pShaders[compute_shader - vertex_shader + 1];
	CGfxDescriptorLayoutPtr m_ptrDescriptorLayouts[DESCRIPTOR_SET_COUNT];

private:
	eastl::unordered_map<uint32_t, uint32_t> m_storageBlockBindings;     // [name, binding]
	eastl::unordered_map<uint32_t, uint32_t> m_uniformBlockBindings;     // [name, binding]
	eastl::unordered_map<uint32_t, uint32_t> m_uniformLocations;         // [name, location]
	eastl::unordered_map<uint32_t, uint32_t> m_sampledImageLocations;    // [name, location]
	eastl::unordered_map<uint32_t, uint32_t> m_sampledImageTextureUnits; // [name, texture unit]
	eastl::unordered_map<uint32_t, uint32_t> m_inputAttachmentNames;     // [input attachment index, name]
	eastl::unordered_map<uint32_t, uint32_t> m_vertexFormats;            // [binding, format]
};
