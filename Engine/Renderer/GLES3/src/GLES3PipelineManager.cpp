#include "GLES3Renderer.h"


CGLES3PipelineManager::CGLES3PipelineManager(void)
{

}

CGLES3PipelineManager::~CGLES3PipelineManager(void)
{
	for (const auto &itPipeline : m_pPipelineComputes) {
		delete itPipeline.second;
	}

	for (const auto &itPipeline : m_pPipelineGraphicses) {
		delete itPipeline.second;
	}

	m_pPipelineComputes.clear();
	m_pPipelineGraphicses.clear();
}

CGLES3PipelineCompute* CGLES3PipelineManager::CreatePipelineCompute(const CGfxShader *pComputeShader)
{
	mutex_autolock autolock(&lock);
	{
		char szName[_MAX_STRING];
		sprintf(szName, "%8.8X", pComputeShader->GetName());

		uint32_t name = HashValue(szName);

		if (m_pPipelineComputes[name] == nullptr) {
			m_pPipelineComputes[name] = new CGLES3PipelineCompute(name);
			((CGLES3PipelineCompute *)m_pPipelineComputes[name])->Create(pComputeShader);
		}

		return (CGLES3PipelineCompute *)m_pPipelineComputes[name];
	}
}

CGLES3PipelineGraphics* CGLES3PipelineManager::CreatePipelineGraphics(const CGfxShader *pVertexShader, const CGfxShader *pFragmentShader, const PipelineState &state)
{
	mutex_autolock autolock(&lock);
	{
		char szName[_MAX_STRING];
		sprintf(szName, "%8.8X_%8.8X_%8.8X", pVertexShader->GetName(), pFragmentShader->GetName(), HashValue((uint8_t *)&state, sizeof(state)));

		uint32_t name = HashValue(szName);

		if (m_pPipelineGraphicses[name] == nullptr) {
			m_pPipelineGraphicses[name] = new CGLES3PipelineGraphics(name);
			((CGLES3PipelineGraphics *)m_pPipelineGraphicses[name])->Create(pVertexShader, pFragmentShader, state);
		}

		return (CGLES3PipelineGraphics *)m_pPipelineGraphicses[name];
	}
}
