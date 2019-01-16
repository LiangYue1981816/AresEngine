#pragma once
#include "VKRenderer.h"


class CVKPipelineComputeManager : public CGfxResourceManager
{
	friend class CVKRenderer;


private:
	CVKPipelineComputeManager(CVKDevice *pDevice);
	virtual ~CVKPipelineComputeManager(void);


private:
	CVKPipelineCompute* Create(const CGfxShader *pComputeShader);


private:
	eastl::unordered_map<uint32_t, CGfxPipelineCompute*> m_pPipelines;

private:
	CVKDevice *m_pDevice;
};


class CVKPipelineGraphicsManager : public CGfxResourceManager
{
	friend class CVKRenderer;


private:
	CVKPipelineGraphicsManager(CVKDevice *pDevice);
	virtual ~CVKPipelineGraphicsManager(void);


private:
	CVKPipelineGraphics* Create(const CGfxShader *pVertexShader, const CGfxShader *pFragmentShader, const PipelineState &state);


private:
	eastl::unordered_map<uint32_t, CGfxPipelineGraphics*> m_pPipelines;

private:
	CVKDevice *m_pDevice;
};
