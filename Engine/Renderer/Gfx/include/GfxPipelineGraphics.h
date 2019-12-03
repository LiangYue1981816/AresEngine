#pragma once
#include "PreHeader.h"


typedef struct PipelineState {
	// Input Assembly State
	bool bEnablePrimitiveRestart = false;
	GfxPrimitiveTopology topology = GFX_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

	// Rasterization State
	bool bEnableRasterizerDiscard = false;

	bool bEnableCullFace = true;
	GfxCullFace cullFace = GFX_CULLFACE_BACK;
	GfxFrontFace frontFace = GFX_FRONTFACE_CCW;

	bool bEnableDepthBias = false;
	float depthBiasSlopeFactor = 0.0f;
	float depthBiasConstantFactor = 0.0f;

	GfxPolygonMode polygonMode = GFX_POLYGON_MODE_FILL;

	// Multisample State
	int samples = 1;
	bool bEnableAlphaToCoverage = false;
	bool bEnableAlphaToOne = false;

	// Depth Stencil State
	bool bEnableDepthTest = true;
	bool bEnableDepthWrite = true;
	GfxCompareOp depthCompareOp = GFX_COMPAREOP_LEQUAL;

	bool bEnableStencilTest = false;
	uint32_t stencilFrontCompareRef = 0;
	uint32_t stencilFrontCompareMask = 0xFFFFFFFF;
	GfxCompareOp stencilFrontCompareOp = GFX_COMPAREOP_ALWAYS;
	GfxStencilOp stencilFrontOpSFail = GFX_STENCILOP_KEEP;
	GfxStencilOp stencilFrontOpDFail = GFX_STENCILOP_KEEP;
	GfxStencilOp stencilFrontOpDPass = GFX_STENCILOP_KEEP;
	uint32_t stencilFrontWriteMask = 0xFFFFFFFF;

	uint32_t stencilBackCompareRef = 0;
	uint32_t stencilBackCompareMask = 0xFFFFFFFF;
	GfxCompareOp stencilBackCompareOp = GFX_COMPAREOP_ALWAYS;
	GfxStencilOp stencilBackOpSFail = GFX_STENCILOP_KEEP;
	GfxStencilOp stencilBackOpDFail = GFX_STENCILOP_KEEP;
	GfxStencilOp stencilBackOpDPass = GFX_STENCILOP_KEEP;
	uint32_t stencilBackWriteMask = 0xFFFFFFFF;

	// Color Blend State
	bool bEnableBlend = false;
	float blendColorRed = 0.0f;
	float blendColorGreen = 0.0f;
	float blendColorBlue = 0.0f;
	float blendColorAlpha = 0.0f;
	GfxBlendOp blendOpRGB = GFX_BLENDOP_ADD;
	GfxBlendFactor blendSrcRGB = GFX_BLENDFACTOR_ONE;
	GfxBlendFactor blendDstRGB = GFX_BLENDFACTOR_ZERO;
	GfxBlendOp blendOpAlpha = GFX_BLENDOP_ADD;
	GfxBlendFactor blendSrcAlpha = GFX_BLENDFACTOR_ONE;
	GfxBlendFactor blendDstAlpha = GFX_BLENDFACTOR_ZERO;

	bool bEnableColorRedWrite = true;
	bool bEnableColorGreenWrite = true;
	bool bEnableColorBlueWrite = true;
	bool bEnableColorAlphaWrite = true;
} PipelineState;

class CALL_API CGfxPipelineGraphics
{
public:
	CGfxPipelineGraphics(uint32_t name)
		: m_name(name)
	{

	}
	virtual ~CGfxPipelineGraphics(void)
	{

	}

	uint32_t GetName(void) const
	{
		return m_name;
	}


public:
	virtual const CGfxDescriptorLayoutPtr GetDescriptorLayout(int indexDescriptorSet) const = 0;

public:
	virtual bool Create(const CGfxRenderPass* pRenderPass, const CGfxShader* pVertexShader, const CGfxShader* pFragmentShader, const PipelineState& state, int indexSubpass, int vertexBinding, int instanceBinding) = 0;
	virtual void Destroy(void) = 0;

public:
	virtual bool IsTransparency(void) const = 0;


private:
	uint32_t m_name;
};
