#pragma once
#include "VKRenderer.h"


class CVKPipeline
{
	friend class CVKPipelineCompute;
	friend class CVKPipelineGraphics;


private:
	CVKPipeline(CVKDevice* pDevice);
	virtual ~CVKPipeline(void);


private:
	const VkPipelineLayout GetPipelineLayout(void) const;
	const CGfxDescriptorLayoutPtr GetDescriptorLayout(int indexDescriptorSet) const;

private:
	bool CreateLayouts(void);
	bool CreateShaderStages(eastl::vector<VkPipelineShaderStageCreateInfo>& shaders);
	bool CreateVertexInputState(eastl::vector<VkVertexInputBindingDescription>& inputBindingDescriptions, eastl::vector<VkVertexInputAttributeDescription>& inputAttributeDescriptions, int vertexBinding, int instanceBinding);

	bool Create(const CGfxShader* pComputeShader);
	bool Create(const CGfxRenderPass* pRenderPass, const CGfxShader* pVertexShader, const CGfxShader* pFragmentShader, const PipelineState& state, int indexSubpass, int vertexBinding, int instanceBinding);
	void Destroy(void);

private:
	uint32_t GetInputAttachmentName(uint32_t inputAttachmentIndex) const;
	bool IsCompatibleVertexFormat(uint32_t binding, uint32_t format) const;

private:
	void Bind(VkCommandBuffer vkCommandBuffer, VkPipelineBindPoint pipelineBindPoint);

	void Uniform1i(VkCommandBuffer vkCommandBuffer, uint32_t name, int v0) const;
	void Uniform2i(VkCommandBuffer vkCommandBuffer, uint32_t name, int v0, int v1) const;
	void Uniform3i(VkCommandBuffer vkCommandBuffer, uint32_t name, int v0, int v1, int v2) const;
	void Uniform4i(VkCommandBuffer vkCommandBuffer, uint32_t name, int v0, int v1, int v2, int v3) const;

	void Uniform1f(VkCommandBuffer vkCommandBuffer, uint32_t name, float v0) const;
	void Uniform2f(VkCommandBuffer vkCommandBuffer, uint32_t name, float v0, float v1) const;
	void Uniform3f(VkCommandBuffer vkCommandBuffer, uint32_t name, float v0, float v1, float v2) const;
	void Uniform4f(VkCommandBuffer vkCommandBuffer, uint32_t name, float v0, float v1, float v2, float v3) const;

	void Uniform1iv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const int* value) const;
	void Uniform2iv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const int* value) const;
	void Uniform3iv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const int* value) const;
	void Uniform4iv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const int* value) const;

	void Uniform1fv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const float* value) const;
	void Uniform2fv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const float* value) const;
	void Uniform3fv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const float* value) const;
	void Uniform4fv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const float* value) const;

	void UniformMatrix2fv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const float* value) const;
	void UniformMatrix3fv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const float* value) const;
	void UniformMatrix4fv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const float* value) const;


private:
	VkPipeline m_vkPipeline;
	VkPipelineLayout m_vkPipelineLayout;

private:
	CVKShader* m_pShaders[compute_shader - vertex_shader + 1];
	CGfxDescriptorLayoutPtr m_ptrDescriptorLayouts[DESCRIPTOR_SET_COUNT];
	eastl::unordered_map<uint32_t, VkPushConstantRange> m_pushConstantRanges;
	eastl::unordered_map<uint32_t, uint32_t> m_inputAttachmentNames; // [input attachment index, name]
	eastl::unordered_map<uint32_t, uint32_t> m_vertexFormats;        // [binding, format]

private:
	CVKDevice* m_pDevice;
};
