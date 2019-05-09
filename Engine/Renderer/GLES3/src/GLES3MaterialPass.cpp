#include "GLES3Renderer.h"


#define TEXTURE_INTERNAL_NAME(name) (uint32_t)(name ^ (size_t)this)


CGLES3MaterialPass::CGLES3MaterialPass(uint32_t name)
	: CGfxMaterialPass(name)
	, m_pPipeline(nullptr)
{

}

CGLES3MaterialPass::~CGLES3MaterialPass(void)
{
	for (auto& itUniform : m_pUniformVec1s) {
		delete itUniform.second;
	}

	for (auto& itUniform : m_pUniformVec2s) {
		delete itUniform.second;
	}

	for (auto& itUniform : m_pUniformVec3s) {
		delete itUniform.second;
	}

	for (auto& itUniform : m_pUniformVec4s) {
		delete itUniform.second;
	}

	for (auto& itUniform : m_pUniformMat4s) {
		delete itUniform.second;
	}
}

const CGfxDescriptorSetPtr CGLES3MaterialPass::GetDescriptorSet(void) const
{
	return m_ptrDescriptorSet;
}

bool CGLES3MaterialPass::SetPipeline(const CGfxRenderPass* pRenderPass, const CGfxShader* pVertexShader, const CGfxShader* pFragmentShader, const PipelineState& state, int indexSubpass, int vertexBinding, int instanceBinding)
{
	ASSERT(pRenderPass);
	ASSERT(pVertexShader);
	ASSERT(pVertexShader->IsValid());
	ASSERT(pVertexShader->GetKind() == vertex_shader);
	ASSERT(pFragmentShader);
	ASSERT(pFragmentShader->IsValid());
	ASSERT(pFragmentShader->GetKind() == fragment_shader);

	m_pPipeline = GLES3Renderer()->CreatePipelineGraphics(pRenderPass, pVertexShader, pFragmentShader, state, indexSubpass, vertexBinding, instanceBinding);
	m_ptrDescriptorSet = GLES3Renderer()->NewDescriptorSet(m_pPipeline->GetDescriptorLayout(DESCRIPTOR_SET_PASS));

	return true;
}

const CGfxPipelineGraphics* CGLES3MaterialPass::GetPipeline(void) const
{
	ASSERT(m_pPipeline);
	return m_pPipeline;
}

bool CGLES3MaterialPass::SetSampler(uint32_t name, GfxFilter minFilter, GfxFilter magFilter, GfxSamplerMipmapMode mipmapMode, GfxSamplerAddressMode addressMode)
{
	ASSERT(m_ptrDescriptorSet);

	if (m_ptrDescriptorSet->GetDescriptorLayout()->IsSampledImageValid(name)) {
		m_pSamplers[name] = GLES3Renderer()->CreateSampler(minFilter, magFilter, mipmapMode, addressMode);
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3MaterialPass::SetTexture2D(uint32_t name, const CGfxTexture2DPtr ptrTexture)
{
	ASSERT(m_ptrDescriptorSet);

	if (m_ptrDescriptorSet->GetDescriptorLayout()->IsSampledImageValid(name)) {
		return m_ptrDescriptorSet->SetTexture2D(name, ptrTexture, m_pSamplers[name]);
	}
	else {
		return false;
	}
}

bool CGLES3MaterialPass::SetTexture2DArray(uint32_t name, const CGfxTexture2DArrayPtr ptrTexture)
{
	ASSERT(m_ptrDescriptorSet);

	if (m_ptrDescriptorSet->GetDescriptorLayout()->IsSampledImageValid(name)) {
		return m_ptrDescriptorSet->SetTexture2DArray(name, ptrTexture, m_pSamplers[name]);
	}
	else {
		return false;
	}
}

bool CGLES3MaterialPass::SetTextureCubemap(uint32_t name, const CGfxTextureCubemapPtr ptrTexture)
{
	ASSERT(m_ptrDescriptorSet);

	if (m_ptrDescriptorSet->GetDescriptorLayout()->IsSampledImageValid(name)) {
		return m_ptrDescriptorSet->SetTextureCubemap(name, ptrTexture, m_pSamplers[name]);
	}
	else {
		return false;
	}
}

bool CGLES3MaterialPass::SetTexture2D(uint32_t name, const char* szFileName, int baseLevel, int numLevels)
{
	ASSERT(m_ptrDescriptorSet);

	if (m_ptrDescriptorSet->GetDescriptorLayout()->IsSampledImageValid(name)) {
		return m_ptrDescriptorSet->SetTexture2D(name, GLES3Renderer()->NewTexture2D(szFileName, baseLevel, numLevels), m_pSamplers[name]);
	}
	else {
		return false;
	}
}

bool CGLES3MaterialPass::SetTexture2DArray(uint32_t name, const char* szFileName, int baseLevel, int numLevels)
{
	ASSERT(m_ptrDescriptorSet);

	if (m_ptrDescriptorSet->GetDescriptorLayout()->IsSampledImageValid(name)) {
		return m_ptrDescriptorSet->SetTexture2DArray(name, GLES3Renderer()->NewTexture2DArray(szFileName, baseLevel, numLevels), m_pSamplers[name]);
	}
	else {
		return false;
	}
}

bool CGLES3MaterialPass::SetTextureCubemap(uint32_t name, const char* szFileName, int baseLevel, int numLevels)
{
	ASSERT(m_ptrDescriptorSet);

	if (m_ptrDescriptorSet->GetDescriptorLayout()->IsSampledImageValid(name)) {
		return m_ptrDescriptorSet->SetTextureCubemap(name, GLES3Renderer()->NewTextureCubemap(szFileName, baseLevel, numLevels), m_pSamplers[name]);
	}
	else {
		return false;
	}
}

bool CGLES3MaterialPass::SetUniformVec1(uint32_t name, float v0)
{
	ASSERT(m_ptrDescriptorSet);

	if (m_ptrDescriptorSet->GetDescriptorLayout()->IsUniformBlockValid(name)) {
		if (m_pUniformVec1s[name] == nullptr) {
			m_pUniformVec1s[name] = new CGfxUniformVec1;
			m_ptrDescriptorSet->SetUniformBuffer(name, m_pUniformVec1s[name]->GetUniformBuffer(), 0, m_pUniformVec1s[name]->GetUniformBuffer()->GetSize());
		}

		m_pUniformVec1s[name]->SetValue(v0);
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3MaterialPass::SetUniformVec2(uint32_t name, float v0, float v1)
{
	ASSERT(m_ptrDescriptorSet);

	if (m_ptrDescriptorSet->GetDescriptorLayout()->IsUniformBlockValid(name)) {
		if (m_pUniformVec2s[name] == nullptr) {
			m_pUniformVec2s[name] = new CGfxUniformVec2;
			m_ptrDescriptorSet->SetUniformBuffer(name, m_pUniformVec2s[name]->GetUniformBuffer(), 0, m_pUniformVec2s[name]->GetUniformBuffer()->GetSize());
		}

		m_pUniformVec2s[name]->SetValue(v0, v1);
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3MaterialPass::SetUniformVec3(uint32_t name, float v0, float v1, float v2)
{
	ASSERT(m_ptrDescriptorSet);

	if (m_ptrDescriptorSet->GetDescriptorLayout()->IsUniformBlockValid(name)) {
		if (m_pUniformVec3s[name] == nullptr) {
			m_pUniformVec3s[name] = new CGfxUniformVec3;
			m_ptrDescriptorSet->SetUniformBuffer(name, m_pUniformVec3s[name]->GetUniformBuffer(), 0, m_pUniformVec3s[name]->GetUniformBuffer()->GetSize());
		}

		m_pUniformVec3s[name]->SetValue(v0, v1, v2);
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3MaterialPass::SetUniformVec4(uint32_t name, float v0, float v1, float v2, float v3)
{
	ASSERT(m_ptrDescriptorSet);

	if (m_ptrDescriptorSet->GetDescriptorLayout()->IsUniformBlockValid(name)) {
		if (m_pUniformVec4s[name] == nullptr) {
			m_pUniformVec4s[name] = new CGfxUniformVec4;
			m_ptrDescriptorSet->SetUniformBuffer(name, m_pUniformVec4s[name]->GetUniformBuffer(), 0, m_pUniformVec4s[name]->GetUniformBuffer()->GetSize());
		}

		m_pUniformVec4s[name]->SetValue(v0, v1, v2, v3);
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3MaterialPass::SetUniformMat4(uint32_t name, const float* value)
{
	ASSERT(m_ptrDescriptorSet);

	if (m_ptrDescriptorSet->GetDescriptorLayout()->IsUniformBlockValid(name)) {
		if (m_pUniformMat4s[name] == nullptr) {
			m_pUniformMat4s[name] = new CGfxUniformMat4;
			m_ptrDescriptorSet->SetUniformBuffer(name, m_pUniformMat4s[name]->GetUniformBuffer(), 0, m_pUniformMat4s[name]->GetUniformBuffer()->GetSize());
		}

		m_pUniformMat4s[name]->SetValue(value);
		return true;
	}
	else {
		return false;
	}
}

void CGLES3MaterialPass::Bind(const CGLES3PipelineGraphics* pPipeline) const
{
	ASSERT(pPipeline);
	ASSERT(m_ptrDescriptorSet);

	pPipeline->BindDescriptorSet(m_ptrDescriptorSet);
}
