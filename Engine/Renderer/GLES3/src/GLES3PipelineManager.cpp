#include "GLES3Renderer.h"


CGLES3PipelineComputeManager::CGLES3PipelineComputeManager(void)
{

}

CGLES3PipelineComputeManager::~CGLES3PipelineComputeManager(void)
{
	for (const auto& itPipeline : m_pPipelines) {
		delete itPipeline.second;
	}
}

CGLES3PipelineCompute* CGLES3PipelineComputeManager::Create(const CGfxShader* pComputeShader)
{
	uint32_t name = HashValueFormat("%x", pComputeShader->GetName());

	mutex_autolock autolock(&lock);
	{
		if (m_pPipelines[name] == nullptr) {
			m_pPipelines[name] = new CGLES3PipelineCompute(name);
			m_pPipelines[name]->Create(pComputeShader);
		}

		return m_pPipelines[name];
	}
}


CGLES3PipelineGraphicsManager::CGLES3PipelineGraphicsManager(void)
{

}

CGLES3PipelineGraphicsManager::~CGLES3PipelineGraphicsManager(void)
{
	for (const auto& itPipeline : m_pPipelines) {
		delete itPipeline.second;
	}
}

CGLES3PipelineGraphics* CGLES3PipelineGraphicsManager::Create(const CGfxRenderPass* pRenderPass, const CGfxShader* pVertexShader, const CGfxShader* pFragmentShader, const PipelineState& state, int indexSubpass, int vertexBinding, int instanceBinding)
{
	uint32_t name = HashValueFormat("%x_%x_%x", pVertexShader->GetName(), pFragmentShader->GetName(), HashValue((uint8_t*)& state, sizeof(state)));

	mutex_autolock autolock(&lock);
	{
		if (m_pPipelines[name] == nullptr) {
			m_pPipelines[name] = new CGLES3PipelineGraphics(name);
			m_pPipelines[name]->Create(pRenderPass, pVertexShader, pFragmentShader, state, indexSubpass, vertexBinding, instanceBinding);
		}

		return m_pPipelines[name];
	}
}
