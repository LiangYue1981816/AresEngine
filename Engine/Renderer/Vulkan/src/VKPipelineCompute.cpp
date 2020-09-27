#include "VKRenderer.h"


CVKPipelineCompute::CVKPipelineCompute(uint32_t name, CVKDevice* pDevice, VkPipelineCache vkPipelineCache, const CGfxShader* pComputeShader)
	: CGfxPipelineCompute(name)
	, m_pDevice(pDevice)
	, m_pPipeline(nullptr)
{
	m_pPipeline = new CVKPipeline(pDevice, vkPipelineCache, pComputeShader);
}

CVKPipelineCompute::~CVKPipelineCompute(void)
{
	delete m_pPipeline;
}

const VkPipelineLayout CVKPipelineCompute::GetPipelineLayout(void) const
{
	return m_pPipeline->GetPipelineLayout();
}

const CGfxDescriptorLayoutPtr CVKPipelineCompute::GetDescriptorLayout(int indexDescriptorSet) const
{
	return m_pPipeline->GetDescriptorLayout(indexDescriptorSet);
}
