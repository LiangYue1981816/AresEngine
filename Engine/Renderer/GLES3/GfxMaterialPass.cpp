#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxMaterialPass.h"


#define TEXTURE_INTERNAL_NAME(name) (uint32_t)(name ^ (size_t)this)

static uint32_t StringToCullFace(const char *szString)
{
	if (szString) {
		if (!stricmp(szString, "GL_FRONT")) return GL_FRONT;
		if (!stricmp(szString, "GL_BACK")) return GL_BACK;
		if (!stricmp(szString, "GL_FRONT_AND_BACK ")) return GL_FRONT_AND_BACK;
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


CGfxMaterialPass::CGfxMaterialPass(uint32_t name)
	: m_name(name)
	, m_pPipeline(nullptr)
{

}

CGfxMaterialPass::~CGfxMaterialPass(void)
{
	for (auto &itUniform : m_pUniformVec1s) {
		delete itUniform.second;
	}

	for (auto &itUniform : m_pUniformVec2s) {
		delete itUniform.second;
	}

	for (auto &itUniform : m_pUniformVec3s) {
		delete itUniform.second;
	}

	for (auto &itUniform : m_pUniformVec4s) {
		delete itUniform.second;
	}

	for (auto &itUniform : m_pUniformMat4s) {
		delete itUniform.second;
	}

	m_pSamplers.clear();
	m_ptrTexture2ds.clear();
	m_ptrTexture2dArrays.clear();
	m_ptrTextureCubeMaps.clear();
	m_pUniformVec1s.clear();
	m_pUniformVec2s.clear();
	m_pUniformVec3s.clear();
	m_pUniformVec4s.clear();
	m_pUniformMat4s.clear();
}

uint32_t CGfxMaterialPass::GetName(void) const
{
	return m_name;
}

bool CGfxMaterialPass::Load(TiXmlNode *pPassNode)
{
	try {
		LogOutput(LOG_TAG_RENDERER, "\tLoadPass(%s)\n", pPassNode->ToElement()->AttributeString("name"));
		{
			if (LoadPipeline(pPassNode) == false) throw 0;
			if (LoadTexture2D(pPassNode) == false) throw 1;
			if (LoadTexture2DArray(pPassNode) == false) throw 2;
			if (LoadTextureCubeMap(pPassNode) == false) throw 3;
			if (LoadUniformVec1(pPassNode) == false) throw 4;
			if (LoadUniformVec2(pPassNode) == false) throw 5;
			if (LoadUniformVec3(pPassNode) == false) throw 6;
			if (LoadUniformVec4(pPassNode) == false) throw 7;
		}
		LogOutput(LOG_TAG_RENDERER, "\tOK\n");
		return true;
	}
	catch (int err) {
		LogOutput(LOG_TAG_RENDERER, "\tFail(%d)\n", err);
		return false;
	}
}

bool CGfxMaterialPass::LoadPipeline(TiXmlNode *pPassNode)
{
	try {
		LogOutput(LOG_TAG_RENDERER, "\t\tLoadPipeline\n");
		{
			TiXmlNode *pPipelineNode = pPassNode->FirstChild("Pipeline");
			if (pPipelineNode == nullptr) throw 0;

			GLstate state;
			LoadPipelineState(pPipelineNode, state);

			CGfxShader *pVertexShader = nullptr;
			CGfxShader *pFragmentShader = nullptr;
			LoadPipelineShader(pPipelineNode, pVertexShader, shaderc_vertex_shader);
			LoadPipelineShader(pPipelineNode, pFragmentShader, shaderc_fragment_shader);

			if (SetPipeline(pVertexShader, pFragmentShader, state) == false) throw 1;
		}
		LogOutput(LOG_TAG_RENDERER, "\t\tOK\n");
		return true;
	}
	catch (int err) {
		LogOutput(LOG_TAG_RENDERER, "\t\tFail(%d)", err);
		return false;
	}
}

bool CGfxMaterialPass::LoadPipelineState(TiXmlNode *pPipelineNode, GLstate &state)
{
	GLInitState(&state);

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

bool CGfxMaterialPass::LoadPipelineShader(TiXmlNode *pPipelineNode, CGfxShader *&pShader, shaderc_shader_kind kind)
{
	try {
		char szShaderKind[2][_MAX_STRING] = { "Vertex", "Fragment" };

		LogOutput(LOG_TAG_RENDERER, "\t\t\tLoad%sShader ", szShaderKind[kind]);
		{
			TiXmlNode *pShaderNode = pPipelineNode->FirstChild(szShaderKind[kind]);
			if (pShaderNode == nullptr) throw 0;

			const char *szFileName = pShaderNode->ToElement()->AttributeString("file_name");
			if (szFileName == nullptr) throw 1;

			LogOutput(nullptr, "%s ... ", szFileName);

			Renderer()->GetShaderCompiler()->ClearMacroDefinition();
			{
				if (TiXmlNode *pDefineNode = pShaderNode->FirstChild("Define")) {
					do {
						if (const char *szDefine = pDefineNode->ToElement()->AttributeString("name")) {
							Renderer()->GetShaderCompiler()->AddMacroDefinition(szDefine);
						}
					} while (pDefineNode = pShaderNode->IterateChildren("Define", pDefineNode));
				}
			}

			std::string preprocess = Renderer()->GetShaderCompiler()->Preprocess(Renderer()->GetResourceFullName(szFileName), kind);
			if (preprocess.empty()) throw 2;

			char szExtName[2][_MAX_STRING] = { "vert", "frag" };
			char szBinFileName[_MAX_STRING] = { 0 };
			sprintf(szBinFileName, "%x.%s", HashValue(preprocess.c_str()), szExtName[kind]);

#ifdef PLATFORM_WINDOWS
			if (fexist(Renderer()->GetResourceFullName(szBinFileName)) == 0) {
				Renderer()->GetShaderCompiler()->Compile(Renderer()->GetResourceFullName(szFileName), szBinFileName, kind);
			}
#endif
			pShader = Renderer()->LoadShader(szBinFileName, kind);
			if (pShader->IsValid() == false) throw 3;
		}
		LogOutput(nullptr, "OK\n");
		return true;
	}
	catch (int err) {
		LogOutput(nullptr, "Fail(%d)\n", err);
		return false;
	}
}

bool CGfxMaterialPass::LoadTexture2D(TiXmlNode *pPassNode)
{
	try {
		if (TiXmlNode *pTextureNode = pPassNode->FirstChild("Texture2D")) {
			do {
				LogOutput(LOG_TAG_RENDERER, "\t\tLoadTexture2D ");
				{
					const char *szName = pTextureNode->ToElement()->AttributeString("name");
					const char *szFileName = pTextureNode->ToElement()->AttributeString("file_name");
					if (szName == nullptr || szFileName == nullptr) throw 0;

					LogOutput(nullptr, "%s ... ", szFileName);

					uint32_t minFilter = StringToMinFilter(pTextureNode->ToElement()->AttributeString("min_filter"));
					uint32_t magFilter = StringToMagFilter(pTextureNode->ToElement()->AttributeString("mag_filter"));
					uint32_t addressMode = StringToAddressMode(pTextureNode->ToElement()->AttributeString("address_mode"));
					if (minFilter == GL_INVALID_ENUM || magFilter == GL_INVALID_ENUM || addressMode == GL_INVALID_ENUM) throw 1;

					SetSampler(szName, minFilter, magFilter, addressMode);
					SetTexture2D(szName, szFileName);
				}
				LogOutput(nullptr, "OK\n");
			} while (pTextureNode = pPassNode->IterateChildren("Texture2D", pTextureNode));
		}
		return true;
	}
	catch (int err) {
		LogOutput(nullptr, "Fail(%d)\n", err);
		return false;
	}
}

bool CGfxMaterialPass::LoadTexture2DArray(TiXmlNode *pPassNode)
{
	try {
		if (TiXmlNode *pTextureNode = pPassNode->FirstChild("Texture2DArray")) {
			do {
				LogOutput(LOG_TAG_RENDERER, "\t\tLoadTexture2DArray ");
				{
					const char *szName = pTextureNode->ToElement()->AttributeString("name");
					const char *szFileName = pTextureNode->ToElement()->AttributeString("file_name");
					if (szName == nullptr || szFileName == nullptr) throw 0;

					LogOutput(nullptr, "%s ... ", szFileName);

					uint32_t minFilter = StringToMinFilter(pTextureNode->ToElement()->AttributeString("min_filter"));
					uint32_t magFilter = StringToMagFilter(pTextureNode->ToElement()->AttributeString("mag_filter"));
					uint32_t addressMode = StringToAddressMode(pTextureNode->ToElement()->AttributeString("address_mode"));
					if (minFilter == GL_INVALID_ENUM || magFilter == GL_INVALID_ENUM || addressMode == GL_INVALID_ENUM) throw 1;

					SetSampler(szName, minFilter, magFilter, addressMode);
					SetTexture2DArray(szName, szFileName);
				}
				LogOutput(nullptr, "OK\n");
			} while (pTextureNode = pPassNode->IterateChildren("Texture2DArray", pTextureNode));
		}
		return true;
	}
	catch (int err) {
		LogOutput(nullptr, "Fail(%d)\n", err);
		return false;
	}
}

bool CGfxMaterialPass::LoadTextureCubeMap(TiXmlNode *pPassNode)
{
	try {
		if (TiXmlNode *pTextureNode = pPassNode->FirstChild("TextureCubeMap")) {
			do {
				LogOutput(LOG_TAG_RENDERER, "\t\tLoadTextureCubeMap ");
				{
					const char *szName = pTextureNode->ToElement()->AttributeString("name");
					const char *szFileName = pTextureNode->ToElement()->AttributeString("file_name");
					if (szName == nullptr || szFileName == nullptr) throw 0;

					LogOutput(nullptr, "%s ... ", szFileName);

					uint32_t minFilter = StringToMinFilter(pTextureNode->ToElement()->AttributeString("min_filter"));
					uint32_t magFilter = StringToMagFilter(pTextureNode->ToElement()->AttributeString("mag_filter"));
					uint32_t addressMode = StringToAddressMode(pTextureNode->ToElement()->AttributeString("address_mode"));
					if (minFilter == GL_INVALID_ENUM || magFilter == GL_INVALID_ENUM || addressMode == GL_INVALID_ENUM) throw 1;

					SetSampler(szName, minFilter, magFilter, addressMode);
					SetTextureCubeMap(szName, szFileName);
				}
				LogOutput(nullptr, "OK\n");
			} while (pTextureNode = pPassNode->IterateChildren("TextureCubeMap", pTextureNode));
		}
		return true;
	}
	catch (int err) {
		LogOutput(nullptr, "Fail(%d)\n", err);
		return false;
	}
}

bool CGfxMaterialPass::LoadUniformVec1(TiXmlNode *pPassNode)
{
	try {
		if (TiXmlNode *pUniformNode = pPassNode->FirstChild("Uniform1f")) {
			do {
				LogOutput(LOG_TAG_RENDERER, "\t\tLoadUniformVec1 ");
				{
					const char *szName = pUniformNode->ToElement()->AttributeString("name");
					const char *szValue = pUniformNode->ToElement()->AttributeString("value");
					if (szName == nullptr || szValue == nullptr) throw 0;

					LogOutput(nullptr, "%s = \"%s\" ... ", szName, szValue);

					float value = pUniformNode->ToElement()->AttributeFloat1("value");
					SetUniformVec1(szName, value);
				}
				LogOutput(nullptr, "OK\n");
			} while (pUniformNode = pPassNode->IterateChildren("Uniform1f", pUniformNode));
		}
		return true;
	}
	catch (int err) {
		LogOutput(nullptr, "Fail(%d)\n", err);
		return false;
	}
}

bool CGfxMaterialPass::LoadUniformVec2(TiXmlNode *pPassNode)
{
	try {
		if (TiXmlNode *pUniformNode = pPassNode->FirstChild("Uniform2f")) {
			do {
				LogOutput(LOG_TAG_RENDERER, "\t\tLoadUniformVec2 ... ");
				{
					const char *szName = pUniformNode->ToElement()->AttributeString("name");
					const char *szValue = pUniformNode->ToElement()->AttributeString("value");
					if (szName == nullptr || szValue == nullptr) throw 0;

					LogOutput(nullptr, "%s = \"%s\" ... ", szName, szValue);

					float value[2]; pUniformNode->ToElement()->AttributeFloat2("value", value);
					SetUniformVec2(szName, value[0], value[1]);
				}
				LogOutput(nullptr, "OK\n");
			} while (pUniformNode = pPassNode->IterateChildren("Uniform2f", pUniformNode));
		}
		return true;
	}
	catch (int err) {
		LogOutput(nullptr, "Fail(%d)\n", err);
		return false;
	}
}

bool CGfxMaterialPass::LoadUniformVec3(TiXmlNode *pPassNode)
{
	try {
		if (TiXmlNode *pUniformNode = pPassNode->FirstChild("Uniform3f")) {
			do {
				LogOutput(LOG_TAG_RENDERER, "\t\tLoadUniformVec3 ... ");
				{
					const char *szName = pUniformNode->ToElement()->AttributeString("name");
					const char *szValue = pUniformNode->ToElement()->AttributeString("value");
					if (szName == nullptr || szValue == nullptr) throw 0;

					LogOutput(nullptr, "%s = \"%s\" ... ", szName, szValue);

					float value[3]; pUniformNode->ToElement()->AttributeFloat3("value", value);
					SetUniformVec3(szName, value[0], value[1], value[2]);
				}
				LogOutput(nullptr, "OK\n");
			} while (pUniformNode = pPassNode->IterateChildren("Uniform3f", pUniformNode));
		}
		return true;
	}
	catch (int err) {
		LogOutput(nullptr, "Fail(%d)\n", err);
		return false;
	}
}

bool CGfxMaterialPass::LoadUniformVec4(TiXmlNode *pPassNode)
{
	try {
		if (TiXmlNode *pUniformNode = pPassNode->FirstChild("Uniform4f")) {
			do {
				LogOutput(LOG_TAG_RENDERER, "\t\tLoadUniformVec4 ... ");
				{
					const char *szName = pUniformNode->ToElement()->AttributeString("name");
					const char *szValue = pUniformNode->ToElement()->AttributeString("value");
					if (szName == nullptr || szValue == nullptr) throw 0;

					LogOutput(nullptr, "%s = \"%s\" ... ", szName, szValue);

					float value[4]; pUniformNode->ToElement()->AttributeFloat4("value", value);
					SetUniformVec4(szName, value[0], value[1], value[2], value[3]);
				}
				LogOutput(nullptr, "OK\n");
			} while (pUniformNode = pPassNode->IterateChildren("Uniform4f", pUniformNode));
		}
		return true;
	}
	catch (int err) {
		LogOutput(nullptr, "Fail(%d)\n", err);
		return false;
	}
}

bool CGfxMaterialPass::SetPipeline(const CGfxShader *pVertexShader, const CGfxShader *pFragmentShader, const GLstate &state)
{
	if (pVertexShader == nullptr ||
		pVertexShader->IsValid() == false) {
		return false;
	}

	if (pFragmentShader == nullptr ||
		pFragmentShader->IsValid() == false) {
		return false;
	}

	m_pPipeline = Renderer()->CreatePipelineGraphics(pVertexShader, pFragmentShader, state);
	return true;
}

bool CGfxMaterialPass::SetSampler(const char *szName, uint32_t minFilter, uint32_t magFilter, uint32_t addressMode)
{
	uint32_t name = HashValue(szName);

	if ((m_pPipeline == nullptr) || (m_pPipeline && m_pPipeline->IsTextureValid(name))) {
		m_pSamplers[name] = Renderer()->CreateSampler(minFilter, magFilter, addressMode);
		return true;
	}

	return false;
}

bool CGfxMaterialPass::SetTexture2D(const char *szName, uint32_t externTexture)
{
	uint32_t name = HashValue(szName);

	if ((m_pPipeline == nullptr) || (m_pPipeline && m_pPipeline->IsTextureValid(name))) {
		m_ptrTexture2ds[name] = Renderer()->CreateTexture2D(TEXTURE_INTERNAL_NAME(name));
		m_ptrTexture2ds[name]->CreateExtern(externTexture);
		return true;
	}

	return false;
}

bool CGfxMaterialPass::SetTexture2DArray(const char *szName, uint32_t externTexture)
{
	uint32_t name = HashValue(szName);

	if ((m_pPipeline == nullptr) || (m_pPipeline && m_pPipeline->IsTextureValid(name))) {
		m_ptrTexture2dArrays[name] = Renderer()->CreateTexture2DArray(TEXTURE_INTERNAL_NAME(name));
		m_ptrTexture2dArrays[name]->CreateExtern(externTexture);
		return true;
	}

	return false;
}

bool CGfxMaterialPass::SetTextureCubeMap(const char *szName, uint32_t externTexture)
{
	uint32_t name = HashValue(szName);

	if ((m_pPipeline == nullptr) || (m_pPipeline && m_pPipeline->IsTextureValid(name))) {
		m_ptrTextureCubeMaps[name] = Renderer()->CreateTextureCubeMap(TEXTURE_INTERNAL_NAME(name));
		m_ptrTextureCubeMaps[name]->CreateExtern(externTexture);
		return true;
	}

	return false;
}

bool CGfxMaterialPass::SetTexture2D(const char *szName, const char *szFileName)
{
	uint32_t name = HashValue(szName);

	if ((m_pPipeline == nullptr) || (m_pPipeline && m_pPipeline->IsTextureValid(name))) {
		m_ptrTexture2ds[name] = Renderer()->LoadTexture2D(szFileName);
		return true;
	}

	return false;
}

bool CGfxMaterialPass::SetTexture2DArray(const char *szName, const char *szFileName)
{
	uint32_t name = HashValue(szName);

	if ((m_pPipeline == nullptr) || (m_pPipeline && m_pPipeline->IsTextureValid(name))) {
		m_ptrTexture2dArrays[name] = Renderer()->LoadTexture2DArray(szFileName);
		return true;
	}

	return false;
}

bool CGfxMaterialPass::SetTextureCubeMap(const char *szName, const char *szFileName)
{
	uint32_t name = HashValue(szName);

	if ((m_pPipeline == nullptr) || (m_pPipeline && m_pPipeline->IsTextureValid(name))) {
		m_ptrTextureCubeMaps[name] = Renderer()->LoadTextureCubeMap(szFileName);
		return true;
	}

	return false;
}

bool CGfxMaterialPass::SetUniformVec1(const char *szName, float x)
{
	uint32_t name = HashValue(szName);

	if ((m_pPipeline == nullptr) || (m_pPipeline && m_pPipeline->IsUniformBlockValid(name))) {
		if (m_pUniformVec1s[name] == nullptr) {
			m_pUniformVec1s[name] = new CGfxUniformVec1;
		}

		m_pUniformVec1s[name]->SetValue(x);
		return true;
	}

	return false;
}

bool CGfxMaterialPass::SetUniformVec2(const char *szName, float x, float y)
{
	uint32_t name = HashValue(szName);

	if ((m_pPipeline == nullptr) || (m_pPipeline && m_pPipeline->IsUniformBlockValid(name))) {
		if (m_pUniformVec2s[name] == nullptr) {
			m_pUniformVec2s[name] = new CGfxUniformVec2;
		}

		m_pUniformVec2s[name]->SetValue(x, y);
		return true;
	}

	return false;
}

bool CGfxMaterialPass::SetUniformVec3(const char *szName, float x, float y, float z)
{
	uint32_t name = HashValue(szName);

	if ((m_pPipeline == nullptr) || (m_pPipeline && m_pPipeline->IsUniformBlockValid(name))) {
		if (m_pUniformVec3s[name] == nullptr) {
			m_pUniformVec3s[name] = new CGfxUniformVec3;
		}

		m_pUniformVec3s[name]->SetValue(x, y, z);
		return true;
	}

	return false;
}

bool CGfxMaterialPass::SetUniformVec4(const char *szName, float x, float y, float z, float w)
{
	uint32_t name = HashValue(szName);

	if ((m_pPipeline == nullptr) || (m_pPipeline && m_pPipeline->IsUniformBlockValid(name))) {
		if (m_pUniformVec4s[name] == nullptr) {
			m_pUniformVec4s[name] = new CGfxUniformVec4;
		}

		m_pUniformVec4s[name]->SetValue(x, y, z, w);
		return true;
	}

	return false;
}

bool CGfxMaterialPass::SetUniformMat4(const char *szName, const float *matrix)
{
	uint32_t name = HashValue(szName);

	if ((m_pPipeline == nullptr) || (m_pPipeline && m_pPipeline->IsUniformBlockValid(name))) {
		if (m_pUniformMat4s[name] == nullptr) {
			m_pUniformMat4s[name] = new CGfxUniformMat4;
		}

		m_pUniformMat4s[name]->SetValue(matrix);
		return true;
	}

	return false;
}

CGfxPipelineGraphics* CGfxMaterialPass::GetPipeline(void) const
{
	return m_pPipeline;
}

void CGfxMaterialPass::Bind(const CGfxPipelineBase *pPipeline, const CGfxMaterialPass *pPass, uint32_t &indexTexUnit)
{
	BindUniforms(pPipeline, pPass);
	BindTextures(pPipeline, pPass, indexTexUnit);
}

void CGfxMaterialPass::BindUniforms(const CGfxPipelineBase *pPipeline, const CGfxMaterialPass *pPass)
{
	if (pPipeline) {
		for (const auto &itUniform : pPass->m_pUniformVec1s) {
			itUniform.second->Apply();
			pPipeline->BindUniformBuffer(itUniform.first, itUniform.second->GetBuffer(), itUniform.second->GetSize());
		}

		for (const auto &itUniform : pPass->m_pUniformVec2s) {
			itUniform.second->Apply();
			pPipeline->BindUniformBuffer(itUniform.first, itUniform.second->GetBuffer(), itUniform.second->GetSize());
		}

		for (const auto &itUniform : pPass->m_pUniformVec3s) {
			itUniform.second->Apply();
			pPipeline->BindUniformBuffer(itUniform.first, itUniform.second->GetBuffer(), itUniform.second->GetSize());
		}

		for (const auto &itUniform : pPass->m_pUniformVec4s) {
			itUniform.second->Apply();
			pPipeline->BindUniformBuffer(itUniform.first, itUniform.second->GetBuffer(), itUniform.second->GetSize());
		}

		for (const auto &itUniform : pPass->m_pUniformMat4s) {
			itUniform.second->Apply();
			pPipeline->BindUniformBuffer(itUniform.first, itUniform.second->GetBuffer(), itUniform.second->GetSize());
		}
	}
}

void CGfxMaterialPass::BindTextures(const CGfxPipelineBase *pPipeline, const CGfxMaterialPass *pPass, uint32_t &indexTexUnit)
{
	if (pPipeline) {
		for (const auto &itTexture : pPass->m_ptrTexture2ds) {
			if (pPipeline->BindTexture(itTexture.first, itTexture.second->GetTarget(), itTexture.second->GetTexture(), pPass->m_pSamplers.find(itTexture.first)->second->GetSampler(), indexTexUnit)) {
				indexTexUnit++;
			}
		}

		for (const auto &itTexture : pPass->m_ptrTexture2dArrays) {
			if (pPipeline->BindTexture(itTexture.first, itTexture.second->GetTarget(), itTexture.second->GetTexture(), pPass->m_pSamplers.find(itTexture.first)->second->GetSampler(), indexTexUnit)) {
				indexTexUnit++;
			}
		}

		for (const auto &itTexture : pPass->m_ptrTextureCubeMaps) {
			if (pPipeline->BindTexture(itTexture.first, itTexture.second->GetTarget(), itTexture.second->GetTexture(), pPass->m_pSamplers.find(itTexture.first)->second->GetSampler(), indexTexUnit)) {
				indexTexUnit++;
			}
		}
	}
}
