#pragma once
#include "VKRenderer.h"


class CVKPipeline
{
protected:
	CVKPipeline(CVKDevice *pDevice);
	virtual ~CVKPipeline(void);


protected:
	bool CreateLayouts(eastl::vector<VkDescriptorSetLayout> &layouts, eastl::vector<VkPushConstantRange> &pushConstantRanges);
	bool CreateShaderStages(eastl::vector<VkPipelineShaderStageCreateInfo> &shaders);
	bool CreateVertexInputState(eastl::vector<VkVertexInputBindingDescription> &inputBindingDescriptions, eastl::vector<VkVertexInputAttributeDescription> &inputAttributeDescriptions, uint32_t vertexBinding, uint32_t instanceBinding);

public:
	bool Uniform1i(VkCommandBuffer vkCommandBuffer, uint32_t name, int v0) const;
	bool Uniform2i(VkCommandBuffer vkCommandBuffer, uint32_t name, int v0, int v1) const;
	bool Uniform3i(VkCommandBuffer vkCommandBuffer, uint32_t name, int v0, int v1, int v2) const;
	bool Uniform4i(VkCommandBuffer vkCommandBuffer, uint32_t name, int v0, int v1, int v2, int v3) const;

	bool Uniform1f(VkCommandBuffer vkCommandBuffer, uint32_t name, float v0) const;
	bool Uniform2f(VkCommandBuffer vkCommandBuffer, uint32_t name, float v0, float v1) const;
	bool Uniform3f(VkCommandBuffer vkCommandBuffer, uint32_t name, float v0, float v1, float v2) const;
	bool Uniform4f(VkCommandBuffer vkCommandBuffer, uint32_t name, float v0, float v1, float v2, float v3) const;

	bool Uniform1iv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const int *value) const;
	bool Uniform2iv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const int *value) const;
	bool Uniform3iv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const int *value) const;
	bool Uniform4iv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const int *value) const;

	bool Uniform1fv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const float *value) const;
	bool Uniform2fv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const float *value) const;
	bool Uniform3fv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const float *value) const;
	bool Uniform4fv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const float *value) const;

	bool UniformMatrix2fv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const float *value) const;
	bool UniformMatrix3fv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const float *value) const;
	bool UniformMatrix4fv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const float *value) const;

public:
	bool IsTextureValid(uint32_t name) const;
	bool IsUniformValid(uint32_t name) const;
	bool IsUniformBlockValid(uint32_t name) const;


protected:
	VkPipeline m_vkPipeline;
	VkPipelineLayout m_vkPipelineLayout;

protected:
	CVKShader *m_pShaders[compute_shader - vertex_shader + 1];
	CVKDescriptorSetLayout *m_pLayouts[DESCRIPTOR_SET_COUNT];
	eastl::unordered_map<uint32_t, VkPushConstantRange> m_pushConstantRanges;

protected:
	CVKDevice *m_pDevice;
};
