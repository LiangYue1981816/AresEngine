#include "GfxHeader.h"

#include "tinyxml.h"
#include "tinystr.h"
#include "Stream.h"
#include "FileManager.h"


static uint32_t StringToCullFace(const char *szString)
{
	if (szString) {
		if (!stricmp(szString, "GL_FRONT")) return GL_FRONT;
		if (!stricmp(szString, "GL_BACK")) return GL_BACK;
		if (!stricmp(szString, "GL_FRONT_AND_BACK")) return GL_FRONT_AND_BACK;
	}

	return GL_BACK;
}

static uint32_t StringToFrontFace(const char *szString)
{
	if (szString) {
		if (!stricmp(szString, "GL_CW")) return GL_CW;
		if (!stricmp(szString, "GL_CCW")) return GL_CCW;
	}

	return GL_CCW;
}

static uint32_t StringToStencilFunc(const char *szString)
{
	if (szString) {
		if (!stricmp(szString, "GL_NEVER")) return GL_NEVER;
		if (!stricmp(szString, "GL_LESS")) return GL_LESS;
		if (!stricmp(szString, "GL_LEQUAL")) return GL_LEQUAL;
		if (!stricmp(szString, "GL_GREATER")) return GL_GREATER;
		if (!stricmp(szString, "GL_GEQUAL")) return GL_GEQUAL;
		if (!stricmp(szString, "GL_EQUAL")) return GL_EQUAL;
		if (!stricmp(szString, "GL_NOTEQUAL")) return GL_NOTEQUAL;
		if (!stricmp(szString, "GL_ALWAYS")) return GL_ALWAYS;
	}

	return GL_ALWAYS;
}

static uint32_t StringToStencilOp(const char *szString)
{
	if (szString) {
		if (!stricmp(szString, "GL_KEEP")) return GL_KEEP;
		if (!stricmp(szString, "GL_ZERO")) return GL_ZERO;
		if (!stricmp(szString, "GL_REPLACE")) return GL_REPLACE;
		if (!stricmp(szString, "GL_INCR")) return GL_INCR;
		if (!stricmp(szString, "GL_INCR_WRAP")) return GL_INCR_WRAP;
		if (!stricmp(szString, "GL_DECR")) return GL_DECR;
		if (!stricmp(szString, "GL_DECR_WRAP")) return GL_DECR_WRAP;
		if (!stricmp(szString, "GL_INVERT")) return GL_INVERT;
	}

	return GL_KEEP;
}

static uint32_t StringToDepthFunc(const char *szString)
{
	if (szString) {
		if (!stricmp(szString, "GL_NEVER")) return GL_NEVER;
		if (!stricmp(szString, "GL_LESS")) return GL_LESS;
		if (!stricmp(szString, "GL_EQUAL")) return GL_EQUAL;
		if (!stricmp(szString, "GL_LEQUAL")) return GL_LEQUAL;
		if (!stricmp(szString, "GL_GREATER")) return GL_GREATER;
		if (!stricmp(szString, "GL_NOTEQUAL")) return GL_NOTEQUAL;
		if (!stricmp(szString, "GL_GEQUAL")) return GL_GEQUAL;
	}

	return GL_LESS;
}

static uint32_t StringToMinFilter(const char *szString)
{
	if (szString) {
		if (!stricmp(szString, "GL_LINEAR")) return GL_LINEAR;
		if (!stricmp(szString, "GL_LINEAR_MIPMAP_LINEAR")) return GL_LINEAR_MIPMAP_LINEAR;
		if (!stricmp(szString, "GL_LINEAR_MIPMAP_NEAREST")) return GL_LINEAR_MIPMAP_NEAREST;
		if (!stricmp(szString, "GL_NEAREST")) return GL_NEAREST;
		if (!stricmp(szString, "GL_NEAREST_MIPMAP_LINEAR")) return GL_NEAREST_MIPMAP_LINEAR;
		if (!stricmp(szString, "GL_NEAREST_MIPMAP_NEAREST")) return GL_NEAREST_MIPMAP_NEAREST;
	}

	return GL_LINEAR_MIPMAP_NEAREST;
}

static uint32_t StringToMagFilter(const char *szString)
{
	if (szString) {
		if (!stricmp(szString, "GL_LINEAR")) return GL_LINEAR;
		if (!stricmp(szString, "GL_NEAREST")) return GL_NEAREST;
	}

	return GL_LINEAR;
}

static uint32_t StringToAddressMode(const char *szString)
{
	if (szString) {
		if (!stricmp(szString, "GL_REPEAT")) return GL_REPEAT;
		if (!stricmp(szString, "GL_CLAMP_TO_EDGE")) return GL_CLAMP_TO_EDGE;
	}

	return GL_REPEAT;
}

static uint32_t StringToBlendSrcFactor(const char *szString)
{
	if (szString) {
		if (!stricmp(szString, "GL_ZERO")) return GL_ZERO;
		if (!stricmp(szString, "GL_ONE")) return GL_ONE;
		if (!stricmp(szString, "GL_SRC_COLOR")) return GL_SRC_COLOR;
		if (!stricmp(szString, "GL_ONE_MINUS_SRC_COLOR")) return GL_ONE_MINUS_SRC_COLOR;
		if (!stricmp(szString, "GL_DST_COLOR")) return GL_DST_COLOR;
		if (!stricmp(szString, "GL_ONE_MINUS_DST_COLOR")) return GL_ONE_MINUS_DST_COLOR;
		if (!stricmp(szString, "GL_SRC_ALPHA")) return GL_SRC_ALPHA;
		if (!stricmp(szString, "GL_ONE_MINUS_SRC_ALPHA")) return GL_ONE_MINUS_SRC_ALPHA;
		if (!stricmp(szString, "GL_DST_ALPHA")) return GL_DST_ALPHA;
		if (!stricmp(szString, "GL_ONE_MINUS_DST_ALPHA")) return GL_ONE_MINUS_DST_ALPHA;
		if (!stricmp(szString, "GL_CONSTANT_COLOR")) return GL_CONSTANT_COLOR;
		if (!stricmp(szString, "GL_ONE_MINUS_CONSTANT_COLOR")) return GL_ONE_MINUS_CONSTANT_COLOR;
		if (!stricmp(szString, "GL_CONSTANT_ALPHA")) return GL_CONSTANT_ALPHA;
		if (!stricmp(szString, "GL_ONE_MINUS_CONSTANT_ALPHA")) return GL_ONE_MINUS_CONSTANT_ALPHA;
		if (!stricmp(szString, "GL_SRC_ALPHA_SATURATE")) return GL_SRC_ALPHA_SATURATE;
	}

	return GL_SRC_ALPHA;
}

static uint32_t StringToBlendDstFactor(const char *szString)
{
	if (szString) {
		if (!stricmp(szString, "GL_ZERO")) return GL_ZERO;
		if (!stricmp(szString, "GL_ONE")) return GL_ONE;
		if (!stricmp(szString, "GL_SRC_COLOR")) return GL_SRC_COLOR;
		if (!stricmp(szString, "GL_ONE_MINUS_SRC_COLOR")) return GL_ONE_MINUS_SRC_COLOR;
		if (!stricmp(szString, "GL_DST_COLOR")) return GL_DST_COLOR;
		if (!stricmp(szString, "GL_ONE_MINUS_DST_COLOR")) return GL_ONE_MINUS_DST_COLOR;
		if (!stricmp(szString, "GL_SRC_ALPHA")) return GL_SRC_ALPHA;
		if (!stricmp(szString, "GL_ONE_MINUS_SRC_ALPHA")) return GL_ONE_MINUS_SRC_ALPHA;
		if (!stricmp(szString, "GL_DST_ALPHA")) return GL_DST_ALPHA;
		if (!stricmp(szString, "GL_ONE_MINUS_DST_ALPHA")) return GL_ONE_MINUS_DST_ALPHA;
		if (!stricmp(szString, "GL_CONSTANT_COLOR")) return GL_CONSTANT_COLOR;
		if (!stricmp(szString, "GL_ONE_MINUS_CONSTANT_COLOR")) return GL_ONE_MINUS_CONSTANT_COLOR;
		if (!stricmp(szString, "GL_CONSTANT_ALPHA")) return GL_CONSTANT_ALPHA;
		if (!stricmp(szString, "GL_ONE_MINUS_CONSTANT_ALPHA")) return GL_ONE_MINUS_CONSTANT_ALPHA;
		if (!stricmp(szString, "GL_SRC_ALPHA_SATURATE")) return GL_SRC_ALPHA_SATURATE;
	}

	return GL_ONE_MINUS_SRC_ALPHA;
}

static uint32_t StringToBlendEquation(const char *szString)
{
	if (szString) {
		if (!stricmp(szString, "GL_FUNC_ADD")) return GL_FUNC_ADD;
		if (!stricmp(szString, "GL_FUNC_SUBTRACT")) return GL_FUNC_SUBTRACT;
		if (!stricmp(szString, "GL_FUNC_REVERSE_SUBTRACT")) return GL_FUNC_REVERSE_SUBTRACT;
		if (!stricmp(szString, "GL_MIN")) return GL_MIN;
		if (!stricmp(szString, "GL_MAX")) return GL_MAX;
	}

	return GL_FUNC_ADD;
}


static bool InternalLoadPipelineState(TiXmlNode *pPipelineNode, PipelineState &state)
{
	if (TiXmlNode *pStateNode = pPipelineNode->FirstChild("State")) {
		LogOutput(LOG_TAG_RENDERER, "\t\t\tLoadState ... ");
		{
			if (TiXmlNode *pCullNode = pStateNode->FirstChild("Cull")) {
				state.bEnableCullFace = pCullNode->ToElement()->AttributeBool("enable");
				state.cullFace = StringToCullFace(pCullNode->ToElement()->AttributeString("cull_face"));
				state.frontFace = StringToFrontFace(pCullNode->ToElement()->AttributeString("front_face"));
			}

			if (TiXmlNode *pStencilNode = pStateNode->FirstChild("Stencil")) {
				state.bEnableStencilTest = pStencilNode->ToElement()->AttributeBool("enable");
				state.stencilFunc = StringToStencilFunc(pStencilNode->ToElement()->AttributeString("func"));
				state.stencilRef = pStencilNode->ToElement()->AttributeInt1("ref");
				state.stencilMask = pStencilNode->ToElement()->AttributeInt1("mask");
				state.stencilOpSFail = StringToStencilOp(pStencilNode->ToElement()->AttributeString("sfail"));
				state.stencilOpDFail = StringToStencilOp(pStencilNode->ToElement()->AttributeString("dfail"));
				state.stencilOpDPass = StringToStencilOp(pStencilNode->ToElement()->AttributeString("dpass"));
			}

			if (TiXmlNode *pDepthNode = pStateNode->FirstChild("Depth")) {
				state.bEnableDepthTest = pDepthNode->ToElement()->AttributeBool("enable_test");
				state.bEnableDepthWrite = pDepthNode->ToElement()->AttributeBool("enable_write");
				state.depthFunc = StringToDepthFunc(pDepthNode->ToElement()->AttributeString("func"));
				state.depthRangeNear = pDepthNode->ToElement()->AttributeFloat1("range_near");
				state.depthRangeFar = pDepthNode->ToElement()->AttributeFloat1("range_far");
			}

			if (TiXmlNode *pColorNode = pStateNode->FirstChild("Color")) {
				state.bEnableColorWrite[0] = pColorNode->ToElement()->AttributeBool("enable_write_red");
				state.bEnableColorWrite[1] = pColorNode->ToElement()->AttributeBool("enable_write_green");
				state.bEnableColorWrite[2] = pColorNode->ToElement()->AttributeBool("enable_write_blue");
				state.bEnableColorWrite[3] = pColorNode->ToElement()->AttributeBool("enable_write_alpha");
			}

			if (TiXmlNode *pAlphaToCoverageNode = pStateNode->FirstChild("AlphaToCoverage")) {
				state.bEnableAlphaToCoverage = pAlphaToCoverageNode->ToElement()->AttributeBool("enable");
			}

			if (TiXmlNode *pBlendNode = pStateNode->FirstChild("Blend")) {
				state.bEnableBlend = pBlendNode->ToElement()->AttributeBool("enable");
				state.blendSrcFactor = StringToBlendSrcFactor(pBlendNode->ToElement()->AttributeString("src_factor"));
				state.blendDstFactor = StringToBlendDstFactor(pBlendNode->ToElement()->AttributeString("dst_factor"));
				state.blendEquation = StringToBlendEquation(pBlendNode->ToElement()->AttributeString("equation"));
			}

			if (TiXmlNode *pOffsetNode = pStateNode->FirstChild("Offset")) {
				state.bEnablePolygonOffset = pOffsetNode->ToElement()->AttributeBool("enable");
				state.polygonOffsetFactor = pOffsetNode->ToElement()->AttributeFloat1("factor");
				state.polygonOffsetUnits = pOffsetNode->ToElement()->AttributeFloat1("units");
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

		pShader = Renderer()->CreateShader(szBinFileName, kind);
		if (pShader->IsValid() == false) { err = -3; goto ERR; }
	}
	LogOutput(nullptr, "OK\n");
	return true;
ERR:
	LogOutput(nullptr, "Fail(%d)\n", err);
	return false;
}

static bool InternalLoadPipeline(TiXmlNode *pPassNode, CGfxMaterialPass *pPass)
{
	int err = 0;

	LogOutput(LOG_TAG_RENDERER, "\t\tLoadPipeline\n");
	{
		TiXmlNode *pPipelineNode = pPassNode->FirstChild("Pipeline");
		if (pPipelineNode == nullptr) { err = -1; goto ERR; }

		PipelineState state;
		if (InternalLoadPipelineState(pPipelineNode, state) == false) { err = -2; goto ERR; }

		CGfxShader *pVertexShader = nullptr;
		CGfxShader *pFragmentShader = nullptr;
		if (InternalLoadPipelineShader(pPipelineNode, pVertexShader, vertex_shader) == false) { err = -3; goto ERR; }
		if (InternalLoadPipelineShader(pPipelineNode, pFragmentShader, fragment_shader) == false) { err = -4; goto ERR; }
		if (pPass->SetPipeline(pVertexShader, pFragmentShader, state) == false) { err = -5; goto ERR; }
	}
	LogOutput(LOG_TAG_RENDERER, "\t\tOK\n");
	return true;
ERR:
	LogOutput(LOG_TAG_RENDERER, "Fail(%d)\n", err);
	return false;
}

static bool InternalLoadTexture2D(TiXmlNode *pPassNode, CGfxMaterialPass *pPass)
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

				uint32_t minFilter = StringToMinFilter(pTextureNode->ToElement()->AttributeString("min_filter"));
				uint32_t magFilter = StringToMagFilter(pTextureNode->ToElement()->AttributeString("mag_filter"));
				uint32_t addressMode = StringToAddressMode(pTextureNode->ToElement()->AttributeString("address_mode"));
				if (minFilter == GL_INVALID_ENUM || magFilter == GL_INVALID_ENUM || addressMode == GL_INVALID_ENUM) { err = -2; goto ERR; }

				pPass->SetSampler(szName, minFilter, magFilter, addressMode);
				pPass->SetTexture2D(szName, szFileName);
			}
			LogOutput(nullptr, "OK\n");
		} while ((pTextureNode = pPassNode->IterateChildren("Texture2D", pTextureNode)) != nullptr);
	}
	return true;
ERR:
	LogOutput(nullptr, "Fail(%d)\n", err);
	return false;
}

static bool InternalLoadTexture2DArray(TiXmlNode *pPassNode, CGfxMaterialPass *pPass)
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

				uint32_t minFilter = StringToMinFilter(pTextureNode->ToElement()->AttributeString("min_filter"));
				uint32_t magFilter = StringToMagFilter(pTextureNode->ToElement()->AttributeString("mag_filter"));
				uint32_t addressMode = StringToAddressMode(pTextureNode->ToElement()->AttributeString("address_mode"));
				if (minFilter == GL_INVALID_ENUM || magFilter == GL_INVALID_ENUM || addressMode == GL_INVALID_ENUM) { err = -2; goto ERR; }

				pPass->SetSampler(szName, minFilter, magFilter, addressMode);
				pPass->SetTexture2DArray(szName, szFileName);
			}
			LogOutput(nullptr, "OK\n");
		} while ((pTextureNode = pPassNode->IterateChildren("Texture2DArray", pTextureNode)) != nullptr);
	}
	return true;
ERR:
	LogOutput(nullptr, "Fail(%d)\n", err);
	return false;
}

static bool InternalLoadTextureCubeMap(TiXmlNode *pPassNode, CGfxMaterialPass *pPass)
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

				uint32_t minFilter = StringToMinFilter(pTextureNode->ToElement()->AttributeString("min_filter"));
				uint32_t magFilter = StringToMagFilter(pTextureNode->ToElement()->AttributeString("mag_filter"));
				uint32_t addressMode = StringToAddressMode(pTextureNode->ToElement()->AttributeString("address_mode"));
				if (minFilter == GL_INVALID_ENUM || magFilter == GL_INVALID_ENUM || addressMode == GL_INVALID_ENUM) { err = -2; goto ERR; }

				pPass->SetSampler(szName, minFilter, magFilter, addressMode);
				pPass->SetTextureCubeMap(szName, szFileName);
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

				LogOutput(nullptr, "%s = \"%s\" ... ", szName, szValue);

				float value[1] = { 0.0f };
				pUniformNode->ToElement()->AttributeFloat1("value", value);
				pPass->SetUniformVec1(szName, value[0]);
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
			LogOutput(LOG_TAG_RENDERER, "\t\tLoadUniformVec2 ... ");
			{
				const char *szName = pUniformNode->ToElement()->AttributeString("name");
				const char *szValue = pUniformNode->ToElement()->AttributeString("value");
				if (szName == nullptr || szValue == nullptr) { err = -1; goto ERR; }

				LogOutput(nullptr, "%s = \"%s\" ... ", szName, szValue);

				float value[2] = { 0.0f };
				pUniformNode->ToElement()->AttributeFloat2("value", value);
				pPass->SetUniformVec2(szName, value[0], value[1]);
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
			LogOutput(LOG_TAG_RENDERER, "\t\tLoadUniformVec3 ... ");
			{
				const char *szName = pUniformNode->ToElement()->AttributeString("name");
				const char *szValue = pUniformNode->ToElement()->AttributeString("value");
				if (szName == nullptr || szValue == nullptr) { err = -1; goto ERR; }

				LogOutput(nullptr, "%s = \"%s\" ... ", szName, szValue);

				float value[3] = { 0.0f };
				pUniformNode->ToElement()->AttributeFloat3("value", value);
				pPass->SetUniformVec3(szName, value[0], value[1], value[2]);
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
			LogOutput(LOG_TAG_RENDERER, "\t\tLoadUniformVec4 ... ");
			{
				const char *szName = pUniformNode->ToElement()->AttributeString("name");
				const char *szValue = pUniformNode->ToElement()->AttributeString("value");
				if (szName == nullptr || szValue == nullptr) { err = -1; goto ERR; }

				LogOutput(nullptr, "%s = \"%s\" ... ", szName, szValue);

				float value[4] = { 0.0f };
				pUniformNode->ToElement()->AttributeFloat4("value", value);
				pPass->SetUniformVec4(szName, value[0], value[1], value[2], value[3]);
			}
			LogOutput(nullptr, "OK\n");
		} while ((pUniformNode = pPassNode->IterateChildren("Uniform4f", pUniformNode)) != nullptr);
	}
	return true;
ERR:
	LogOutput(nullptr, "Fail(%d)\n", err);
	return false;
}

static bool InternalLoadPass(TiXmlNode *pPassNode, CGfxMaterialPass *pPass)
{
	int err = 0;

	LogOutput(LOG_TAG_RENDERER, "\tLoadPass(%s)\n", pPassNode->ToElement()->AttributeString("name"));
	{
		if (InternalLoadPipeline(pPassNode, pPass) == false) { err = -1; goto ERR; }
		if (InternalLoadTexture2D(pPassNode, pPass) == false) { err = -2; goto ERR; }
		if (InternalLoadTexture2DArray(pPassNode, pPass) == false) { err = -3; goto ERR; }
		if (InternalLoadTextureCubeMap(pPassNode, pPass) == false) { err = -4; goto ERR; }
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


bool CGfxResourceLoader::LoadMaterial(const char *szFileName, CGfxMaterial *pMaterial)
{
	//<Material>
	//	<Pass name="">
	//		<Pipeline>
	//			<Vertex file_name="">
	//				<Define name="" />
	//				<Define name="" />
	//			</Vertex>
	//			<Fragment file_name="">
	//				<Define name="" />
	//				<Define name="" />
	//			</Fragment>
	//			<State>
	//				<Cull enable="" cull_face="" front_face="" />
	//				<Stencil enable="" func="" ref="" mask="" sfail="" dfail="" dpass="" />
	//				<Depth enable_test="" enable_write="" func="" range_near="" range_far="" />
	//				<Color enable_write_red="" enable_write_green="" enable_write_blue="" enable_write_alpha="" />
	//				<AlphaToCoverage enable="" />
	//				<Blend enable="" src_factor="" dst_factor="" equation="" />
	//				<Offset enable="" factor="" units="" />
	//			</State>
	//		</Pipeline>
	//		<Texture2D file_name="" name="" min_filter="" mag_filter="" address_mode="" />
	//		<Texture2DArray file_name="" name="" min_filter="" mag_filter="" address_mode="" />
	//		<TextureCubeMap file_name="" name="" min_filter="" mag_filter="" address_mode="" />
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
				if (InternalLoadPass(pPassNode, pMaterial->GetPass(name)) == false) { err = -5; goto ERR; }
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
