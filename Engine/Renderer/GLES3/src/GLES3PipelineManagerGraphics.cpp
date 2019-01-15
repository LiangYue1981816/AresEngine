#include "GLES3Renderer.h"


CGLES3PipelineGraphicsManager::CGLES3PipelineGraphicsManager(void)
{

}

CGLES3PipelineGraphicsManager::~CGLES3PipelineGraphicsManager(void)
{
	for (const auto &itPipeline : m_pPipelines) {
		delete itPipeline.second;
	}

	m_pPipelines.clear();
}

CGLES3PipelineGraphics* CGLES3PipelineGraphicsManager::Create(const CGfxShader *pVertexShader, const CGfxShader *pFragmentShader, const PipelineState &state)
{
	mutex_autolock autolock(&lock);
	{
		char szName[_MAX_STRING];
		sprintf(szName, "%8.8X_%8.8X_%8.8X", pVertexShader->GetName(), pFragmentShader->GetName(), HashValue((uint8_t *)&state, sizeof(state)));

		uint32_t name = HashValue(szName);

		if (m_pPipelines[name] == nullptr) {
			m_pPipelines[name] = new CGLES3PipelineGraphics(name);
			((CGLES3PipelineGraphics *)m_pPipelines[name])->Create(pVertexShader, pFragmentShader, state);
		}

		return (CGLES3PipelineGraphics *)m_pPipelines[name];
	}
}
