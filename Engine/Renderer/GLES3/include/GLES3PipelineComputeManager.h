#pragma once
#include "GLES3Renderer.h"


class CGLES3PipelineComputeManager : public CGfxResourceManager
{
	friend class CGLES3Renderer;


private:
	CGLES3PipelineComputeManager(void);
	virtual ~CGLES3PipelineComputeManager(void);


private:
	CGLES3PipelineCompute* Create(const CGfxShader *pComputeShader);


private:
	eastl::unordered_map<uint32_t, CGfxPipelineCompute*> m_pPipelines;
};
