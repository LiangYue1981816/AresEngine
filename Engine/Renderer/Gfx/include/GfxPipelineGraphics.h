#pragma once
#include "GfxRenderer.h"


typedef struct PipelineState {
	bool bEnableCullFace = true;
	GfxCullFace cullFace = GFX_CULLFACE_BACK;
	GfxFrontFace frontFace = GFX_FRONTFACE_CCW;

	bool bEnableStencilTest = false;
	uint32_t stencilRef = 0;
	uint32_t stencilMask = 1;
	GfxFunc stencilFunc = GFX_FUNC_ALWAYS;
	GfxStencilOp stencilOpSFail = GFX_STENCILOP_KEEP;
	GfxStencilOp stencilOpDFail = GFX_STENCILOP_KEEP;
	GfxStencilOp stencilOpDPass = GFX_STENCILOP_KEEP;

	bool bEnableDepthTest = true;
	bool bEnableDepthWrite = true;
	float depthRangeNear = 0.0f;
	float depthRangeFar = 1.0f;
	GfxFunc depthFunc = GFX_FUNC_LESS;

	bool bEnableColorWrite[4] = { true, true, true, true };

	bool bEnableAlphaToCoverage = false;
	bool bEnableBlend = false;
	float blendColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	GfxBlendFactor blendSrcFactor = GFX_BLENDFACTOR_ONE;
	GfxBlendFactor blendDstFactor = GFX_BLENDFACTOR_ZERO;
	GfxBlendEquation blendEquation = GFX_BLENDEQUATION_FUNC_ADD;

	bool bEnablePolygonOffset = false;
	float polygonOffsetFactor = 0.0f;
	float polygonOffsetUnits = 0.0f;
} PipelineState;


class CALL_API CGfxPipelineGraphics
{
public:
	CGfxPipelineGraphics(uint32_t name)
	{

	}
	virtual ~CGfxPipelineGraphics(void)
	{

	}


public:
	virtual uint32_t GetName(void) const = 0;

public:
	virtual bool Create(const CGfxShader *pVertexShader, const CGfxShader *pFragmentShader, const PipelineState &state) = 0;
	virtual void Destroy(void) = 0;

public:
	virtual bool IsTextureValid(uint32_t name) const = 0;
	virtual bool IsUniformValid(uint32_t name) const = 0;
	virtual bool IsUniformBlockValid(uint32_t name) const = 0;
};
