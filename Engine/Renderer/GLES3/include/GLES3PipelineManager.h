#pragma once
#include "GLES3Renderer.h"


class CGLES3PipelineComputeManager : public CGfxResourceManager
{
	friend class CGLES3Renderer;
	friend class CGLES3PipelineCompute;


private:
	CGLES3PipelineComputeManager(void);
	virtual ~CGLES3PipelineComputeManager(void);


private:
	CGLES3PipelineCompute* Create(const CGfxShader* pComputeShader);


private:
	eastl::unordered_map<uint32_t, CGLES3PipelineCompute*> m_pPipelines;
};


class CGLES3PipelineGraphicsManager : public CGfxResourceManager
{
	friend class CGLES3Renderer;
	friend class CGLES3PipelineGraphics;


private:
	CGLES3PipelineGraphicsManager(void);
	virtual ~CGLES3PipelineGraphicsManager(void);


private:
	CGLES3PipelineGraphics* Create(const CGfxRenderPass* pRenderPass, const CGfxShader* pVertexShader, const CGfxShader* pFragmentShader, const PipelineState& state, int indexSubpass, int vertexBinding, int instanceBinding);


private:
	eastl::unordered_map<uint32_t, CGLES3PipelineGraphics*> m_pPipelines;
};
