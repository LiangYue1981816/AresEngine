#pragma once
#include "GLES3Renderer.h"


class CGLES3PipelineGraphicsManager : public CGfxResourceManager
{
	friend class CGLES3Renderer;


private:
	CGLES3PipelineGraphicsManager(void);
	virtual ~CGLES3PipelineGraphicsManager(void);


private:
	CGLES3PipelineGraphics* Create(const CGfxShader *pVertexShader, const CGfxShader *pFragmentShader, const PipelineState &state);


private:
	eastl::unordered_map<uint32_t, CGfxPipelineGraphics*> m_pPipelines;
};
