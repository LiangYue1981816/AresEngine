#include "VKRenderer.h"


CVKMaterialPass::CVKMaterialPass(CVKDevice* pDevice, uint32_t name)
	: CGfxMaterialPass(name)
	, m_pDevice(pDevice)
	, m_pPipeline(nullptr)
{

}

CVKMaterialPass::~CVKMaterialPass(void)
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

const CGfxDescriptorSetPtr CVKMaterialPass::GetDescriptorSet(void) const
{
	ASSERT(m_ptrDescriptorSet);
	return m_ptrDescriptorSet;
}

bool CVKMaterialPass::SetPipeline(const CGfxRenderPass* pRenderPass, const CGfxShader* pVertexShader, const CGfxShader* pFragmentShader, const PipelineState& state, int indexSubpass, int vertexBinding, int instanceBinding)
{
	m_pPipeline = VKRenderer()->CreatePipelineGraphics(pRenderPass, pVertexShader, pFragmentShader, state, indexSubpass, vertexBinding, instanceBinding);
	m_ptrDescriptorSet = VKRenderer()->NewDescriptorSet(HashValueFormat("%p_%x", this, GetName()), m_pPipeline->GetDescriptorLayout(DESCRIPTOR_SET_MATPASS));
	return true;
}

const CGfxPipelineGraphics* CVKMaterialPass::GetPipeline(void) const
{
	ASSERT(m_pPipeline);
	return m_pPipeline;
}

bool CVKMaterialPass::SetSampler(uint32_t name, GfxFilter minFilter, GfxFilter magFilter, GfxSamplerMipmapMode mipmapMode, GfxSamplerAddressMode addressMode)
{
	ASSERT(m_pPipeline);
	ASSERT(m_ptrDescriptorSet);

	if (m_ptrDescriptorSet->GetDescriptorLayout()->IsSampledImageValid(name)) {
		m_pSamplers[name] = VKRenderer()->CreateSampler(minFilter, magFilter, mipmapMode, addressMode);
		return true;
	}
	else {
		return false;
	}
}

bool CVKMaterialPass::SetTexture2D(uint32_t name, const CGfxTexture2DPtr ptrTexture)
{
	ASSERT(m_pPipeline);
	ASSERT(m_ptrDescriptorSet);

	if (m_ptrDescriptorSet->GetDescriptorLayout()->IsSampledImageValid(name)) {
		m_ptrDescriptorSet->SetTexture2D(name, ptrTexture, m_pSamplers[name]);
		m_ptrDescriptorSet->Update();
		return true;
	}
	else {
		return false;
	}
}

bool CVKMaterialPass::SetTexture2DArray(uint32_t name, const CGfxTexture2DArrayPtr ptrTexture)
{
	ASSERT(m_pPipeline);
	ASSERT(m_ptrDescriptorSet);

	if (m_ptrDescriptorSet->GetDescriptorLayout()->IsSampledImageValid(name)) {
		m_ptrDescriptorSet->SetTexture2DArray(name, ptrTexture, m_pSamplers[name]);
		m_ptrDescriptorSet->Update();
		return true;
	}
	else {
		return false;
	}
}

bool CVKMaterialPass::SetTextureCubemap(uint32_t name, const CGfxTextureCubemapPtr ptrTexture)
{
	ASSERT(m_pPipeline);
	ASSERT(m_ptrDescriptorSet);

	if (m_ptrDescriptorSet->GetDescriptorLayout()->IsSampledImageValid(name)) {
		m_ptrDescriptorSet->SetTextureCubemap(name, ptrTexture, m_pSamplers[name]);
		m_ptrDescriptorSet->Update();
		return true;
	}
	else {
		return false;
	}
}

bool CVKMaterialPass::SetTexture2D(uint32_t name, const char* szFileName, int baseLevel, int numLevels)
{
	ASSERT(m_pPipeline);
	ASSERT(m_ptrDescriptorSet);

	if (m_ptrDescriptorSet->GetDescriptorLayout()->IsSampledImageValid(name)) {
		m_ptrDescriptorSet->SetTexture2D(name, VKRenderer()->NewTexture2D(szFileName, baseLevel, numLevels), m_pSamplers[name]);
		m_ptrDescriptorSet->Update();
		return true;
	}
	else {
		return false;
	}
}

bool CVKMaterialPass::SetTexture2DArray(uint32_t name, const char* szFileName, int baseLevel, int numLevels)
{
	ASSERT(m_pPipeline);
	ASSERT(m_ptrDescriptorSet);

	if (m_ptrDescriptorSet->GetDescriptorLayout()->IsSampledImageValid(name)) {
		m_ptrDescriptorSet->SetTexture2DArray(name, VKRenderer()->NewTexture2DArray(szFileName, baseLevel, numLevels), m_pSamplers[name]);
		m_ptrDescriptorSet->Update();
		return true;
	}
	else {
		return false;
	}
}

bool CVKMaterialPass::SetTextureCubemap(uint32_t name, const char* szFileName, int baseLevel, int numLevels)
{
	ASSERT(m_pPipeline);
	ASSERT(m_ptrDescriptorSet);

	if (m_ptrDescriptorSet->GetDescriptorLayout()->IsSampledImageValid(name)) {
		m_ptrDescriptorSet->SetTextureCubemap(name, VKRenderer()->NewTextureCubemap(szFileName, baseLevel, numLevels), m_pSamplers[name]);
		m_ptrDescriptorSet->Update();
		return true;
	}
	else {
		return false;
	}
}

bool CVKMaterialPass::SetUniformVec1(uint32_t name, float v0)
{
	ASSERT(m_pPipeline);
	ASSERT(m_ptrDescriptorSet);

	if (m_ptrDescriptorSet->GetDescriptorLayout()->IsUniformBlockValid(name)) {
		if (m_pUniformVec1s[name] == nullptr) {
			m_pUniformVec1s[name] = new CGfxUniformVec1;
			m_ptrDescriptorSet->SetUniformBuffer(name, m_pUniformVec1s[name]->GetUniformBuffer(), 0, m_pUniformVec1s[name]->GetUniformBuffer()->GetSize());
			m_ptrDescriptorSet->Update();
		}

		m_pUniformVec1s[name]->SetValue(v0);
		return true;
	}
	else {
		return false;
	}
}

bool CVKMaterialPass::SetUniformVec2(uint32_t name, float v0, float v1)
{
	ASSERT(m_pPipeline);
	ASSERT(m_ptrDescriptorSet);

	if (m_ptrDescriptorSet->GetDescriptorLayout()->IsUniformBlockValid(name)) {
		if (m_pUniformVec2s[name] == nullptr) {
			m_pUniformVec2s[name] = new CGfxUniformVec2;
			m_ptrDescriptorSet->SetUniformBuffer(name, m_pUniformVec2s[name]->GetUniformBuffer(), 0, m_pUniformVec2s[name]->GetUniformBuffer()->GetSize());
			m_ptrDescriptorSet->Update();
		}

		m_pUniformVec2s[name]->SetValue(v0, v1);
		return true;
	}
	else {
		return false;
	}
}

bool CVKMaterialPass::SetUniformVec3(uint32_t name, float v0, float v1, float v2)
{
	ASSERT(m_pPipeline);
	ASSERT(m_ptrDescriptorSet);

	if (m_ptrDescriptorSet->GetDescriptorLayout()->IsUniformBlockValid(name)) {
		if (m_pUniformVec3s[name] == nullptr) {
			m_pUniformVec3s[name] = new CGfxUniformVec3;
			m_ptrDescriptorSet->SetUniformBuffer(name, m_pUniformVec3s[name]->GetUniformBuffer(), 0, m_pUniformVec3s[name]->GetUniformBuffer()->GetSize());
			m_ptrDescriptorSet->Update();
		}

		m_pUniformVec3s[name]->SetValue(v0, v1, v2);
		return true;
	}
	else {
		return false;
	}
}

bool CVKMaterialPass::SetUniformVec4(uint32_t name, float v0, float v1, float v2, float v3)
{
	ASSERT(m_pPipeline);
	ASSERT(m_ptrDescriptorSet);

	if (m_ptrDescriptorSet->GetDescriptorLayout()->IsUniformBlockValid(name)) {
		if (m_pUniformVec4s[name] == nullptr) {
			m_pUniformVec4s[name] = new CGfxUniformVec4;
			m_ptrDescriptorSet->SetUniformBuffer(name, m_pUniformVec4s[name]->GetUniformBuffer(), 0, m_pUniformVec4s[name]->GetUniformBuffer()->GetSize());
			m_ptrDescriptorSet->Update();
		}

		m_pUniformVec4s[name]->SetValue(v0, v1, v2, v3);
		return true;
	}
	else {
		return false;
	}
}

void CVKMaterialPass::Bind(VkCommandBuffer vkCommandBuffer) const
{
	ASSERT(m_pPipeline);
	ASSERT(m_ptrDescriptorSet);

	((CVKDescriptorSet*)m_ptrDescriptorSet.GetPointer())->Bind(vkCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ((CVKPipelineGraphics*)m_pPipeline)->GetPipelineLayout());
}
