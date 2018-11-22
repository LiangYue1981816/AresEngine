#if GLES_VER == 310

#pragma once
#include "GLES3Renderer.h"


class CGLES3PipelineGraphics : public CGLES3PipelineBase, public CGfxPipelineGraphics
{
	friend class CGLES3PipelineManager;


private:
	CGLES3PipelineGraphics(uint32_t name);
	virtual ~CGLES3PipelineGraphics(void);


private:
	bool Create(const CGfxShader *pVertexShader, const CGfxShader *pFragmentShader, const PipelineState &state);
	void Destroy(void);

public:
	void Bind(void *pParam);


private:
	PipelineState m_state;
};

#endif
