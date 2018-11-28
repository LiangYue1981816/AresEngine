#pragma once
#include "GLES3Renderer.h"


class CGLES3PipelineManager : public CGfxResourceManager
{
	friend class CGLES3Renderer;


private:
	CGLES3PipelineManager(void);
	virtual ~CGLES3PipelineManager(void);


private:
	CGfxPipelineCompute* CreatePipelineCompute(const CGfxShader *pComputeShader);
	CGfxPipelineGraphics* CreatePipelineGraphics(const CGfxShader *pVertexShader, const CGfxShader *pFragmentShader, const PipelineState &state);
	void Destroy(CGfxPipelineBase *pPipeline);


private:
	eastl::unordered_map<uint32_t, CGfxPipelineBase*> m_pPipelines;
};
