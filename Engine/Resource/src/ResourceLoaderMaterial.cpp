#include "ResourceLoader.h"

#include "tinyxml.h"
#include "tinystr.h"
#include "Stream.h"
#include "FileManager.h"


static bool StringToBool(const char *szString)
{
	if (szString) {
		if (!stricmp(szString, "GFX_TRUE")) return true;
		if (!stricmp(szString, "GFX_FALSE")) return false;
	}

	return false;
}

static GfxPrimitiveTopology StringToTopology(const char *szString)
{
	if (szString) {
		if (!stricmp(szString, "GFX_POINT_LIST")) return GFX_PRIMITIVE_TOPOLOGY_POINT_LIST;
		if (!stricmp(szString, "GFX_LINE_LIST")) return GFX_PRIMITIVE_TOPOLOGY_LINE_LIST;
		if (!stricmp(szString, "GFX_LINE_STRIP")) return GFX_PRIMITIVE_TOPOLOGY_LINE_STRIP;
		if (!stricmp(szString, "GFX_TRIANGLE_LIST")) return GFX_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		if (!stricmp(szString, "GFX_TRIANGLE_STRIP")) return GFX_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
		if (!stricmp(szString, "GFX_TRIANGLE_FAN")) return GFX_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;
	}

	return GFX_PRIMITIVE_TOPOLOGY_INVALID_ENUM;
}

static GfxCullFace StringToCullFace(const char *szString)
{
	if (szString) {
		if (!stricmp(szString, "GFX_FRONT")) return GFX_CULLFACE_FRONT;
		if (!stricmp(szString, "GFX_BACK")) return GFX_CULLFACE_BACK;
		if (!stricmp(szString, "GFX_FRONT_AND_BACK")) return GFX_CULLFACE_FRONT_AND_BACK;
	}

	return GFX_CULLFACE_INVALID_ENUM;
}

static GfxFrontFace StringToFrontFace(const char *szString)
{
	if (szString) {
		if (!stricmp(szString, "GFX_CW")) return GFX_FRONTFACE_CW;
		if (!stricmp(szString, "GFX_CCW")) return GFX_FRONTFACE_CCW;
	}

	return GFX_FRONTFACE_INVALID_ENUM;
}

static GfxCompareOp StringToCompareOp(const char *szString)
{
	if (szString) {
		if (!stricmp(szString, "GFX_COMPAREOP_NEVER")) return GFX_COMPAREOP_NEVER;
		if (!stricmp(szString, "GFX_COMPAREOP_LESS")) return GFX_COMPAREOP_LESS;
		if (!stricmp(szString, "GFX_COMPAREOP_LEQUAL")) return GFX_COMPAREOP_LEQUAL;
		if (!stricmp(szString, "GFX_COMPAREOP_GREATER")) return GFX_COMPAREOP_GREATER;
		if (!stricmp(szString, "GFX_COMPAREOP_GEQUAL")) return GFX_COMPAREOP_GEQUAL;
		if (!stricmp(szString, "GFX_COMPAREOP_EQUAL")) return GFX_COMPAREOP_EQUAL;
		if (!stricmp(szString, "GFX_COMPAREOP_NOTEQUAL")) return GFX_COMPAREOP_NOTEQUAL;
		if (!stricmp(szString, "GFX_COMPAREOP_ALWAYS")) return GFX_COMPAREOP_ALWAYS;
	}

	return GFX_COMPAREOP_INVALID_ENUM;
}

static GfxStencilOp StringToStencilOp(const char *szString)
{
	if (szString) {
		if (!stricmp(szString, "GFX_KEEP")) return GFX_STENCILOP_KEEP;
		if (!stricmp(szString, "GFX_ZERO")) return GFX_STENCILOP_ZERO;
		if (!stricmp(szString, "GFX_REPLACE")) return GFX_STENCILOP_REPLACE;
		if (!stricmp(szString, "GFX_INCR")) return GFX_STENCILOP_INCR;
		if (!stricmp(szString, "GFX_INCR_WRAP")) return GFX_STENCILOP_INCR_WRAP;
		if (!stricmp(szString, "GFX_DECR")) return GFX_STENCILOP_DECR;
		if (!stricmp(szString, "GFX_DECR_WRAP")) return GFX_STENCILOP_DECR_WRAP;
		if (!stricmp(szString, "GFX_INVERT")) return GFX_STENCILOP_INVERT;
	}

	return GFX_STENCILOP_INVALID_ENUM;
}

static GfxBlendFactor StringToBlendFactor(const char *szString)
{
	if (szString) {
		if (!stricmp(szString, "GFX_ZERO")) return GFX_BLENDFACTOR_ZERO;
		if (!stricmp(szString, "GFX_ONE")) return GFX_BLENDFACTOR_ONE;
		if (!stricmp(szString, "GFX_SRC_COLOR")) return GFX_BLENDFACTOR_SRC_COLOR;
		if (!stricmp(szString, "GFX_ONE_MINUS_SRC_COLOR")) return GFX_BLENDFACTOR_ONE_MINUS_SRC_COLOR;
		if (!stricmp(szString, "GFX_DST_COLOR")) return GFX_BLENDFACTOR_DST_COLOR;
		if (!stricmp(szString, "GFX_ONE_MINUS_DST_COLOR")) return GFX_BLENDFACTOR_ONE_MINUS_DST_COLOR;
		if (!stricmp(szString, "GFX_SRC_ALPHA")) return GFX_BLENDFACTOR_SRC_ALPHA;
		if (!stricmp(szString, "GFX_ONE_MINUS_SRC_ALPHA")) return GFX_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
		if (!stricmp(szString, "GFX_DST_ALPHA")) return GFX_BLENDFACTOR_DST_ALPHA;
		if (!stricmp(szString, "GFX_ONE_MINUS_DST_ALPHA")) return GFX_BLENDFACTOR_ONE_MINUS_DST_ALPHA;
		if (!stricmp(szString, "GFX_CONSTANT_COLOR")) return GFX_BLENDFACTOR_CONSTANT_COLOR;
		if (!stricmp(szString, "GFX_ONE_MINUS_CONSTANT_COLOR")) return GFX_BLENDFACTOR_ONE_MINUS_CONSTANT_COLOR;
		if (!stricmp(szString, "GFX_CONSTANT_ALPHA")) return GFX_BLENDFACTOR_CONSTANT_ALPHA;
		if (!stricmp(szString, "GFX_ONE_MINUS_CONSTANT_ALPHA")) return GFX_BLENDFACTOR_ONE_MINUS_CONSTANT_ALPHA;
		if (!stricmp(szString, "GFX_SRC_ALPHA_SATURATE")) return GFX_BLENDFACTOR_SRC_ALPHA_SATURATE;
	}

	return GFX_BLENDFACTOR_INVALID_ENUM;
}

static GfxBlendOp StringToBlendOp(const char *szString)
{
	if (szString) {
		if (!stricmp(szString, "GFX_ADD")) return GFX_BLENDOP_ADD;
		if (!stricmp(szString, "GFX_SUBTRACT")) return GFX_BLENDOP_SUBTRACT;
		if (!stricmp(szString, "GFX_REVERSE_SUBTRACT")) return GFX_BLENDOP_REVERSE_SUBTRACT;
		if (!stricmp(szString, "GFX_MIN")) return GFX_BLENDOP_MIN;
		if (!stricmp(szString, "GFX_MAX")) return GFX_BLENDOP_MAX;
	}

	return GFX_BLENDOP_INVALID_ENUM;
}

static GfxFilter StringToFilter(const char *szString)
{
	if (szString) {
		if (!stricmp(szString, "GFX_NEAREST")) return GFX_FILTER_NEAREST;
		if (!stricmp(szString, "GFX_LINEAR")) return GFX_FILTER_LINEAR;
	}

	return GFX_FILTER_INVALID_ENUM;
}

static GfxSamplerMipmapMode StringToMipmapMode(const char *szString)
{
	if (szString) {
		if (!stricmp(szString, "GFX_NEAREST")) return GFX_SAMPLER_MIPMAP_MODE_NEAREST;
		if (!stricmp(szString, "GFX_LINEAR")) return GFX_SAMPLER_MIPMAP_MODE_LINEAR;
	}

	return GFX_SAMPLER_MIPMAP_MODE_INVALID_ENUM;
}

static GfxSamplerAddressMode StringToAddressMode(const char *szString)
{
	if (szString) {
		if (!stricmp(szString, "GFX_REPEAT")) return GFX_SAMPLER_ADDRESS_MODE_REPEAT;
		if (!stricmp(szString, "GFX_MIRRORED_REPEAT")) return GFX_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
		if (!stricmp(szString, "GFX_CLAMP_TO_EDGE")) return GFX_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		if (!stricmp(szString, "GFX_CLAMP_TO_BORDER")) return GFX_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
	}

	return GFX_SAMPLER_ADDRESS_MODE_INVALID_ENUM;
}


static bool InternalLoadPipelineState(TiXmlNode *pPipelineNode, PipelineState &state)
{
	if (TiXmlNode *pStateNode = pPipelineNode->FirstChild("State")) {
		LogOutput(LOG_TAG_RENDERER, "\t\t\tLoadState ... ");
		{
			if (TiXmlNode *pInputAssemblyNode = pStateNode->FirstChild("InputAssembly")) {
				if (TiXmlNode *pPrimitiveRestart = pInputAssemblyNode->FirstChild("PrimitiveRestart")) {
					state.bEnablePrimitiveRestart = StringToBool(pPrimitiveRestart->ToElement()->AttributeString("enable"));
				}
				if (TiXmlNode *pPrimitiveTopology = pInputAssemblyNode->FirstChild("PrimitiveTopology")) {
					state.topology = StringToTopology(pPrimitiveTopology->ToElement()->AttributeString("topology"));
				}
			}

			if (TiXmlNode *pRasterizationNode = pStateNode->FirstChild("Rasterization")) {
				if (TiXmlNode *pCullNode = pRasterizationNode->FirstChild("Cull")) {
					state.bEnableCullFace = StringToBool(pCullNode->ToElement()->AttributeString("enable"));
					state.cullFace = StringToCullFace(pCullNode->ToElement()->AttributeString("cull_face"));
					state.frontFace = StringToFrontFace(pCullNode->ToElement()->AttributeString("front_face"));
				}
				if (TiXmlNode *pDepthBiasNode = pRasterizationNode->FirstChild("DepthBias")) {
					state.bEnableDepthBias = StringToBool(pDepthBiasNode->ToElement()->AttributeString("enable"));
					state.depthBiasSlopeFactor = pDepthBiasNode->ToElement()->AttributeFloat1("slope_factor");
					state.depthBiasConstantFactor = pDepthBiasNode->ToElement()->AttributeFloat1("constant_factor");
				}
			}

			if (TiXmlNode *pMultisampleNode = pStateNode->FirstChild("Multisample")) {
				if (TiXmlNode *pSampleNode = pMultisampleNode->FirstChild("Sample")) {
					state.samples = pSampleNode->ToElement()->AttributeInt1("count");
				}
				if (TiXmlNode *pAlphaToCoverageNode = pMultisampleNode->FirstChild("AlphaToCoverage")) {
					state.bEnableAlphaToCoverage = StringToBool(pAlphaToCoverageNode->ToElement()->AttributeString("enable"));
				}
			}

			if (TiXmlNode *pDepthNode = pStateNode->FirstChild("Depth")) {
				if (TiXmlNode *pDepthTestNode = pDepthNode->FirstChild("DepthTest")) {
					state.bEnableDepthTest = StringToBool(pDepthTestNode->ToElement()->AttributeString("enable"));
					state.depthCompareOp = StringToCompareOp(pDepthTestNode->ToElement()->AttributeString("compare_op"));
				}
				if (TiXmlNode *pDepthWriteNode = pDepthNode->FirstChild("DepthWrite")) {
					state.bEnableDepthWrite = StringToBool(pDepthWriteNode->ToElement()->AttributeString("enable"));
				}
			}

			if (TiXmlNode *pStencilNode = pStateNode->FirstChild("Stencil")) {
				if (TiXmlNode *pStencilTestNode = pStencilNode->FirstChild("StencilTest")) {
					state.bEnableStencilTest = StringToBool(pStencilTestNode->ToElement()->AttributeString("enable"));
				}
				if (TiXmlNode *pFrontNode = pStencilNode->FirstChild("Front")) {
					state.stencilFrontWriteMask = pFrontNode->ToElement()->AttributeInt1("write_mask");
					state.stencilFrontCompareMask = pFrontNode->ToElement()->AttributeInt1("compare_mask");
					state.stencilFrontCompareRef = pFrontNode->ToElement()->AttributeInt1("compare_ref");
					state.stencilFrontCompareOp = StringToCompareOp(pFrontNode->ToElement()->AttributeString("compare_op"));
					state.stencilFrontOpSFail = StringToStencilOp(pFrontNode->ToElement()->AttributeString("stencil_fail"));
					state.stencilFrontOpDFail = StringToStencilOp(pFrontNode->ToElement()->AttributeString("depth_fail"));
					state.stencilFrontOpDPass = StringToStencilOp(pFrontNode->ToElement()->AttributeString("pass"));
				}
				if (TiXmlNode *pBackNode = pStencilNode->FirstChild("Back")) {
					state.stencilBackWriteMask = pBackNode->ToElement()->AttributeInt1("write_mask");
					state.stencilBackCompareMask = pBackNode->ToElement()->AttributeInt1("compare_mask");
					state.stencilBackCompareRef = pBackNode->ToElement()->AttributeInt1("compare_ref");
					state.stencilBackCompareOp = StringToCompareOp(pBackNode->ToElement()->AttributeString("compare_op"));
					state.stencilBackOpSFail = StringToStencilOp(pBackNode->ToElement()->AttributeString("stencil_fail"));
					state.stencilBackOpDFail = StringToStencilOp(pBackNode->ToElement()->AttributeString("depth_fail"));
					state.stencilBackOpDPass = StringToStencilOp(pBackNode->ToElement()->AttributeString("pass"));
				}
			}

			if (TiXmlNode *pColorBlendNode = pStateNode->FirstChild("ColorBlend")) {
				if (TiXmlNode *pBlendNode = pColorBlendNode->FirstChild("Blend")) {
					state.bEnableBlend = StringToBool(pBlendNode->ToElement()->AttributeString("enable"));
				}
				if (TiXmlNode *pConstantNode = pColorBlendNode->FirstChild("Constant")) {
					state.blendColorRed = pConstantNode->ToElement()->AttributeFloat1("red");
					state.blendColorGreen = pConstantNode->ToElement()->AttributeFloat1("green");
					state.blendColorBlue = pConstantNode->ToElement()->AttributeFloat1("blue");
					state.blendColorAlpha = pConstantNode->ToElement()->AttributeFloat1("alpha");
				}
				if (TiXmlNode *pRGBBlendNode = pColorBlendNode->FirstChild("RGBBlend")) {
					state.blendOpRGB = StringToBlendOp(pRGBBlendNode->ToElement()->AttributeString("blend_op"));
					state.blendSrcRGB = StringToBlendFactor(pRGBBlendNode->ToElement()->AttributeString("src_factor"));
					state.blendDstRGB = StringToBlendFactor(pRGBBlendNode->ToElement()->AttributeString("dst_factor"));
				}
				if (TiXmlNode *pAlphaBlendNode = pColorBlendNode->FirstChild("AlphaBlend")) {
					state.blendOpAlpha = StringToBlendOp(pAlphaBlendNode->ToElement()->AttributeString("blend_op"));
					state.blendSrcAlpha = StringToBlendFactor(pAlphaBlendNode->ToElement()->AttributeString("src_factor"));
					state.blendDstAlpha = StringToBlendFactor(pAlphaBlendNode->ToElement()->AttributeString("dst_factor"));
				}
				if (TiXmlNode *pColorMaskNode = pColorBlendNode->FirstChild("ColorMask")) {
					state.bEnableColorRedWrite = StringToBool(pColorMaskNode->ToElement()->AttributeString("enable_write_red"));
					state.bEnableColorGreenWrite = StringToBool(pColorMaskNode->ToElement()->AttributeString("enable_write_green"));
					state.bEnableColorBlueWrite = StringToBool(pColorMaskNode->ToElement()->AttributeString("enable_write_blue"));
					state.bEnableColorAlphaWrite = StringToBool(pColorMaskNode->ToElement()->AttributeString("enable_write_alpha"));
				}
			}
		}
		LogOutput(nullptr, "OK\n");
	}
	return true;
}

static bool InternalLoadPipelineShader(TiXmlNode *pPipelineNode, CGfxShader *&pShader, shader_kind kind)
{
	int err = 0;

	char szShaderKind[2][_MAX_STRING] = { "Vertex", "Fragment" };
	LogOutput(LOG_TAG_RENDERER, "\t\t\tLoad%sShader ", szShaderKind[kind]);
	{
		TiXmlNode *pShaderNode = pPipelineNode->FirstChild(szShaderKind[kind]);
		if (pShaderNode == nullptr) { err = -1; goto ERR; }

		const char *szFileName = pShaderNode->ToElement()->AttributeString("file_name");
		if (szFileName == nullptr) { err = -2; goto ERR; }

		LogOutput(nullptr, "%s ... ", szFileName);

		char szHashName[_MAX_STRING] = { 0 };
		strcat(szHashName, szFileName);

#ifdef PLATFORM_WINDOWS
		ShaderCompiler()->ClearMacroDefinition();
#endif
		{
			if (TiXmlNode *pDefineNode = pShaderNode->FirstChild("Define")) {
				do {
					if (const char *szDefine = pDefineNode->ToElement()->AttributeString("name")) {
						strcat(szHashName, "_");
						strcat(szHashName, szDefine);

#ifdef PLATFORM_WINDOWS
						ShaderCompiler()->AddMacroDefinition(szDefine);
#endif
					}
				} while ((pDefineNode = pShaderNode->IterateChildren("Define", pDefineNode)) != nullptr);
			}
		}

		char szExtName[2][_MAX_STRING] = { "vert", "frag" };
		char szBinFileName[_MAX_STRING] = { 0 };
		sprintf(szBinFileName, "%x.%s", HashValue(szHashName), szExtName[kind]);

#ifdef PLATFORM_WINDOWS
		ShaderCompiler()->Compile(FileManager()->GetFullName(szFileName), szBinFileName, (shaderc_shader_kind)kind);
#endif

		pShader = GfxRenderer()->CreateShader(szBinFileName, kind);
		if (pShader->IsValid() == false) { err = -3; goto ERR; }
	}
	LogOutput(nullptr, "OK\n");
	return true;
ERR:
	LogOutput(nullptr, "Fail(%d)\n", err);
	return false;
}

static bool InternalLoadPipeline(TiXmlNode *pPassNode, CGfxMaterialPass *pPass, uint32_t vertexBinding, uint32_t instanceBinding)
{
	int err = 0;

	LogOutput(LOG_TAG_RENDERER, "\t\tLoadPipeline\n");
	{
		TiXmlNode *pPipelineNode = pPassNode->FirstChild("Pipeline");
		if (pPipelineNode == nullptr) { err = -1; goto ERR; }

		const uint32_t indexSubpass = pPipelineNode->ToElement()->AttributeInt1("sub_pass");
		const char *szRenderPassName = pPipelineNode->ToElement()->AttributeString("render_pass");
		if (szRenderPassName == nullptr) { err = -2; goto ERR; }

		CGfxRenderPassPtr ptrRenderPass = GfxRenderer()->GetRenderPass(HashValue(szRenderPassName));
		if (ptrRenderPass == nullptr) { err = -3; goto ERR; }

		PipelineState state;
		if (InternalLoadPipelineState(pPipelineNode, state) == false) { err = -4; goto ERR; }

		CGfxShader *pVertexShader = nullptr;
		CGfxShader *pFragmentShader = nullptr;
#ifdef PLATFORM_WINDOWS
		InternalLoadPipelineShader(pPipelineNode, pVertexShader, vertex_shader);
		InternalLoadPipelineShader(pPipelineNode, pFragmentShader, fragment_shader);
		pPass->SetPipeline(ptrRenderPass, pVertexShader, pFragmentShader, state, indexSubpass, vertexBinding, instanceBinding);
#else
		if (InternalLoadPipelineShader(pPipelineNode, pVertexShader, vertex_shader) == false) { err = -5; goto ERR; }
		if (InternalLoadPipelineShader(pPipelineNode, pFragmentShader, fragment_shader) == false) { err = -6; goto ERR; }
		if (pPass->SetPipeline(ptrRenderPass, pVertexShader, pFragmentShader, state, indexSubpass, vertexBinding, instanceBinding) == false) { err = -7; goto ERR; }
#endif
	}
	LogOutput(LOG_TAG_RENDERER, "\t\tOK\n");
	return true;
ERR:
	LogOutput(LOG_TAG_RENDERER, "Fail(%d)\n", err);
	return false;
}

static bool InternalLoadTexture2D(TiXmlNode *pPassNode, CGfxMaterialPass *pPass, uint32_t baseLevel)
{
	int err = 0;

	if (TiXmlNode *pTextureNode = pPassNode->FirstChild("Texture2D")) {
		do {
			LogOutput(LOG_TAG_RENDERER, "\t\tLoadTexture2D ");
			{
				const char *szName = pTextureNode->ToElement()->AttributeString("name");
				const char *szFileName = pTextureNode->ToElement()->AttributeString("file_name");
				if (szName == nullptr || szFileName == nullptr) { err = -1; goto ERR; }

				LogOutput(nullptr, "%s ... ", szFileName);

				GfxFilter minFilter = StringToFilter(pTextureNode->ToElement()->AttributeString("min_filter"));
				GfxFilter magFilter = StringToFilter(pTextureNode->ToElement()->AttributeString("mag_filter"));
				GfxSamplerMipmapMode mipmapMode = StringToMipmapMode(pTextureNode->ToElement()->AttributeString("mipmap_mode"));
				GfxSamplerAddressMode addressMode = StringToAddressMode(pTextureNode->ToElement()->AttributeString("address_mode"));
				if (minFilter == GFX_FILTER_INVALID_ENUM || magFilter == GFX_FILTER_INVALID_ENUM || mipmapMode == GFX_SAMPLER_MIPMAP_MODE_INVALID_ENUM || addressMode == GFX_SAMPLER_ADDRESS_MODE_INVALID_ENUM) { err = -2; goto ERR; }

				if (pPass->SetSampler(HashValue(szName), minFilter, magFilter, mipmapMode, addressMode)) {
					pPass->SetTexture2D(HashValue(szName), szFileName, baseLevel);
				}
			}
			LogOutput(nullptr, "OK\n");
		} while ((pTextureNode = pPassNode->IterateChildren("Texture2D", pTextureNode)) != nullptr);
	}
	return true;
ERR:
	LogOutput(nullptr, "Fail(%d)\n", err);
	return false;
}

static bool InternalLoadTexture2DArray(TiXmlNode *pPassNode, CGfxMaterialPass *pPass, uint32_t baseLevel)
{
	int err = 0;

	if (TiXmlNode *pTextureNode = pPassNode->FirstChild("Texture2DArray")) {
		do {
			LogOutput(LOG_TAG_RENDERER, "\t\tLoadTexture2DArray ");
			{
				const char *szName = pTextureNode->ToElement()->AttributeString("name");
				const char *szFileName = pTextureNode->ToElement()->AttributeString("file_name");
				if (szName == nullptr || szFileName == nullptr) { err = -1; goto ERR; }

				LogOutput(nullptr, "%s ... ", szFileName);

				GfxFilter minFilter = StringToFilter(pTextureNode->ToElement()->AttributeString("min_filter"));
				GfxFilter magFilter = StringToFilter(pTextureNode->ToElement()->AttributeString("mag_filter"));
				GfxSamplerMipmapMode mipmapMode = StringToMipmapMode(pTextureNode->ToElement()->AttributeString("mipmap_mode"));
				GfxSamplerAddressMode addressMode = StringToAddressMode(pTextureNode->ToElement()->AttributeString("address_mode"));
				if (minFilter == GFX_FILTER_INVALID_ENUM || magFilter == GFX_FILTER_INVALID_ENUM || mipmapMode == GFX_SAMPLER_MIPMAP_MODE_INVALID_ENUM || addressMode == GFX_SAMPLER_ADDRESS_MODE_INVALID_ENUM) { err = -2; goto ERR; }

				if (pPass->SetSampler(HashValue(szName), minFilter, magFilter, mipmapMode, addressMode)) {
					pPass->SetTexture2DArray(HashValue(szName), szFileName, baseLevel);
				}
			}
			LogOutput(nullptr, "OK\n");
		} while ((pTextureNode = pPassNode->IterateChildren("Texture2DArray", pTextureNode)) != nullptr);
	}
	return true;
ERR:
	LogOutput(nullptr, "Fail(%d)\n", err);
	return false;
}

static bool InternalLoadTextureCubeMap(TiXmlNode *pPassNode, CGfxMaterialPass *pPass, uint32_t baseLevel)
{
	int err = 0;

	if (TiXmlNode *pTextureNode = pPassNode->FirstChild("TextureCubeMap")) {
		do {
			LogOutput(LOG_TAG_RENDERER, "\t\tLoadTextureCubeMap ");
			{
				const char *szName = pTextureNode->ToElement()->AttributeString("name");
				const char *szFileName = pTextureNode->ToElement()->AttributeString("file_name");
				if (szName == nullptr || szFileName == nullptr) { err = -1; goto ERR; }

				LogOutput(nullptr, "%s ... ", szFileName);

				GfxFilter minFilter = StringToFilter(pTextureNode->ToElement()->AttributeString("min_filter"));
				GfxFilter magFilter = StringToFilter(pTextureNode->ToElement()->AttributeString("mag_filter"));
				GfxSamplerMipmapMode mipmapMode = StringToMipmapMode(pTextureNode->ToElement()->AttributeString("mipmap_mode"));
				GfxSamplerAddressMode addressMode = StringToAddressMode(pTextureNode->ToElement()->AttributeString("address_mode"));
				if (minFilter == GFX_FILTER_INVALID_ENUM || magFilter == GFX_FILTER_INVALID_ENUM || mipmapMode == GFX_SAMPLER_MIPMAP_MODE_INVALID_ENUM || addressMode == GFX_SAMPLER_ADDRESS_MODE_INVALID_ENUM) { err = -2; goto ERR; }

				if (pPass->SetSampler(HashValue(szName), minFilter, magFilter, mipmapMode, addressMode)) {
					pPass->SetTextureCubeMap(HashValue(szName), szFileName, baseLevel);
				}
			}
			LogOutput(nullptr, "OK\n");
		} while ((pTextureNode = pPassNode->IterateChildren("TextureCubeMap", pTextureNode)) != nullptr);
	}
	return true;
ERR:
	LogOutput(nullptr, "Fail(%d)\n", err);
	return false;
}

static bool InternalLoadUniformVec1(TiXmlNode *pPassNode, CGfxMaterialPass *pPass)
{
	int err = 0;

	if (TiXmlNode *pUniformNode = pPassNode->FirstChild("Uniform1f")) {
		do {
			LogOutput(LOG_TAG_RENDERER, "\t\tLoadUniformVec1 ");
			{
				const char *szName = pUniformNode->ToElement()->AttributeString("name");
				const char *szValue = pUniformNode->ToElement()->AttributeString("value");
				if (szName == nullptr || szValue == nullptr) { err = -1; goto ERR; }

				LogOutput(nullptr, "%s = %s ", szName, szValue);

				float value[1] = { 0.0f };
				pUniformNode->ToElement()->AttributeFloat1("value", value);

				pPass->SetUniformVec1(HashValue(szName), value[0]);
			}
			LogOutput(nullptr, "OK\n");
		} while ((pUniformNode = pPassNode->IterateChildren("Uniform1f", pUniformNode)) != nullptr);
	}
	return true;
ERR:
	LogOutput(nullptr, "Fail(%d)\n", err);
	return false;
}

static bool InternalLoadUniformVec2(TiXmlNode *pPassNode, CGfxMaterialPass *pPass)
{
	int err = 0;

	if (TiXmlNode *pUniformNode = pPassNode->FirstChild("Uniform2f")) {
		do {
			LogOutput(LOG_TAG_RENDERER, "\t\tLoadUniformVec2 ");
			{
				const char *szName = pUniformNode->ToElement()->AttributeString("name");
				const char *szValue = pUniformNode->ToElement()->AttributeString("value");
				if (szName == nullptr || szValue == nullptr) { err = -1; goto ERR; }

				LogOutput(nullptr, "%s = vec2(%s) ", szName, szValue);

				float value[2] = { 0.0f };
				pUniformNode->ToElement()->AttributeFloat2("value", value);

				pPass->SetUniformVec2(HashValue(szName), value[0], value[1]);
			}
			LogOutput(nullptr, "OK\n");
		} while ((pUniformNode = pPassNode->IterateChildren("Uniform2f", pUniformNode)) != nullptr);
	}
	return true;
ERR:
	LogOutput(nullptr, "Fail(%d)\n", err);
	return false;
}

static bool InternalLoadUniformVec3(TiXmlNode *pPassNode, CGfxMaterialPass *pPass)
{
	int err = 0;

	if (TiXmlNode *pUniformNode = pPassNode->FirstChild("Uniform3f")) {
		do {
			LogOutput(LOG_TAG_RENDERER, "\t\tLoadUniformVec3 ");
			{
				const char *szName = pUniformNode->ToElement()->AttributeString("name");
				const char *szValue = pUniformNode->ToElement()->AttributeString("value");
				if (szName == nullptr || szValue == nullptr) { err = -1; goto ERR; }

				LogOutput(nullptr, "%s = vec3(%s) ", szName, szValue);

				float value[3] = { 0.0f };
				pUniformNode->ToElement()->AttributeFloat3("value", value);

				pPass->SetUniformVec3(HashValue(szName), value[0], value[1], value[2]);
			}
			LogOutput(nullptr, "OK\n");
		} while ((pUniformNode = pPassNode->IterateChildren("Uniform3f", pUniformNode)) != nullptr);
	}
	return true;
ERR:
	LogOutput(nullptr, "Fail(%d)\n", err);
	return false;
}

static bool InternalLoadUniformVec4(TiXmlNode *pPassNode, CGfxMaterialPass *pPass)
{
	int err = 0;

	if (TiXmlNode *pUniformNode = pPassNode->FirstChild("Uniform4f")) {
		do {
			LogOutput(LOG_TAG_RENDERER, "\t\tLoadUniformVec4 ");
			{
				const char *szName = pUniformNode->ToElement()->AttributeString("name");
				const char *szValue = pUniformNode->ToElement()->AttributeString("value");
				if (szName == nullptr || szValue == nullptr) { err = -1; goto ERR; }

				LogOutput(nullptr, "%s = vec4(%s) ", szName, szValue);

				float value[4] = { 0.0f };
				pUniformNode->ToElement()->AttributeFloat4("value", value);

				pPass->SetUniformVec4(HashValue(szName), value[0], value[1], value[2], value[3]);
			}
			LogOutput(nullptr, "OK\n");
		} while ((pUniformNode = pPassNode->IterateChildren("Uniform4f", pUniformNode)) != nullptr);
	}
	return true;
ERR:
	LogOutput(nullptr, "Fail(%d)\n", err);
	return false;
}

static bool InternalLoadPass(TiXmlNode *pPassNode, CGfxMaterialPass *pPass, uint32_t vertexBinding, uint32_t instanceBinding, uint32_t baseLevel)
{
	int err = 0;

	LogOutput(LOG_TAG_RENDERER, "\tLoadPass(%s)\n", pPassNode->ToElement()->AttributeString("name"));
	{
		if (InternalLoadPipeline(pPassNode, pPass, vertexBinding, instanceBinding) == false) { err = -1; goto ERR; }
		if (InternalLoadTexture2D(pPassNode, pPass, baseLevel) == false) { err = -2; goto ERR; }
		if (InternalLoadTexture2DArray(pPassNode, pPass, baseLevel) == false) { err = -3; goto ERR; }
		if (InternalLoadTextureCubeMap(pPassNode, pPass, baseLevel) == false) { err = -4; goto ERR; }
		if (InternalLoadUniformVec1(pPassNode, pPass) == false) { err = -5; goto ERR; }
		if (InternalLoadUniformVec2(pPassNode, pPass) == false) { err = -6; goto ERR; }
		if (InternalLoadUniformVec3(pPassNode, pPass) == false) { err = -7; goto ERR; }
		if (InternalLoadUniformVec4(pPassNode, pPass) == false) { err = -8; goto ERR; }
	}
	LogOutput(LOG_TAG_RENDERER, "\tOK\n");
	return true;
ERR:
	LogOutput(LOG_TAG_RENDERER, "Fail(%d)\n", err);
	return false;
}


bool CResourceLoader::LoadMaterial(const char *szFileName, CGfxMaterial *pMaterial, uint32_t vertexBinding, uint32_t instanceBinding, uint32_t baseLevel)
{
	//<Material>
	//	<Pass name="">
	//		<Pipeline render_pass="" sub_pass="">
	//			<Vertex file_name="">
	//				<Define name="" />
	//			</Vertex>
	//			<Fragment file_name="">
	//				<Define name="" />
	//			</Fragment>
	//			<State>
	//				<InputAssembly>
	//					<PrimitiveRestart enable="" />
	//					<PrimitiveTopology topology="" />
	//				</InputAssembly>
	//				<Rasterization>
	//					<Cull enable="" cull_face="" front_face="" />
	//					<DepthBias enable="" slope_factor="" constant_factor="" />
	//				</Rasterization>
	//				<Multisample>
	//					<Sample count="" />
	//					<AlphaToCoverage enable="" />
	//				</Multisample>
	//				<Depth>
	//					<DepthTest enable="" compare_op="" />
	//					<DepthWrite enable="" />
	//				</Depth>
	//				<Stencil>
	//					<StencilTest enable="" />
	//					<Front write_mask="" compare_mask="" compare_ref="" compare_op="" stencil_fail="" depth_fail="" pass="" />
	//					<Back write_mask="" compare_mask="" compare_ref="" compare_op="" stencil_fail="" depth_fail="" pass="" />
	//				</Stencil>
	//				<ColorBlend>
	//					<Blend enable="" />
	//					<Constant red="" green="" blue="" alpha="" />
	//					<RGBBlend blend_op="" src_factor="" dst_factor="" />
	//					<AlphaBlend blend_op="" src_factor="" dst_factor="" />
	//					<ColorMask enable_write_red="" enable_write_green="" enable_write_blue="" enable_write_alpha="" />
	//				</ColorBlend>
	//			</State>
	//		</Pipeline>
	//		<Texture2D file_name="" name="" min_filter="" mag_filter="" mipmap_mode="" address_mode="" />
	//		<Texture2DArray file_name="" name="" min_filter="" mag_filter="" mipmap_mode="" address_mode="" />
	//		<TextureCubeMap file_name="" name="" min_filter="" mag_filter="" mipmap_mode="" address_mode="" />
	//		<Uniform1f name="" value="" />
	//		<Uniform2f name="" value="" />
	//		<Uniform3f name="" value="" />
	//		<Uniform4f name="" value="" />
	//	</Pass>
	//</Material>

	int err = 0;

	pMaterial->Destroy();
	LogOutput(LOG_TAG_RENDERER, "LoadMaterial(%s)\n", szFileName);
	{
		CStream stream;
		if (FileManager()->LoadStream(szFileName, &stream) == false) { err = -1; goto ERR; }

		TiXmlDocument xmlDoc;
		if (xmlDoc.LoadFile((char *)stream.GetAddress(), stream.GetFullSize()) == false) { err = -2; goto ERR; }

		TiXmlNode *pMaterialNode = xmlDoc.FirstChild("Material");
		if (pMaterialNode == nullptr) { err = -3; goto ERR; }

		if (TiXmlNode *pPassNode = pMaterialNode->FirstChild("Pass")) {
			do {
				uint32_t name = HashValue(pPassNode->ToElement()->AttributeString("name"));
				if (pMaterial->CreatePass(name) == false) { err = -4; goto ERR; }
				if (InternalLoadPass(pPassNode, pMaterial->GetPass(name), vertexBinding, instanceBinding, baseLevel) == false) { err = -5; goto ERR; }
			} while ((pPassNode = pMaterialNode->IterateChildren("Pass", pPassNode)) != nullptr);
		}
	}
	LogOutput(LOG_TAG_RENDERER, "OK\n");
	return true;
ERR:
	LogOutput(LOG_TAG_RENDERER, "Fail(%d)\n", err);
	pMaterial->Destroy();
	return false;
}
