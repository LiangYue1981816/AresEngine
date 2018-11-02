#pragma once
#include "GfxRenderer.h"


class CGfxPipelineGraphics : public CGfxPipelineBase
{
	friend class CGfxRenderer;
	friend class CGfxPipelineManager;


private:
	CGfxPipelineGraphics(uint32_t name);
	virtual ~CGfxPipelineGraphics(void);


private:
	bool Create(const CGfxShader *pVertexShader, const CGfxShader *pFragmentShader, const GLstate &state);
	void CreateProgram(const CGfxShader *pVertexShader, const CGfxShader *pFragmentShader);
	void CreateState(const GLstate &state);
	void Destroy(void);

public:
	void BindPipeline(void) const;


private:
	GLstate m_state;
};
