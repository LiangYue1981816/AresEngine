#pragma once
#include "GLES3Renderer.h"


class CGLES3PipelineGraphics : public CGLES3Pipeline, public CGfxPipelineGraphics
{
	friend class CGLES3PipelineGraphicsManager;


private:
	CGLES3PipelineGraphics(uint32_t name);
	virtual ~CGLES3PipelineGraphics(void);


public:
	uint32_t GetName(void) const;

public:
	HANDLE GetPipeline(void) const;
	const PipelineState& GetPipelineState(void) const;

public:
	bool Create(const CGfxRenderPass *pRenderPass, const CGfxShader *pVertexShader, const CGfxShader *pFragmentShader, const PipelineState &state, uint32_t indexSubpass, uint32_t vertexBinding, uint32_t instanceBinding);
	void Destroy(void);

public:
	bool IsTextureValid(uint32_t name) const;
	bool IsUniformValid(uint32_t name) const;
	bool IsUniformBlockValid(uint32_t name) const;

public:
	void Bind(void);


private:
	uint32_t m_name;

private:
	PipelineState m_state;
};
