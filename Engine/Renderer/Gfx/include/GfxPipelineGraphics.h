#pragma once
#include "PreHeader.h"


typedef struct PipelineState {
	PipelineState(void)
	{
		// Input Assembly State
		bEnablePrimitiveRestart = false;
		topology = GFX_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

		// Rasterization State
		bEnableDepthClamp = false;
		bEnableRasterizerDiscard = false;

		bEnableCullFace = true;
		cullFace = GFX_CULLFACE_BACK;
		frontFace = GFX_FRONTFACE_CCW;

		bEnableDepthBias = false;
		depthBiasSlopeFactor = 0.0f;
		depthBiasConstantFactor = 0.0f;

		polygonMode = GFX_POLYGON_MODE_FILL;

		// Multisample State
		samples = 1;
		bEnableAlphaToCoverage = false;
		bEnableAlphaToOne = false;

		// Depth Stencil State
		bEnableDepthTest = true;
		bEnableDepthWrite = true;
		depthCompareOp = GFX_COMPAREOP_LEQUAL;

		bEnableStencilTest = false;
		stencilFrontCompareRef = 0;
		stencilFrontCompareMask = 0xFFFFFFFF;
		stencilFrontCompareOp = GFX_COMPAREOP_ALWAYS;
		stencilFrontOpSFail = GFX_STENCILOP_KEEP;
		stencilFrontOpDFail = GFX_STENCILOP_KEEP;
		stencilFrontOpDPass = GFX_STENCILOP_KEEP;
		stencilFrontWriteMask = 0xFFFFFFFF;

		stencilBackCompareRef = 0;
		stencilBackCompareMask = 0xFFFFFFFF;
		stencilBackCompareOp = GFX_COMPAREOP_ALWAYS;
		stencilBackOpSFail = GFX_STENCILOP_KEEP;
		stencilBackOpDFail = GFX_STENCILOP_KEEP;
		stencilBackOpDPass = GFX_STENCILOP_KEEP;
		stencilBackWriteMask = 0xFFFFFFFF;

		// Color Blend State
		for (int index = 0; index < 4; index++) {
			bEnableBlend[index] = false;
			bEnableColorRedWrite[index] = true;
			bEnableColorGreenWrite[index] = true;
			bEnableColorBlueWrite[index] = true;
			bEnableColorAlphaWrite[index] = true;
			blendOpRGB[index] = GFX_BLENDOP_ADD;
			blendSrcRGB[index] = GFX_BLENDFACTOR_ONE;
			blendDstRGB[index] = GFX_BLENDFACTOR_ZERO;
			blendOpAlpha[index] = GFX_BLENDOP_ADD;
			blendSrcAlpha[index] = GFX_BLENDFACTOR_ONE;
			blendDstAlpha[index] = GFX_BLENDFACTOR_ZERO;
		}

		blendColorRed = 0.0f;
		blendColorGreen = 0.0f;
		blendColorBlue = 0.0f;
		blendColorAlpha = 0.0f;

	}

	// Input Assembly State
	bool bEnablePrimitiveRestart;
	GfxPrimitiveTopology topology;

	// Rasterization State
	bool bEnableDepthClamp;
	bool bEnableRasterizerDiscard;

	bool bEnableCullFace;
	GfxCullFace cullFace;
	GfxFrontFace frontFace;

	bool bEnableDepthBias;
	float depthBiasSlopeFactor;
	float depthBiasConstantFactor;

	GfxPolygonMode polygonMode;

	// Multisample State
	int samples;
	bool bEnableAlphaToCoverage;
	bool bEnableAlphaToOne;

	// Depth Stencil State
	bool bEnableDepthTest;
	bool bEnableDepthWrite;
	GfxCompareOp depthCompareOp;

	bool bEnableStencilTest;
	uint32_t stencilFrontCompareRef;
	uint32_t stencilFrontCompareMask;
	GfxCompareOp stencilFrontCompareOp;
	GfxStencilOp stencilFrontOpSFail;
	GfxStencilOp stencilFrontOpDFail;
	GfxStencilOp stencilFrontOpDPass;
	uint32_t stencilFrontWriteMask;

	uint32_t stencilBackCompareRef;
	uint32_t stencilBackCompareMask;
	GfxCompareOp stencilBackCompareOp;
	GfxStencilOp stencilBackOpSFail;
	GfxStencilOp stencilBackOpDFail;
	GfxStencilOp stencilBackOpDPass;
	uint32_t stencilBackWriteMask;

	// Color Blend State
	bool bEnableBlend[4];
	bool bEnableColorRedWrite[4];
	bool bEnableColorGreenWrite[4];
	bool bEnableColorBlueWrite[4];
	bool bEnableColorAlphaWrite[4];
	GfxBlendOp blendOpRGB[4];
	GfxBlendFactor blendSrcRGB[4];
	GfxBlendFactor blendDstRGB[4];
	GfxBlendOp blendOpAlpha[4];
	GfxBlendFactor blendSrcAlpha[4];
	GfxBlendFactor blendDstAlpha[4];
	float blendColorRed;
	float blendColorGreen;
	float blendColorBlue;
	float blendColorAlpha;

} PipelineState;

class CGfxPipelineGraphics
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
	virtual bool IsCompatibleVertexFormat(uint32_t binding, uint32_t format) const = 0;
	virtual bool IsTransparency(void) const = 0;


private:
	uint32_t m_name;
};
