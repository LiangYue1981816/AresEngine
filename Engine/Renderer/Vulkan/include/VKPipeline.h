#pragma once
#include "VKRenderer.h"


class CVKPipeline
{
protected:
	CVKPipeline(CVKDevice *pDevice);
	virtual ~CVKPipeline(void);


protected:
	bool CreateLayouts(eastl::vector<VkDescriptorSetLayout> &layouts);
	bool CreateShaderStages(eastl::vector<VkPipelineShaderStageCreateInfo> &shaders);
	bool CreateVertexInputState(eastl::vector<VkVertexInputBindingDescription> &inputBindingDescriptions, eastl::vector<VkVertexInputAttributeDescription> &inputAttributeDescriptions, uint32_t vertexBinding, uint32_t instanceBinding);


protected:
	VkPipeline m_vkPipeline;
	VkPipelineLayout m_vkPipelineLayout;

protected:
	CVKShader *m_pShaders[compute_shader - vertex_shader + 1];
	CVKDescriptorSetLayout *m_pLayouts[DESCRIPTOR_SET_COUNT];

protected:
	CVKDevice *m_pDevice;
};
