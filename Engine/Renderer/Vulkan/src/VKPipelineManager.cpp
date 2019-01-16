#include "VKRenderer.h"


CVKPipelineComputeManager::CVKPipelineComputeManager(CVKDevice *pDevice)
	: m_pDevice(pDevice)
{

}

CVKPipelineComputeManager::~CVKPipelineComputeManager(void)
{
	for (const auto &itPipeline : m_pPipelines) {
		delete itPipeline.second;
	}

	m_pPipelines.clear();
}

CVKPipelineCompute* CVKPipelineComputeManager::Create(const CGfxShader *pComputeShader)
{
	mutex_autolock autolock(&lock);
	{
		char szName[_MAX_STRING];
		sprintf(szName, "%8.8X", pComputeShader->GetName());

		uint32_t name = HashValue(szName);

		if (m_pPipelines[name] == nullptr) {
			m_pPipelines[name] = new CVKPipelineCompute(m_pDevice, this, name);
			((CVKPipelineCompute *)m_pPipelines[name])->Create(pComputeShader);
		}

		return (CVKPipelineCompute *)m_pPipelines[name];
	}
}


CVKPipelineGraphicsManager::CVKPipelineGraphicsManager(CVKDevice *pDevice)
	: m_pDevice(pDevice)
{

}

CVKPipelineGraphicsManager::~CVKPipelineGraphicsManager(void)
{
	for (const auto &itPipeline : m_pPipelines) {
		delete itPipeline.second;
	}

	m_pPipelines.clear();
}

CVKPipelineGraphics* CVKPipelineGraphicsManager::Create(const CGfxShader *pVertexShader, const CGfxShader *pFragmentShader, const PipelineState &state)
{
	mutex_autolock autolock(&lock);
	{
		char szName[_MAX_STRING];
		sprintf(szName, "%8.8X_%8.8X_%8.8X", pVertexShader->GetName(), pFragmentShader->GetName(), HashValue((uint8_t *)&state, sizeof(state)));

		uint32_t name = HashValue(szName);

		if (m_pPipelines[name] == nullptr) {
			m_pPipelines[name] = new CVKPipelineGraphics(m_pDevice, this, name);
			((CVKPipelineGraphics *)m_pPipelines[name])->Create(pVertexShader, pFragmentShader, state);
		}

		return (CVKPipelineGraphics *)m_pPipelines[name];
	}
}
