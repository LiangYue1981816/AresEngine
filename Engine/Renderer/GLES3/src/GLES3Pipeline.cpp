#include "GLES3Renderer.h"


CGLES3Pipeline::CGLES3Pipeline(void)
	: m_program(0)
	, m_pShaders{ nullptr }
{
	m_ptrDescriptorLayouts[DESCRIPTOR_SET_ENGINE] = GLES3Renderer()->NewDescriptorLayout(DESCRIPTOR_SET_ENGINE);
	m_ptrDescriptorLayouts[DESCRIPTOR_SET_CAMERA] = GLES3Renderer()->NewDescriptorLayout(DESCRIPTOR_SET_CAMERA);
	m_ptrDescriptorLayouts[DESCRIPTOR_SET_PASS] = GLES3Renderer()->NewDescriptorLayout(DESCRIPTOR_SET_PASS);
	m_ptrDescriptorLayouts[DESCRIPTOR_SET_DRAW] = GLES3Renderer()->NewDescriptorLayout(DESCRIPTOR_SET_DRAW);
}

CGLES3Pipeline::~CGLES3Pipeline(void)
{
	Destroy();
}

bool CGLES3Pipeline::CreateProgram(void)
{
	m_program = glCreateProgram();

	for (int index = 0; index < compute_shader - vertex_shader + 1; index++) {
		if (m_pShaders[index]) {
			glAttachShader(m_program, (uint32_t)m_pShaders[index]->GetShader());
		}
	}

	GLint success;
	glLinkProgram(m_program);
	glGetProgramiv(m_program, GL_LINK_STATUS, &success);

	if (success == GL_FALSE) {
		GLsizei length = 0;
		char szError[128 * 1024] = { 0 };

		glGetProgramInfoLog(m_program, sizeof(szError), &length, szError);

		LogOutput(nullptr, "Program Link Error:\n");
		LogOutput(nullptr, "%s\n", szError);

		return false;
	}

	return true;
}

bool CGLES3Pipeline::CreateLayouts(void)
{
	for (int index = 0; index < compute_shader - vertex_shader + 1; index++) {
		if (m_pShaders[index]) {
			const eastl::unordered_map<eastl::string, PushConstantRange> &pushConstantRanges = m_pShaders[index]->GetSprivCross().GetPushConstantRanges();
			const eastl::unordered_map<eastl::string, DescriptorSetBinding> &uniformBlockBindings = m_pShaders[index]->GetSprivCross().GetUniformBlockBindings();
			const eastl::unordered_map<eastl::string, DescriptorSetBinding> &sampledImageBindings = m_pShaders[index]->GetSprivCross().GetSampledImageBindings();
			const eastl::unordered_map<eastl::string, DescriptorSetBinding> &inputAttachmentBindings = m_pShaders[index]->GetSprivCross().GetInputAttachmentBindings();

			for (const auto &itPushConstant : pushConstantRanges) {
				SetUniformLocation(itPushConstant.first.c_str());
			}

			for (const auto &itUniformBlock : uniformBlockBindings) {
				if (m_ptrDescriptorLayouts[itUniformBlock.second.set]->SetUniformBlockBinding(HashValue(itUniformBlock.first.c_str()), itUniformBlock.second.binding)) {
					SetUniformBlockBinding(itUniformBlock.first.c_str(), itUniformBlock.second.binding);
				}
			}

			for (const auto &itSampledImage : sampledImageBindings) {
				if (m_ptrDescriptorLayouts[itSampledImage.second.set]->SetSampledImageBinding(HashValue(itSampledImage.first.c_str()), itSampledImage.second.binding)) {
					SetSampledImageLocation(itSampledImage.first.c_str());
				}
			}

			for (const auto &itInputAttachment : inputAttachmentBindings) {
				if (m_ptrDescriptorLayouts[itInputAttachment.second.set]->SetInputAttachmentBinding(HashValue(itInputAttachment.first.c_str()), itInputAttachment.second.binding)) {
					SetSampledImageLocation(itInputAttachment.first.c_str());
				}
			}
		}
	}

	m_ptrDescriptorLayouts[DESCRIPTOR_SET_ENGINE]->Create();
	m_ptrDescriptorLayouts[DESCRIPTOR_SET_CAMERA]->Create();
	m_ptrDescriptorLayouts[DESCRIPTOR_SET_PASS]->Create();
	m_ptrDescriptorLayouts[DESCRIPTOR_SET_DRAW]->Create();

	return true;
}

void CGLES3Pipeline::Destroy(void)
{
	if (m_program) {
		glDeleteProgram(m_program);
	}

	m_program = 0;

	m_pShaders[vertex_shader] = nullptr;
	m_pShaders[fragment_shader] = nullptr;
	m_pShaders[compute_shader] = nullptr;

	m_ptrDescriptorLayouts[DESCRIPTOR_SET_ENGINE]->Destroy();
	m_ptrDescriptorLayouts[DESCRIPTOR_SET_CAMERA]->Destroy();
	m_ptrDescriptorLayouts[DESCRIPTOR_SET_PASS]->Destroy();
	m_ptrDescriptorLayouts[DESCRIPTOR_SET_DRAW]->Destroy();

	m_uniformLocations.clear();
	m_uniformBlockBindings.clear();
	m_sampledImageLocations.clear();
	m_sampledImageTextureUnits.clear();
}

void CGLES3Pipeline::SetUniformLocation(const char *szName)
{
	uint32_t name = HashValue(szName);

	if (m_uniformLocations.find(name) == m_uniformLocations.end()) {
		uint32_t location = glGetUniformLocation(m_program, szName);

		if (location != GL_INVALID_INDEX) {
			m_uniformLocations[name] = location;
		}
	}
}

void CGLES3Pipeline::SetUniformBlockBinding(const char *szName, uint32_t binding)
{
	uint32_t name = HashValue(szName);

	if (m_uniformBlockBindings.find(name) == m_uniformBlockBindings.end()) {
		uint32_t indexBinding = glGetUniformBlockIndex(m_program, szName);

		if (indexBinding != GL_INVALID_INDEX) {
			m_uniformBlockBindings[name] = binding;
			glUniformBlockBinding(m_program, indexBinding, binding);
		}
	}
}

void CGLES3Pipeline::SetSampledImageLocation(const char *szName)
{
	uint32_t name = HashValue(szName);

	if (m_sampledImageLocations.find(name) == m_sampledImageLocations.end()) {
		uint32_t location = glGetUniformLocation(m_program, szName);

		if (location != GL_INVALID_INDEX) {
			m_sampledImageTextureUnits[name] = m_sampledImageLocations.size();
			m_sampledImageLocations[name] = location;
		}
	}
}

void CGLES3Pipeline::BindDescriptorSet(const CGfxDescriptorSetPtr ptrDescriptorSet)
{
	const CGfxDescriptorLayoutPtr ptrDescriptorLayout = ptrDescriptorSet->GetDescriptorLayout();

	if (ptrDescriptorLayout->GetSetIndex() < 0 || ptrDescriptorLayout->GetSetIndex() >= DESCRIPTOR_SET_COUNT) {
		return;
	}

	if (ptrDescriptorLayout->IsCompatible(m_ptrDescriptorLayouts[ptrDescriptorLayout->GetSetIndex()]) == false) {
		return;
	}

	for (const auto &itUniformBlock : m_uniformBlockBindings) {
		if (const CGfxDescriptorSet::DescriptorBufferInfo *pDescriptorBufferInfo = ptrDescriptorSet->GetDescriptorBufferInfo(itUniformBlock.first)) {
			((CGLES3UniformBuffer *)pDescriptorBufferInfo->ptrUniformBuffer.GetPointer())->Bind(itUniformBlock.second, pDescriptorBufferInfo->offset, pDescriptorBufferInfo->range);
		}
	}

	for (const auto &itSampledImage : m_sampledImageLocations) {
		if (const CGfxDescriptorSet::DescriptorImageInfo *pDescriptorImageInfo = ptrDescriptorSet->GetDescriptorImageInfo(itSampledImage.first)) {
			uint32_t indexTextureUnit = m_sampledImageTextureUnits[itSampledImage.first];

			if (pDescriptorImageInfo->ptrTexture2D.IsValid()) {
				GLUniform1i(itSampledImage.second, indexTextureUnit);
				((CGLES3Sampler *)pDescriptorImageInfo->pSampler)->Bind(indexTextureUnit);
				((CGLES3Texture2D *)pDescriptorImageInfo->ptrTexture2D.GetPointer())->Bind(indexTextureUnit);
				continue;
			}

			if (pDescriptorImageInfo->ptrTexture2DArray.IsValid()) {
				GLUniform1i(itSampledImage.second, indexTextureUnit);
				((CGLES3Sampler *)pDescriptorImageInfo->pSampler)->Bind(indexTextureUnit);
				((CGLES3Texture2DArray *)pDescriptorImageInfo->ptrTexture2DArray.GetPointer())->Bind(indexTextureUnit);
				continue;
			}

			if (pDescriptorImageInfo->ptrTextureCubeMap.IsValid()) {
				GLUniform1i(itSampledImage.second, indexTextureUnit);
				((CGLES3Sampler *)pDescriptorImageInfo->pSampler)->Bind(indexTextureUnit);
				((CGLES3TextureCubeMap *)pDescriptorImageInfo->ptrTextureCubeMap.GetPointer())->Bind(indexTextureUnit);
				continue;
			}

			if (pDescriptorImageInfo->ptrRenderTexture.IsValid()) {
				GLUniform1i(itSampledImage.second, indexTextureUnit);
				((CGLES3Sampler *)pDescriptorImageInfo->pSampler)->Bind(indexTextureUnit);
				((CGLES3RenderTexture *)pDescriptorImageInfo->ptrRenderTexture.GetPointer())->Bind(indexTextureUnit);
				continue;
			}
		}
	}
}

void CGLES3Pipeline::Uniform1i(uint32_t name, int v0) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLUniform1i(itLocation->second, v0);
	}
}

void CGLES3Pipeline::Uniform2i(uint32_t name, int v0, int v1) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLUniform2i(itLocation->second, v0, v1);
	}
}

void CGLES3Pipeline::Uniform3i(uint32_t name, int v0, int v1, int v2) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLUniform3i(itLocation->second, v0, v1, v2);
	}
}

void CGLES3Pipeline::Uniform4i(uint32_t name, int v0, int v1, int v2, int v3) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLUniform4i(itLocation->second, v0, v1, v2, v3);
	}
}

void CGLES3Pipeline::Uniform1f(uint32_t name, float v0) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLUniform1f(itLocation->second, v0);
	}
}

void CGLES3Pipeline::Uniform2f(uint32_t name, float v0, float v1) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLUniform2f(itLocation->second, v0, v1);
	}
}

void CGLES3Pipeline::Uniform3f(uint32_t name, float v0, float v1, float v2) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLUniform3f(itLocation->second, v0, v1, v2);
	}
}

void CGLES3Pipeline::Uniform4f(uint32_t name, float v0, float v1, float v2, float v3) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLUniform4f(itLocation->second, v0, v1, v2, v3);
	}
}

void CGLES3Pipeline::Uniform1iv(uint32_t name, int count, const int *value) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLUniform1iv(itLocation->second, count, value);
	}
}

void CGLES3Pipeline::Uniform2iv(uint32_t name, int count, const int *value) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLUniform2iv(itLocation->second, count, value);
	}
}

void CGLES3Pipeline::Uniform3iv(uint32_t name, int count, const int *value) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLUniform3iv(itLocation->second, count, value);
	}
}

void CGLES3Pipeline::Uniform4iv(uint32_t name, int count, const int *value) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLUniform4iv(itLocation->second, count, value);
	}
}

void CGLES3Pipeline::Uniform1fv(uint32_t name, int count, const float *value) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLUniform1fv(itLocation->second, count, value);
	}
}

void CGLES3Pipeline::Uniform2fv(uint32_t name, int count, const float *value) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLUniform2fv(itLocation->second, count, value);
	}
}

void CGLES3Pipeline::Uniform3fv(uint32_t name, int count, const float *value) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLUniform3fv(itLocation->second, count, value);
	}
}

void CGLES3Pipeline::Uniform4fv(uint32_t name, int count, const float *value) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLUniform4fv(itLocation->second, count, value);
	}
}

void CGLES3Pipeline::UniformMatrix2fv(uint32_t name, int count, const float *value) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLUniformMatrix2fv(itLocation->second, count, value);
	}
}

void CGLES3Pipeline::UniformMatrix3fv(uint32_t name, int count, const float *value) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLUniformMatrix3fv(itLocation->second, count, value);
	}
}

void CGLES3Pipeline::UniformMatrix4fv(uint32_t name, int count, const float *value) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLUniformMatrix4fv(itLocation->second, count, value);
	}
}
