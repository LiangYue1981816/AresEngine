#pragma once
#include "VKRenderer.h"


class CVKPipelineCompute : public CGfxPipelineCompute
{
public:
	CVKPipelineCompute(uint32_t name, CVKDevice* pDevice, VkPipelineCache vkPipelineCache, const CGfxShader* pComputeShader);
	virtual ~CVKPipelineCompute(void);


public:
	const VkPipelineLayout GetPipelineLayout(void) const;
	const CGfxDescriptorLayoutPtr GetDescriptorLayout(int indexDescriptorSet) const;


private:
	CVKPipeline* m_pPipeline;

private:
	CVKDevice* m_pDevice;
};
