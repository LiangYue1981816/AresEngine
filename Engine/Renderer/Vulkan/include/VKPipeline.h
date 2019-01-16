#pragma once
#include "VKRenderer.h"


class CVKPipeline
{
protected:
	CVKPipeline(CVKDevice *pDevice);
	virtual ~CVKPipeline(void);


private:
	bool CreateLayouts(eastl::vector<VkDescriptorSetLayout> &layouts);
	bool CreateShaderStages(eastl::vector<VkPipelineShaderStageCreateInfo> &shaders);


private:
	VkPipeline m_vkPipeline;
	VkPipelineLayout m_vkPipelineLayout;

private:
	CVKShader *m_pShaders[compute_shader - vertex_shader + 1];
	CVKDescriptorSetLayout *m_pLayouts[DESCRIPTOR_SET_COUNT];

private:
	CVKDevice *m_pDevice;
};
