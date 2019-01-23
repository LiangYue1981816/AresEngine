#pragma once
#include "GfxRenderer.h"


typedef struct PipelineState {
	// Input Assembly State
	bool bEnablePrimitiveRestart = false;
	GfxPrimitiveTopology topology = GFX_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

	// Rasterization State
	bool bEnableCullFace = true;
	GfxCullFace cullFace = GFX_CULLFACE_BACK;
	GfxFrontFace frontFace = GFX_FRONTFACE_CCW;

	bool bEnableRasterizerDiscard = false;
	GfxPolygonMode polygonMode = GFX_POLYGON_MODE_FILL;

	bool bEnableDepthBias = false;
	float depthBiasSlopeFactor = 0.0f;
	float depthBiasConstantFactor = 0.0f;

	// Multisample State
	int samples = 1;
	bool bEnableAlphaToCoverage = false;
	bool bEnableAlphaToOne = false;

	// Depth Stencil State
	bool bEnableDepthTest = true;
	bool bEnableDepthWrite = true;
	float depthRangeNear = 0.0f;
	float depthRangeFar = 1.0f;
	GfxFunc depthFunc = GFX_FUNC_LESS;

	bool bEnableStencilTest = false;
	uint32_t stencilRef = 0;
	uint32_t stencilMask = 1;
	GfxFunc stencilFunc = GFX_FUNC_ALWAYS;
	GfxStencilOp stencilOpSFail = GFX_STENCILOP_KEEP;
	GfxStencilOp stencilOpDFail = GFX_STENCILOP_KEEP;
	GfxStencilOp stencilOpDPass = GFX_STENCILOP_KEEP;

	// Color Blend State
	bool bEnableBlend = false;
	float blendColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	GfxBlendFactor blendSrcFactor = GFX_BLENDFACTOR_ONE;
	GfxBlendFactor blendDstFactor = GFX_BLENDFACTOR_ZERO;
	GfxBlendEquation blendEquation = GFX_BLENDEQUATION_FUNC_ADD;

	bool bEnableRedWrite = true;
	bool bEnableGreenWrite = true;
	bool bEnableBlueWrite = true;
	bool bEnableAlphaWrite = true;
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
	virtual bool Create(const CGfxRenderPass *pRenderPass, const CGfxShader *pVertexShader, const CGfxShader *pFragmentShader, const PipelineState &state, uint32_t indexSubpass, uint32_t vertexBinding, uint32_t instanceBinding) = 0;
	virtual void Destroy(void) = 0;

public:
	virtual bool IsTextureValid(uint32_t name) const = 0;
	virtual bool IsUniformValid(uint32_t name) const = 0;
	virtual bool IsUniformBlockValid(uint32_t name) const = 0;
};
