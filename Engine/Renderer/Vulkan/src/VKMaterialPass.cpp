#include "VKRenderer.h"


#define TEXTURE_INTERNAL_NAME(name) (uint32_t)(name ^ (size_t)this)


CVKMaterialPass::CVKMaterialPass(CVKDevice *pDevice, uint32_t name)
	: CGfxMaterialPass(name)
	, m_name(name)
	, m_pDevice(pDevice)

	, m_pPipeline(nullptr)
	, m_pDescriptorSet(nullptr)
{

}

CVKMaterialPass::~CVKMaterialPass(void)
{
	if (m_pDescriptorSet) {
		m_pDevice->GetDescriptorSetManager()->FreeDescriptorSet(m_pDescriptorSet);
	}

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

uint32_t CVKMaterialPass::GetName(void) const
{
	return m_name;
}

CGfxPipelineGraphics* CVKMaterialPass::GetPipeline(void) const
{
	return m_pPipeline;
}

CVKDescriptorSet* CVKMaterialPass::GetDescriptorSet(void) const
{
	return m_pDescriptorSet;
}

bool CVKMaterialPass::SetPipeline(const CGfxRenderPass *pRenderPass, const CGfxShader *pVertexShader, const CGfxShader *pFragmentShader, const PipelineState &state, uint32_t indexSubpass, uint32_t vertexBinding, uint32_t instanceBinding)
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

	m_pPipeline = VKRenderer()->CreatePipelineGraphics(pRenderPass, pVertexShader, pFragmentShader, state, indexSubpass, vertexBinding, instanceBinding);
	m_pDescriptorSet = m_pDevice->GetDescriptorSetManager()->AllocDescriptorSet(((CVKPipelineGraphics *)m_pPipeline)->GetDescriptorLayout(DESCRIPTOR_SET_PASS));

	return true;
}

bool CVKMaterialPass::SetSampler(const char *szName, GfxFilter minFilter, GfxFilter magFilter, GfxSamplerMipmapMode mipmapMode, GfxSamplerAddressMode addressMode)
{
	uint32_t name = HashValue(szName);

	if (m_pDescriptorSet && m_pPipeline && m_pPipeline->IsTextureValid(name)) {
		m_pSamplers[name] = VKRenderer()->CreateSampler(minFilter, magFilter, mipmapMode, addressMode);
		return true;
	}

	return false;
}

bool CVKMaterialPass::SetTexture2D(const char *szName, const CGfxTexture2DPtr ptrTexture)
{
	uint32_t name = HashValue(szName);

	if (m_pDescriptorSet && m_pPipeline && m_pPipeline->IsTextureValid(name)) {
		m_pDescriptorSet->SetTexture2D(name, ptrTexture, m_pSamplers[name]);
		return true;
	}

	return false;
}

bool CVKMaterialPass::SetTexture2DArray(const char *szName, const CGfxTexture2DArrayPtr ptrTexture)
{
	uint32_t name = HashValue(szName);

	if (m_pDescriptorSet && m_pPipeline && m_pPipeline->IsTextureValid(name)) {
		m_pDescriptorSet->SetTexture2DArray(name, ptrTexture, m_pSamplers[name]);
		return true;
	}

	return false;
}

bool CVKMaterialPass::SetTextureCubeMap(const char *szName, const CGfxTextureCubeMapPtr ptrTexture)
{
	uint32_t name = HashValue(szName);

	if (m_pDescriptorSet && m_pPipeline && m_pPipeline->IsTextureValid(name)) {
		m_pDescriptorSet->SetTextureCubeMap(name, ptrTexture, m_pSamplers[name]);
		return true;
	}

	return false;
}

bool CVKMaterialPass::SetTexture2D(const char *szName, const char *szFileName)
{
	uint32_t name = HashValue(szName);

	if (m_pDescriptorSet && m_pPipeline && m_pPipeline->IsTextureValid(name)) {
		m_pDescriptorSet->SetTexture2D(name, VKRenderer()->NewTexture2D(szFileName), m_pSamplers[name]);
		return true;
	}

	return false;
}

bool CVKMaterialPass::SetTexture2DArray(const char *szName, const char *szFileName)
{
	uint32_t name = HashValue(szName);

	if (m_pDescriptorSet && m_pPipeline && m_pPipeline->IsTextureValid(name)) {
		m_pDescriptorSet->SetTexture2DArray(name, VKRenderer()->NewTexture2DArray(szFileName), m_pSamplers[name]);
		return true;
	}

	return false;
}

bool CVKMaterialPass::SetTextureCubeMap(const char *szName, const char *szFileName)
{
	uint32_t name = HashValue(szName);

	if (m_pDescriptorSet && m_pPipeline && m_pPipeline->IsTextureValid(name)) {
		m_pDescriptorSet->SetTextureCubeMap(name, VKRenderer()->NewTextureCubeMap(szFileName), m_pSamplers[name]);
		return true;
	}

	return false;
}

bool CVKMaterialPass::SetUniformVec1(const char *szName, float v0)
{
	uint32_t name = HashValue(szName);

	if (m_pDescriptorSet && m_pPipeline && m_pPipeline->IsUniformBlockValid(name)) {
		if (m_pUniformVec1s[name] == nullptr) {
			m_pUniformVec1s[name] = new CGfxUniformVec1;
			m_pDescriptorSet->SetUniformBuffer(name, m_pUniformVec1s[name]->GetUniformBuffer());
		}

		m_pUniformVec1s[name]->SetValue(v0);

		return true;
	}

	return false;
}

bool CVKMaterialPass::SetUniformVec2(const char *szName, float v0, float v1)
{
	uint32_t name = HashValue(szName);

	if (m_pDescriptorSet && m_pPipeline && m_pPipeline->IsUniformBlockValid(name)) {
		if (m_pUniformVec2s[name] == nullptr) {
			m_pUniformVec2s[name] = new CGfxUniformVec2;
			m_pDescriptorSet->SetUniformBuffer(name, m_pUniformVec2s[name]->GetUniformBuffer());
		}

		m_pUniformVec2s[name]->SetValue(v0, v1);

		return true;
	}

	return false;
}

bool CVKMaterialPass::SetUniformVec3(const char *szName, float v0, float v1, float v2)
{
	uint32_t name = HashValue(szName);

	if (m_pDescriptorSet && m_pPipeline && m_pPipeline->IsUniformBlockValid(name)) {
		if (m_pUniformVec3s[name] == nullptr) {
			m_pUniformVec3s[name] = new CGfxUniformVec3;
			m_pDescriptorSet->SetUniformBuffer(name, m_pUniformVec3s[name]->GetUniformBuffer());
		}

		m_pUniformVec3s[name]->SetValue(v0, v1, v2);

		return true;
	}

	return false;
}

bool CVKMaterialPass::SetUniformVec4(const char *szName, float v0, float v1, float v2, float v3)
{
	uint32_t name = HashValue(szName);

	if (m_pDescriptorSet && m_pPipeline && m_pPipeline->IsUniformBlockValid(name)) {
		if (m_pUniformVec4s[name] == nullptr) {
			m_pUniformVec4s[name] = new CGfxUniformVec4;
			m_pDescriptorSet->SetUniformBuffer(name, m_pUniformVec4s[name]->GetUniformBuffer());
		}

		m_pUniformVec4s[name]->SetValue(v0, v1, v2, v3);

		return true;
	}

	return false;
}

bool CVKMaterialPass::SetUniformMat4(const char *szName, const float *value)
{
	uint32_t name = HashValue(szName);

	if (m_pDescriptorSet && m_pPipeline && m_pPipeline->IsUniformBlockValid(name)) {
		if (m_pUniformMat4s[name] == nullptr) {
			m_pUniformMat4s[name] = new CGfxUniformMat4;
			m_pDescriptorSet->SetUniformBuffer(name, m_pUniformMat4s[name]->GetUniformBuffer());
		}

		m_pUniformMat4s[name]->SetValue(value);

		return true;
	}

	return false;
}

CGfxTexture2DPtr CVKMaterialPass::GetTexture2D(const char *szName) const
{
	if (m_pDescriptorSet) {
		return m_pDescriptorSet->GetTexture2D(HashValue(szName));
	}
	else {
		return nullptr;
	}
}

CGfxTexture2DArrayPtr CVKMaterialPass::GetTexture2DArray(const char *szName) const
{
	if (m_pDescriptorSet) {
		return m_pDescriptorSet->GetTexture2DArray(HashValue(szName));
	}
	else {
		return nullptr;
	}
}

CGfxTextureCubeMapPtr CVKMaterialPass::GetTextureCubeMap(const char *szName) const
{
	if (m_pDescriptorSet) {
		return m_pDescriptorSet->GetTextureCubeMap(HashValue(szName));
	}
	else {
		return nullptr;
	}
}

void CVKMaterialPass::Bind(VkCommandBuffer vkCommandBuffer, const CVKPipeline *pPipeline) const
{
	if (m_pDescriptorSet) {
		m_pDescriptorSet->Bind(vkCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pPipeline->GetPipelineLayout());
	}
}
