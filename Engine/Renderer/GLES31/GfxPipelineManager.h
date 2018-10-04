#pragma once
#include "GfxRenderer.h"


class CGfxPipelineManager
{
	friend class CGfxRenderer;


private:
	CGfxPipelineManager(void);
	virtual ~CGfxPipelineManager(void);


private:
	CGfxPipelineCompute* CreatePipelineCompute(const CGfxShader *pComputeShader);
	CGfxPipelineGraphics* CreatePipelineGraphics(const CGfxShader *pVertexShader, const CGfxShader *pFragmentShader, const GLstate &state);
	void DestroyPipeline(CGfxPipelineBase *pPipeline);


private:
	eastl::unordered_map<uint32_t, CGfxPipelineBase*> m_pPipelines;
};
