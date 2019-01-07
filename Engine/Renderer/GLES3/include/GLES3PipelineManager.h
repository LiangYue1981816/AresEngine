#pragma once
#include "GLES3Renderer.h"


class CGLES3PipelineManager : public CGfxResourceManager
{
	friend class CGLES3Renderer;


private:
	CGLES3PipelineManager(void);
	virtual ~CGLES3PipelineManager(void);


private:
	CGLES3PipelineCompute* CreatePipelineCompute(const CGfxShader *pComputeShader);
	CGLES3PipelineGraphics* CreatePipelineGraphics(const CGfxShader *pVertexShader, const CGfxShader *pFragmentShader, const PipelineState &state);


private:
	eastl::unordered_map<uint32_t, CGfxPipelineCompute*> m_pPipelineComputes;
	eastl::unordered_map<uint32_t, CGfxPipelineGraphics*> m_pPipelineGraphicses;
};
