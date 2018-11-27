#if GLES_VER == 300

#pragma once
#include "GLES3Renderer.h"


class CGLES3PipelineGraphics : public CGLES3PipelineBase, public CGfxPipelineGraphics
{
	friend class CGLES3PipelineManager;


private:
	CGLES3PipelineGraphics(uint32_t name);
	virtual ~CGLES3PipelineGraphics(void);


public:
	bool Create(const CGfxShader *pVertexShader, const CGfxShader *pFragmentShader, const PipelineState &state);
	void Destroy(void);

public:
	bool IsTextureValid(uint32_t name) const;
	bool IsUniformValid(uint32_t name) const;
	bool IsUniformBlockValid(uint32_t name) const;

public:
	void Bind(void *pParam);


private:
	PipelineState m_state;
};

#endif
