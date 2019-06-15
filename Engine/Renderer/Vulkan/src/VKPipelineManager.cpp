#include "VKRenderer.h"


CVKPipelineComputeManager::CVKPipelineComputeManager(CVKDevice* pDevice)
	: m_pDevice(pDevice)
{

}

CVKPipelineComputeManager::~CVKPipelineComputeManager(void)
{
	for (const auto& itPipeline : m_pPipelines) {
		delete itPipeline.second;
	}
}

CVKPipelineCompute* CVKPipelineComputeManager::Create(const CGfxShader* pComputeShader)
{
	mutex_autolock autolock(&lock);
	{
		uint32_t name = HashValueFormat("%x", pComputeShader->GetName());

		if (m_pPipelines[name] == nullptr) {
			m_pPipelines[name] = new CVKPipelineCompute(m_pDevice, VK_NULL_HANDLE, name);
			m_pPipelines[name]->Create(pComputeShader);
		}

		return m_pPipelines[name];
	}
}


CVKPipelineGraphicsManager::CVKPipelineGraphicsManager(CVKDevice* pDevice)
	: m_pDevice(pDevice)
{
	ASSERT(m_pDevice);
}

CVKPipelineGraphicsManager::~CVKPipelineGraphicsManager(void)
{
	for (const auto& itPipeline : m_pPipelines) {
		delete itPipeline.second;
	}
}

CVKPipelineGraphics* CVKPipelineGraphicsManager::Create(const CGfxRenderPass* pRenderPass, const CGfxShader* pVertexShader, const CGfxShader* pFragmentShader, const PipelineState& state, uint32_t indexSubpass, uint32_t vertexBinding, uint32_t instanceBinding)
{
	mutex_autolock autolock(&lock);
	{
		uint32_t name = HashValueFormat("%p_%x_%x_%x_%x_%x_%x", pRenderPass, pVertexShader->GetName(), pFragmentShader->GetName(), HashValue((uint8_t*)& state, sizeof(state)), indexSubpass, vertexBinding, instanceBinding);

		if (m_pPipelines[name] == nullptr) {
			m_pPipelines[name] = new CVKPipelineGraphics(m_pDevice, VK_NULL_HANDLE, name);
			m_pPipelines[name]->Create(pRenderPass, pVertexShader, pFragmentShader, state, indexSubpass, vertexBinding, instanceBinding);
		}

		return m_pPipelines[name];
	}
}
