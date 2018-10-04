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
	void SetEnableCullFace(bool bEnable, uint32_t cullFace, uint32_t frontFace);
	void SetEnableDepthTest(bool bEnable, uint32_t depthFunc);
	void SetEnableDepthWrite(bool bEnable);
	void SetEnableColorWrite(bool bEnableRed, bool bEnableGreen, bool bEnableBlue, bool bEnableAlpha);
	void SetEnableBlend(bool bEnable, uint32_t srcFactor, uint32_t dstFactor);
	void SetEnablePolygonOffset(bool bEnable, float factor, float units);

	bool IsEnableBlend(void) const;

public:
	void BindPipeline(void) const;


private:
	GLstate m_state;
};
