#include "GfxHeader.h"


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


CGLES3MaterialPass::CGLES3MaterialPass(uint32_t name)
	: CGfxMaterialPass(name)
	, m_pPipeline(nullptr)
{

}

CGLES3MaterialPass::~CGLES3MaterialPass(void)
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
	m_ptrTexture2Ds.clear();
	m_ptrTexture2DArrays.clear();
	m_ptrTextureCubeMaps.clear();
	m_pUniformVec1s.clear();
	m_pUniformVec2s.clear();
	m_pUniformVec3s.clear();
	m_pUniformVec4s.clear();
	m_pUniformMat4s.clear();
}

bool CGLES3MaterialPass::SetPipeline(const CGfxShader *pVertexShader, const CGfxShader *pFragmentShader, const PipelineState &state)
{
	if (pVertexShader == nullptr) {
		return false;
	}

	if (pVertexShader->IsValid() == false) {
		return false;
	}

	if (pVertexShader->GetKind() != vertex_shader) {
		return false;
	}

	if (pFragmentShader == nullptr) {
		return false;
	}

	if (pFragmentShader->IsValid() == false) {
		return false;
	}

	if (pFragmentShader->GetKind() != fragment_shader) {
		return false;
	}

	m_pPipeline = GLES3Renderer()->CreatePipelineGraphics(pVertexShader, pFragmentShader, state);
	return m_pPipeline != nullptr;
}

bool CGLES3MaterialPass::SetSampler(const char *szName, uint32_t minFilter, uint32_t magFilter, uint32_t addressMode)
{
	uint32_t name = HashValue(szName);

	if ((m_pPipeline == nullptr) || (m_pPipeline && m_pPipeline->IsTextureValid(name))) {
		m_pSamplers[name] = GLES3Renderer()->CreateSampler(minFilter, magFilter, addressMode);
		return true;
	}

	return false;
}

bool CGLES3MaterialPass::SetTexture2D(const char *szName, uint32_t externTexture)
{
	uint32_t name = HashValue(szName);

	if ((m_pPipeline == nullptr) || (m_pPipeline && m_pPipeline->IsTextureValid(name))) {
		m_ptrTexture2Ds[name] = GLES3Renderer()->NewTexture2D(TEXTURE_INTERNAL_NAME(name));
		((CGLES3TextureBase *)m_ptrTexture2Ds[name].GetPointer())->Create(GL_TEXTURE_2D, externTexture);
		return true;
	}

	return false;
}

bool CGLES3MaterialPass::SetTexture2DArray(const char *szName, uint32_t externTexture)
{
	uint32_t name = HashValue(szName);

	if ((m_pPipeline == nullptr) || (m_pPipeline && m_pPipeline->IsTextureValid(name))) {
		m_ptrTexture2DArrays[name] = GLES3Renderer()->NewTexture2DArray(TEXTURE_INTERNAL_NAME(name));
		((CGLES3TextureBase *)m_ptrTexture2DArrays[name].GetPointer())->Create(GL_TEXTURE_2D_ARRAY, externTexture);
		return true;
	}

	return false;
}

bool CGLES3MaterialPass::SetTextureCubeMap(const char *szName, uint32_t externTexture)
{
	uint32_t name = HashValue(szName);

	if ((m_pPipeline == nullptr) || (m_pPipeline && m_pPipeline->IsTextureValid(name))) {
		m_ptrTextureCubeMaps[name] = GLES3Renderer()->NewTextureCubeMap(TEXTURE_INTERNAL_NAME(name));
		((CGLES3TextureBase *)m_ptrTextureCubeMaps[name].GetPointer())->Create(GL_TEXTURE_CUBE_MAP, externTexture);
		return true;
	}

	return false;
}

bool CGLES3MaterialPass::SetTexture2D(const char *szName, const char *szFileName)
{
	uint32_t name = HashValue(szName);

	if ((m_pPipeline == nullptr) || (m_pPipeline && m_pPipeline->IsTextureValid(name))) {
		m_ptrTexture2Ds[name] = GLES3Renderer()->NewTexture2D(szFileName);
		return true;
	}

	return false;
}

bool CGLES3MaterialPass::SetTexture2DArray(const char *szName, const char *szFileName)
{
	uint32_t name = HashValue(szName);

	if ((m_pPipeline == nullptr) || (m_pPipeline && m_pPipeline->IsTextureValid(name))) {
		m_ptrTexture2DArrays[name] = GLES3Renderer()->NewTexture2DArray(szFileName);
		return true;
	}

	return false;
}

bool CGLES3MaterialPass::SetTextureCubeMap(const char *szName, const char *szFileName)
{
	uint32_t name = HashValue(szName);

	if ((m_pPipeline == nullptr) || (m_pPipeline && m_pPipeline->IsTextureValid(name))) {
		m_ptrTextureCubeMaps[name] = GLES3Renderer()->NewTextureCubeMap(szFileName);
		return true;
	}

	return false;
}

bool CGLES3MaterialPass::SetUniformVec1(const char *szName, float v0)
{
	uint32_t name = HashValue(szName);

	if ((m_pPipeline == nullptr) || (m_pPipeline && m_pPipeline->IsUniformBlockValid(name))) {
		if (m_pUniformVec1s[name] == nullptr) {
			m_pUniformVec1s[name] = new CGfxUniformVec1(true);
		}

		m_pUniformVec1s[name]->SetValue(v0);
		return true;
	}

	return false;
}

bool CGLES3MaterialPass::SetUniformVec2(const char *szName, float v0, float v1)
{
	uint32_t name = HashValue(szName);

	if ((m_pPipeline == nullptr) || (m_pPipeline && m_pPipeline->IsUniformBlockValid(name))) {
		if (m_pUniformVec2s[name] == nullptr) {
			m_pUniformVec2s[name] = new CGfxUniformVec2(true);
		}

		m_pUniformVec2s[name]->SetValue(v0, v1);
		return true;
	}

	return false;
}

bool CGLES3MaterialPass::SetUniformVec3(const char *szName, float v0, float v1, float v2)
{
	uint32_t name = HashValue(szName);

	if ((m_pPipeline == nullptr) || (m_pPipeline && m_pPipeline->IsUniformBlockValid(name))) {
		if (m_pUniformVec3s[name] == nullptr) {
			m_pUniformVec3s[name] = new CGfxUniformVec3(false);
		}

		m_pUniformVec3s[name]->SetValue(v0, v1, v2);
		return true;
	}

	return false;
}

bool CGLES3MaterialPass::SetUniformVec4(const char *szName, float v0, float v1, float v2, float v3)
{
	uint32_t name = HashValue(szName);

	if ((m_pPipeline == nullptr) || (m_pPipeline && m_pPipeline->IsUniformBlockValid(name))) {
		if (m_pUniformVec4s[name] == nullptr) {
			m_pUniformVec4s[name] = new CGfxUniformVec4(false);
		}

		m_pUniformVec4s[name]->SetValue(v0, v1, v2, v3);
		return true;
	}

	return false;
}

bool CGLES3MaterialPass::SetUniformMat4(const char *szName, const float *value)
{
	uint32_t name = HashValue(szName);

	if ((m_pPipeline == nullptr) || (m_pPipeline && m_pPipeline->IsUniformBlockValid(name))) {
		if (m_pUniformMat4s[name] == nullptr) {
			m_pUniformMat4s[name] = new CGfxUniformMat4(false);
		}

		m_pUniformMat4s[name]->SetValue(value);
		return true;
	}

	return false;
}

CGfxPipelineGraphics* CGLES3MaterialPass::GetPipeline(void) const
{
	return m_pPipeline;
}

void CGLES3MaterialPass::Bind(CGLES3PipelineBase *pPipeline, const CGLES3MaterialPass *pPass, uint32_t &indexTexUnit)
{
	BindUniforms(pPipeline, pPass);
	BindTextures(pPipeline, pPass, indexTexUnit);
}

void CGLES3MaterialPass::BindUniforms(CGLES3PipelineBase *pPipeline, const CGLES3MaterialPass *pPass)
{
	if (pPipeline) {
		for (const auto &itUniform : pPass->m_pUniformVec1s) {
			itUniform.second->Apply();
			pPipeline->BindUniformBuffer(itUniform.first, (CGLES3UniformBuffer *)itUniform.second->GetUniformBuffer().GetPointer(), itUniform.second->GetUniformBuffer()->GetSize());
		}

		for (const auto &itUniform : pPass->m_pUniformVec2s) {
			itUniform.second->Apply();
			pPipeline->BindUniformBuffer(itUniform.first, (CGLES3UniformBuffer *)itUniform.second->GetUniformBuffer().GetPointer(), itUniform.second->GetUniformBuffer()->GetSize());
		}

		for (const auto &itUniform : pPass->m_pUniformVec3s) {
			itUniform.second->Apply();
			pPipeline->BindUniformBuffer(itUniform.first, (CGLES3UniformBuffer *)itUniform.second->GetUniformBuffer().GetPointer(), itUniform.second->GetUniformBuffer()->GetSize());
		}

		for (const auto &itUniform : pPass->m_pUniformVec4s) {
			itUniform.second->Apply();
			pPipeline->BindUniformBuffer(itUniform.first, (CGLES3UniformBuffer *)itUniform.second->GetUniformBuffer().GetPointer(), itUniform.second->GetUniformBuffer()->GetSize());
		}

		for (const auto &itUniform : pPass->m_pUniformMat4s) {
			itUniform.second->Apply();
			pPipeline->BindUniformBuffer(itUniform.first, (CGLES3UniformBuffer *)itUniform.second->GetUniformBuffer().GetPointer(), itUniform.second->GetUniformBuffer()->GetSize());
		}
	}
}

void CGLES3MaterialPass::BindTextures(CGLES3PipelineBase *pPipeline, const CGLES3MaterialPass *pPass, uint32_t &indexTexUnit)
{
	if (pPipeline) {
		for (const auto &itTexture : pPass->m_ptrTexture2Ds) {
			if (pPipeline->BindTexture(itTexture.first, (CGLES3Texture2D *)itTexture.second.GetPointer(), (CGLES3Sampler *)pPass->m_pSamplers.find(itTexture.first)->second, indexTexUnit)) {
				indexTexUnit++;
			}
		}

		for (const auto &itTexture : pPass->m_ptrTexture2DArrays) {
			if (pPipeline->BindTexture(itTexture.first, (CGLES3Texture2DArray *)itTexture.second.GetPointer(), (CGLES3Sampler *)pPass->m_pSamplers.find(itTexture.first)->second, indexTexUnit)) {
				indexTexUnit++;
			}
		}

		for (const auto &itTexture : pPass->m_ptrTextureCubeMaps) {
			if (pPipeline->BindTexture(itTexture.first, (CGLES3TextureCubeMap *)itTexture.second.GetPointer(), (CGLES3Sampler *)pPass->m_pSamplers.find(itTexture.first)->second, indexTexUnit)) {
				indexTexUnit++;
			}
		}
	}
}
