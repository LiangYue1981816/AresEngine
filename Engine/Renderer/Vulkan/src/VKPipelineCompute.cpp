#include "VKRenderer.h"


CVKPipelineCompute::CVKPipelineCompute(CVKDevice* pDevice, uint32_t name)
	: CGfxPipelineCompute(name)
	, m_pDevice(pDevice)
	, m_pPipeline(nullptr)
{
	ASSERT(m_pDevice);

	m_pPipeline = new CVKPipeline(m_pDevice);
}

CVKPipelineCompute::~CVKPipelineCompute(void)
{
	delete m_pPipeline;
}

const CGfxDescriptorLayoutPtr CVKPipelineCompute::GetDescriptorLayout(int indexDescriptorSet) const
{
	return m_pPipeline->GetDescriptorLayout(indexDescriptorSet);
}

bool CVKPipelineCompute::Create(const CGfxShader* pComputeShader)
{
	return m_pPipeline->Create(pComputeShader);
}

void CVKPipelineCompute::Destroy(void)
{
	m_pPipeline->Destroy();
}

void CVKPipelineCompute::Bind(VkCommandBuffer vkCommandBuffer)
{
	m_pPipeline->Bind(vkCommandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE);
}
