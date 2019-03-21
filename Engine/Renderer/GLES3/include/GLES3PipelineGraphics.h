#pragma once
#include "GLES3Renderer.h"


class CGLES3PipelineGraphics : public CGLES3Pipeline, public CGfxPipelineGraphics
{
	friend class CGLES3PipelineGraphicsManager;


private:
	CGLES3PipelineGraphics(uint32_t name);
	virtual ~CGLES3PipelineGraphics(void);


public:
	HANDLE GetPipeline(void) const;
	const PipelineState* GetPipelineState(void) const;

public:
	bool Create(const CGfxRenderPass *pRenderPass, const CGfxShader *pVertexShader, const CGfxShader *pFragmentShader, const PipelineState &state, uint32_t indexSubpass, uint32_t vertexBinding, uint32_t instanceBinding);
	void Destroy(void);

public:
	void Bind(void) const;


private:
	PipelineState m_state;
};
