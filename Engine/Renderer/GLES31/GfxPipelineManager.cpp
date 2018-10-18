#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxPipelineManager.h"


CGfxPipelineManager::CGfxPipelineManager(void)
{

}

CGfxPipelineManager::~CGfxPipelineManager(void)
{
	for (const auto &itPipeline : m_pPipelines) {
		delete itPipeline.second;
	}

	m_pPipelines.clear();
}

CGfxPipelineCompute* CGfxPipelineManager::CreatePipelineCompute(const CGfxShader *pComputeShader)
{
	char szName[_MAX_STRING];
	sprintf(szName, "%8.8X", pComputeShader->GetName());

	uint32_t name = HashValue(szName);

	if (m_pPipelines[name] == NULL) {
		m_pPipelines[name] = new CGfxPipelineCompute(name);
		((CGfxPipelineCompute *)m_pPipelines[name])->Create(pComputeShader);
	}

	return (CGfxPipelineCompute *)m_pPipelines[name];
}

CGfxPipelineGraphics* CGfxPipelineManager::CreatePipelineGraphics(const CGfxShader *pVertexShader, const CGfxShader *pFragmentShader, const GLstate &state)
{
	char szName[_MAX_STRING];
	sprintf(szName, "%8.8X_%8.8X_%8.8X", pVertexShader->GetName(), pFragmentShader->GetName(), HashValue((unsigned char *)&state, sizeof(state)));

	uint32_t name = HashValue(szName);

	if (m_pPipelines[name] == NULL) {
		m_pPipelines[name] = new CGfxPipelineGraphics(name);
		((CGfxPipelineGraphics *)m_pPipelines[name])->Create(pVertexShader, pFragmentShader, state);
	}

	return (CGfxPipelineGraphics *)m_pPipelines[name];
}

void CGfxPipelineManager::DestroyPipeline(CGfxPipelineBase *pPipeline)
{
	if (pPipeline) {
		m_pPipelines.erase(pPipeline->GetName());
		delete pPipeline;
	}
}
