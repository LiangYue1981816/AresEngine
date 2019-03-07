#include "GLES3Renderer.h"


#define TEXTURE_INTERNAL_NAME(name) (uint32_t)(name ^ (size_t)this)


CGLES3MaterialPass::CGLES3MaterialPass(uint32_t name)
	: CGfxMaterialPass(name)
	, m_name(name)

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
}

uint32_t CGLES3MaterialPass::GetName(void) const
{
	return m_name;
}

bool CGLES3MaterialPass::SetPipeline(const CGfxRenderPass *pRenderPass, const CGfxShader *pVertexShader, const CGfxShader *pFragmentShader, const PipelineState &state, uint32_t indexSubpass, uint32_t vertexBinding, uint32_t instanceBinding)
{
	if (pRenderPass == nullptr) {
		return false;
	}

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

	m_pPipeline = GLES3Renderer()->CreatePipelineGraphics(pRenderPass, pVertexShader, pFragmentShader, state, indexSubpass, vertexBinding, instanceBinding);
	return true;
}

bool CGLES3MaterialPass::SetSampler(const char *szName, GfxFilter minFilter, GfxFilter magFilter, GfxSamplerMipmapMode mipmapMode, GfxSamplerAddressMode addressMode)
{
	uint32_t name = HashValue(szName);

	if ((m_pPipeline == nullptr) || (m_pPipeline && m_pPipeline->IsTextureValid(name))) {
		m_pSamplers[name] = GLES3Renderer()->CreateSampler(minFilter, magFilter, mipmapMode, addressMode);
		return true;
	}

	return false;
}

bool CGLES3MaterialPass::SetTexture2D(const char *szName, HANDLE hExternTexture)
{
	uint32_t name = HashValue(szName);

	if ((m_pPipeline == nullptr) || (m_pPipeline && m_pPipeline->IsTextureValid(name))) {
		m_ptrTexture2Ds[name] = GLES3Renderer()->NewTexture2D(TEXTURE_INTERNAL_NAME(name));
		m_ptrTexture2Ds[name]->Create(hExternTexture);
		return true;
	}

	return false;
}

bool CGLES3MaterialPass::SetTexture2DArray(const char *szName, HANDLE hExternTexture)
{
	uint32_t name = HashValue(szName);

	if ((m_pPipeline == nullptr) || (m_pPipeline && m_pPipeline->IsTextureValid(name))) {
		m_ptrTexture2DArrays[name] = GLES3Renderer()->NewTexture2DArray(TEXTURE_INTERNAL_NAME(name));
		m_ptrTexture2DArrays[name]->Create(hExternTexture);
		return true;
	}

	return false;
}

bool CGLES3MaterialPass::SetTextureCubeMap(const char *szName, HANDLE hExternTexture)
{
	uint32_t name = HashValue(szName);

	if ((m_pPipeline == nullptr) || (m_pPipeline && m_pPipeline->IsTextureValid(name))) {
		m_ptrTextureCubeMaps[name] = GLES3Renderer()->NewTextureCubeMap(TEXTURE_INTERNAL_NAME(name));
		m_ptrTextureCubeMaps[name]->Create(hExternTexture);
		return true;
	}

	return false;
}

bool CGLES3MaterialPass::SetTexture2D(const char *szName, const CGfxTexture2DPtr ptrTexture)
{
	uint32_t name = HashValue(szName);

	if ((m_pPipeline == nullptr) || (m_pPipeline && m_pPipeline->IsTextureValid(name))) {
		m_ptrTexture2Ds[name] = ptrTexture;
		return true;
	}

	return false;
}

bool CGLES3MaterialPass::SetTexture2DArray(const char *szName, const CGfxTexture2DArrayPtr ptrTexture)
{
	uint32_t name = HashValue(szName);

	if ((m_pPipeline == nullptr) || (m_pPipeline && m_pPipeline->IsTextureValid(name))) {
		m_ptrTexture2DArrays[name] = ptrTexture;
		return true;
	}

	return false;
}

bool CGLES3MaterialPass::SetTextureCubeMap(const char *szName, const CGfxTextureCubeMapPtr ptrTexture)
{
	uint32_t name = HashValue(szName);

	if ((m_pPipeline == nullptr) || (m_pPipeline && m_pPipeline->IsTextureValid(name))) {
		m_ptrTextureCubeMaps[name] = ptrTexture;
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
			m_pUniformVec1s[name] = new CGfxUniformVec1;
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
			m_pUniformVec2s[name] = new CGfxUniformVec2;
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
			m_pUniformVec3s[name] = new CGfxUniformVec3;
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
			m_pUniformVec4s[name] = new CGfxUniformVec4;
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
			m_pUniformMat4s[name] = new CGfxUniformMat4;
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

CGfxTexture2DPtr CGLES3MaterialPass::GetTexture2D(const char *szName) const
{
	const auto &itTexture = m_ptrTexture2Ds.find(HashValue(szName));

	if (itTexture != m_ptrTexture2Ds.end()) {
		return itTexture->second;
	}
	else {
		return nullptr;
	}
}

CGfxTexture2DArrayPtr CGLES3MaterialPass::GetTexture2DArray(const char *szName) const
{
	const auto &itTexture = m_ptrTexture2DArrays.find(HashValue(szName));

	if (itTexture != m_ptrTexture2DArrays.end()) {
		return itTexture->second;
	}
	else {
		return nullptr;
	}
}

CGfxTextureCubeMapPtr CGLES3MaterialPass::GetTextureCubeMap(const char *szName) const
{
	const auto &itTexture = m_ptrTextureCubeMaps.find(HashValue(szName));

	if (itTexture != m_ptrTextureCubeMaps.end()) {
		return itTexture->second;
	}
	else {
		return nullptr;
	}
}

void CGLES3MaterialPass::Bind(const CGLES3Pipeline *pPipeline, const CGLES3MaterialPass *pPass, uint32_t &indexTexUnit)
{
	BindUniforms(pPipeline, pPass);
	BindTextures(pPipeline, pPass, indexTexUnit);
}

void CGLES3MaterialPass::BindUniforms(const CGLES3Pipeline *pPipeline, const CGLES3MaterialPass *pPass)
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

void CGLES3MaterialPass::BindTextures(const CGLES3Pipeline *pPipeline, const CGLES3MaterialPass *pPass, uint32_t &indexTexUnit)
{
	if (pPipeline) {
		for (const auto &itTexture : pPass->m_ptrTexture2Ds) {
			if (pPipeline->IsTextureValid(itTexture.first)) {
				pPipeline->BindTexture(itTexture.first, (CGLES3Texture2D *)itTexture.second.GetPointer(), (CGLES3Sampler *)pPass->m_pSamplers.find(itTexture.first)->second, indexTexUnit);
				indexTexUnit++;
			}
		}

		for (const auto &itTexture : pPass->m_ptrTexture2DArrays) {
			if (pPipeline->IsTextureValid(itTexture.first)) {
				pPipeline->BindTexture(itTexture.first, (CGLES3Texture2DArray *)itTexture.second.GetPointer(), (CGLES3Sampler *)pPass->m_pSamplers.find(itTexture.first)->second, indexTexUnit);
				indexTexUnit++;
			}
		}

		for (const auto &itTexture : pPass->m_ptrTextureCubeMaps) {
			if (pPipeline->IsTextureValid(itTexture.first)) {
				pPipeline->BindTexture(itTexture.first, (CGLES3TextureCubeMap *)itTexture.second.GetPointer(), (CGLES3Sampler *)pPass->m_pSamplers.find(itTexture.first)->second, indexTexUnit);
				indexTexUnit++;
			}
		}
	}
}
