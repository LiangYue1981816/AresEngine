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

	if (m_pPipeline && m_pPipeline->IsTextureValid(name)) {
		m_pSamplers[name] = VKRenderer()->CreateSampler(minFilter, magFilter, mipmapMode, addressMode);
		return true;
	}

	return false;
}

bool CVKMaterialPass::SetTexture2D(const char *szName, HANDLE hExternTexture)
{
	uint32_t name = HashValue(szName);

	if (m_pPipeline && m_pPipeline->IsTextureValid(name)) {
		m_ptrTexture2Ds[name] = VKRenderer()->NewTexture2D(TEXTURE_INTERNAL_NAME(name));
		m_ptrTexture2Ds[name]->Create(hExternTexture);

		if (m_pDescriptorSet) {
			m_pDescriptorSet->SetTexture2D(name, m_ptrTexture2Ds[name], m_pSamplers[name]);
		}

		return true;
	}

	return false;
}

bool CVKMaterialPass::SetTexture2DArray(const char *szName, HANDLE hExternTexture)
{
	uint32_t name = HashValue(szName);

	if (m_pPipeline && m_pPipeline->IsTextureValid(name)) {
		m_ptrTexture2DArrays[name] = VKRenderer()->NewTexture2DArray(TEXTURE_INTERNAL_NAME(name));
		m_ptrTexture2DArrays[name]->Create(hExternTexture);

		if (m_pDescriptorSet) {
			m_pDescriptorSet->SetTexture2DArray(name, m_ptrTexture2DArrays[name], m_pSamplers[name]);
		}

		return true;
	}

	return false;
}

bool CVKMaterialPass::SetTextureCubeMap(const char *szName, HANDLE hExternTexture)
{
	uint32_t name = HashValue(szName);

	if (m_pPipeline && m_pPipeline->IsTextureValid(name)) {
		m_ptrTextureCubeMaps[name] = VKRenderer()->NewTextureCubeMap(TEXTURE_INTERNAL_NAME(name));
		m_ptrTextureCubeMaps[name]->Create(hExternTexture);

		if (m_pDescriptorSet) {
			m_pDescriptorSet->SetTextureCubeMap(name, m_ptrTextureCubeMaps[name], m_pSamplers[name]);
		}

		return true;
	}

	return false;
}

bool CVKMaterialPass::SetTexture2D(const char *szName, const CGfxTexture2DPtr ptrTexture)
{
	uint32_t name = HashValue(szName);

	if (m_pPipeline && m_pPipeline->IsTextureValid(name)) {
		m_ptrTexture2Ds[name] = ptrTexture;

		if (m_pDescriptorSet) {
			m_pDescriptorSet->SetTexture2D(name, m_ptrTexture2Ds[name], m_pSamplers[name]);
		}

		return true;
	}

	return false;
}

bool CVKMaterialPass::SetTexture2DArray(const char *szName, const CGfxTexture2DArrayPtr ptrTexture)
{
	uint32_t name = HashValue(szName);

	if (m_pPipeline && m_pPipeline->IsTextureValid(name)) {
		m_ptrTexture2DArrays[name] = ptrTexture;

		if (m_pDescriptorSet) {
			m_pDescriptorSet->SetTexture2DArray(name, m_ptrTexture2DArrays[name], m_pSamplers[name]);
		}

		return true;
	}

	return false;
}

bool CVKMaterialPass::SetTextureCubeMap(const char *szName, const CGfxTextureCubeMapPtr ptrTexture)
{
	uint32_t name = HashValue(szName);

	if (m_pPipeline && m_pPipeline->IsTextureValid(name)) {
		m_ptrTextureCubeMaps[name] = ptrTexture;

		if (m_pDescriptorSet) {
			m_pDescriptorSet->SetTextureCubeMap(name, m_ptrTextureCubeMaps[name], m_pSamplers[name]);
		}

		return true;
	}

	return false;
}

bool CVKMaterialPass::SetTexture2D(const char *szName, const char *szFileName)
{
	uint32_t name = HashValue(szName);

	if (m_pPipeline && m_pPipeline->IsTextureValid(name)) {
		m_ptrTexture2Ds[name] = VKRenderer()->NewTexture2D(szFileName);

		if (m_pDescriptorSet) {
			m_pDescriptorSet->SetTexture2D(name, m_ptrTexture2Ds[name], m_pSamplers[name]);
		}

		return true;
	}

	return false;
}

bool CVKMaterialPass::SetTexture2DArray(const char *szName, const char *szFileName)
{
	uint32_t name = HashValue(szName);

	if (m_pPipeline && m_pPipeline->IsTextureValid(name)) {
		m_ptrTexture2DArrays[name] = VKRenderer()->NewTexture2DArray(szFileName);

		if (m_pDescriptorSet) {
			m_pDescriptorSet->SetTexture2DArray(name, m_ptrTexture2DArrays[name], m_pSamplers[name]);
		}

		return true;
	}

	return false;
}

bool CVKMaterialPass::SetTextureCubeMap(const char *szName, const char *szFileName)
{
	uint32_t name = HashValue(szName);

	if (m_pPipeline && m_pPipeline->IsTextureValid(name)) {
		m_ptrTextureCubeMaps[name] = VKRenderer()->NewTextureCubeMap(szFileName);

		if (m_pDescriptorSet) {
			m_pDescriptorSet->SetTextureCubeMap(name, m_ptrTextureCubeMaps[name], m_pSamplers[name]);
		}

		return true;
	}

	return false;
}

bool CVKMaterialPass::SetUniformVec1(const char *szName, float v0)
{
	uint32_t name = HashValue(szName);

	if (m_pPipeline && m_pPipeline->IsUniformBlockValid(name)) {
		if (m_pUniformVec1s[name] == nullptr) {
			m_pUniformVec1s[name] = new CGfxUniformVec1;

			if (m_pDescriptorSet) {
				m_pDescriptorSet->SetUniformBuffer(name, m_pUniformVec1s[name]->GetUniformBuffer());
			}
		}

		m_pUniformVec1s[name]->SetValue(v0);

		return true;
	}

	return false;
}

bool CVKMaterialPass::SetUniformVec2(const char *szName, float v0, float v1)
{
	uint32_t name = HashValue(szName);

	if (m_pPipeline && m_pPipeline->IsUniformBlockValid(name)) {
		if (m_pUniformVec2s[name] == nullptr) {
			m_pUniformVec2s[name] = new CGfxUniformVec2;

			if (m_pDescriptorSet) {
				m_pDescriptorSet->SetUniformBuffer(name, m_pUniformVec2s[name]->GetUniformBuffer());
			}
		}

		m_pUniformVec2s[name]->SetValue(v0, v1);

		return true;
	}

	return false;
}

bool CVKMaterialPass::SetUniformVec3(const char *szName, float v0, float v1, float v2)
{
	uint32_t name = HashValue(szName);

	if (m_pPipeline && m_pPipeline->IsUniformBlockValid(name)) {
		if (m_pUniformVec3s[name] == nullptr) {
			m_pUniformVec3s[name] = new CGfxUniformVec3;

			if (m_pDescriptorSet) {
				m_pDescriptorSet->SetUniformBuffer(name, m_pUniformVec3s[name]->GetUniformBuffer());
			}
		}

		m_pUniformVec3s[name]->SetValue(v0, v1, v2);

		return true;
	}

	return false;
}

bool CVKMaterialPass::SetUniformVec4(const char *szName, float v0, float v1, float v2, float v3)
{
	uint32_t name = HashValue(szName);

	if (m_pPipeline && m_pPipeline->IsUniformBlockValid(name)) {
		if (m_pUniformVec4s[name] == nullptr) {
			m_pUniformVec4s[name] = new CGfxUniformVec4;

			if (m_pDescriptorSet) {
				m_pDescriptorSet->SetUniformBuffer(name, m_pUniformVec4s[name]->GetUniformBuffer());
			}
		}

		m_pUniformVec4s[name]->SetValue(v0, v1, v2, v3);

		return true;
	}

	return false;
}

bool CVKMaterialPass::SetUniformMat4(const char *szName, const float *value)
{
	uint32_t name = HashValue(szName);

	if (m_pPipeline && m_pPipeline->IsUniformBlockValid(name)) {
		if (m_pUniformMat4s[name] == nullptr) {
			m_pUniformMat4s[name] = new CGfxUniformMat4;

			if (m_pDescriptorSet) {
				m_pDescriptorSet->SetUniformBuffer(name, m_pUniformMat4s[name]->GetUniformBuffer());
			}
		}

		m_pUniformMat4s[name]->SetValue(value);

		return true;
	}

	return false;
}

CGfxPipelineGraphics* CVKMaterialPass::GetPipeline(void) const
{
	return m_pPipeline;
}

CVKDescriptorSet* CVKMaterialPass::GetDescriptorSet(void) const
{
	return m_pDescriptorSet;
}

CGfxTexture2DPtr CVKMaterialPass::GetTexture2D(const char *szName) const
{
	const auto &itTexture = m_ptrTexture2Ds.find(HashValue(szName));

	if (itTexture != m_ptrTexture2Ds.end()) {
		return itTexture->second;
	}
	else {
		return nullptr;
	}
}

CGfxTexture2DArrayPtr CVKMaterialPass::GetTexture2DArray(const char *szName) const
{
	const auto &itTexture = m_ptrTexture2DArrays.find(HashValue(szName));

	if (itTexture != m_ptrTexture2DArrays.end()) {
		return itTexture->second;
	}
	else {
		return nullptr;
	}
}

CGfxTextureCubeMapPtr CVKMaterialPass::GetTextureCubeMap(const char *szName) const
{
	const auto &itTexture = m_ptrTextureCubeMaps.find(HashValue(szName));

	if (itTexture != m_ptrTextureCubeMaps.end()) {
		return itTexture->second;
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
