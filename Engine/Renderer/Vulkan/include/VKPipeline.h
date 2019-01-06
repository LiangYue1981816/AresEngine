#pragma once
#include "VKRenderer.h"


class CVKPipeline
{
private:
	CVKPipeline(CVKDevice *pDevice);
	virtual ~CVKPipeline(void);


private:
	VkPipeline m_vkPipeline;
	VkPipelineLayout m_vkPipelineLayout;

private:
	VkPipelineShaderStageCreateInfo m_shaders[compute_shader - vertex_shader + 1];

private:
	CVKDevice *m_pDevice;
};
