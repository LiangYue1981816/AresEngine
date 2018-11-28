#pragma once
#include "GfxRenderer.h"


typedef struct PipelineState {
	uint32_t bEnableCullFace = GL_TRUE;
	uint32_t cullFace = GL_BACK;
	uint32_t frontFace = GL_CCW;

	uint32_t bEnableStencilTest = GL_FALSE;
	uint32_t stencilFunc = GL_ALWAYS;
	uint32_t stencilRef = 0;
	uint32_t stencilMask = 1;
	uint32_t stencilOpSFail = GL_KEEP;
	uint32_t stencilOpDFail = GL_KEEP;
	uint32_t stencilOpDPass = GL_KEEP;

	uint32_t bEnableDepthTest = GL_TRUE;
	uint32_t bEnableDepthWrite = GL_TRUE;
	uint32_t depthFunc = GL_LESS;
	float depthRangeNear = 0.0f;
	float depthRangeFar = 1.0f;

	uint32_t bEnableColorWrite[4] = { GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE };

	uint32_t bEnableAlphaToCoverage = GL_FALSE;
	uint32_t bEnableBlend = GL_FALSE;
	uint32_t blendSrcFactor = GL_ONE;
	uint32_t blendDstFactor = GL_ZERO;
	uint32_t blendEquation = GL_FUNC_ADD;
	float blendColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	uint32_t bEnablePolygonOffset = GL_FALSE;
	float polygonOffsetFactor = 0.0f;
	float polygonOffsetUnits = 0.0f;
} PipelineState;

class CALL_API CGfxPipelineGraphics : public CGfxPipelineBase
{
public:
	CGfxPipelineGraphics(uint32_t name)
		: CGfxPipelineBase(name)
	{

	}
	virtual ~CGfxPipelineGraphics(void)
	{

	}


public:
	virtual bool Create(const CGfxShader *pVertexShader, const CGfxShader *pFragmentShader, const PipelineState &state) = 0;
	virtual void Destroy(void) = 0;

public:
	virtual void Bind(void *pParam) = 0;
};
