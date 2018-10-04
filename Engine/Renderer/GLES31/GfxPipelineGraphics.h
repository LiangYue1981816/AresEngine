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
	bool CreateProgram(const CGfxShader *pVertexShader, const CGfxShader *pFragmentShader);
	bool CreateState(const GLstate &state);

public:
	void SetEnableCullFace(bool bEnable, GLenum cullFace, GLenum frontFace);
	void SetEnableDepthTest(bool bEnable, GLenum depthFunc);
	void SetEnableDepthWrite(bool bEnable);
	void SetEnableColorWrite(bool bEnableRed, bool bEnableGreen, bool bEnableBlue, bool bEnableAlpha);
	void SetEnableBlend(bool bEnable, GLenum srcFactor, GLenum dstFactor);
	void SetEnablePolygonOffset(bool bEnable, GLfloat factor, GLfloat units);

	bool IsEnableBlend(void) const;

public:
	void BindPipeline(void) const;


private:
	GLstate m_state;
};
