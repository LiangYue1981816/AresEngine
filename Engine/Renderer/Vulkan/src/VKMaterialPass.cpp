#include "VKRenderer.h"


CVKMaterialPass::CVKMaterialPass(CVKDevice* pDevice, uint32_t name)
	: CGfxMaterialPass(name)
	, m_pDevice(pDevice)
	, m_pPipeline(nullptr)
{

}

CVKMaterialPass::CVKMaterialPass(CVKDevice* pDevice, uint32_t name, const CVKMaterialPass* pPassCopyFrom)
	: CGfxMaterialPass(name)
	, m_pDevice(pDevice)
	, m_pPipeline(nullptr)
{
	m_pPipeline = ((CVKMaterialPass *)pPassCopyFrom)->m_pPipeline;
	m_pSamplers = ((CVKMaterialPass *)pPassCopyFrom)->m_pSamplers;

	m_ptrDescriptorSet = VKRenderer()->NewDescriptorSet(HashValueFormat("%p_%x", this, GetName()), pPassCopyFrom->GetDescriptorSet());
	{
		for (const auto& itUniform : ((CVKMaterialPass *)pPassCopyFrom)->m_pUniformVec1s) {
			m_pUniformVec1s[itUniform.first] = new CGfxUniformVec1;
			m_pUniformVec1s[itUniform.first]->SetValue(itUniform.second->GetValue());
			m_ptrDescriptorSet->SetUniformBuffer(itUniform.first, m_pUniformVec1s[itUniform.first]->GetUniformBuffer(), 0, m_pUniformVec1s[itUniform.first]->GetUniformBuffer()->GetSize());
		}

		for (const auto& itUniform : ((CVKMaterialPass *)pPassCopyFrom)->m_pUniformVec2s) {
			m_pUniformVec2s[itUniform.first] = new CGfxUniformVec2;
			m_pUniformVec2s[itUniform.first]->SetValue(itUniform.second->GetValue().x, itUniform.second->GetValue().y);
			m_ptrDescriptorSet->SetUniformBuffer(itUniform.first, m_pUniformVec2s[itUniform.first]->GetUniformBuffer(), 0, m_pUniformVec2s[itUniform.first]->GetUniformBuffer()->GetSize());
		}

		for (const auto& itUniform : ((CVKMaterialPass *)pPassCopyFrom)->m_pUniformVec3s) {
			m_pUniformVec3s[itUniform.first] = new CGfxUniformVec3;
			m_pUniformVec3s[itUniform.first]->SetValue(itUniform.second->GetValue().x, itUniform.second->GetValue().y, itUniform.second->GetValue().z);
			m_ptrDescriptorSet->SetUniformBuffer(itUniform.first, m_pUniformVec3s[itUniform.first]->GetUniformBuffer(), 0, m_pUniformVec3s[itUniform.first]->GetUniformBuffer()->GetSize());
		}

		for (const auto& itUniform : ((CVKMaterialPass *)pPassCopyFrom)->m_pUniformVec4s) {
			m_pUniformVec4s[itUniform.first] = new CGfxUniformVec4;
			m_pUniformVec4s[itUniform.first]->SetValue(itUniform.second->GetValue().x, itUniform.second->GetValue().y, itUniform.second->GetValue().z, itUniform.second->GetValue().w);
			m_ptrDescriptorSet->SetUniformBuffer(itUniform.first, m_pUniformVec4s[itUniform.first]->GetUniformBuffer(), 0, m_pUniformVec4s[itUniform.first]->GetUniformBuffer()->GetSize());
		}
	}
}

CVKMaterialPass::~CVKMaterialPass(void)
{
	for (const auto& itUniform : m_pUniformVec1s) {
		delete itUniform.second;
	}

	for (const auto& itUniform : m_pUniformVec2s) {
		delete itUniform.second;
	}

	for (const auto& itUniform : m_pUniformVec3s) {
		delete itUniform.second;
	}

	for (const auto& itUniform : m_pUniformVec4s) {
		delete itUniform.second;
	}

	for (const auto& itUniform : m_pUniformMat4s) {
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
