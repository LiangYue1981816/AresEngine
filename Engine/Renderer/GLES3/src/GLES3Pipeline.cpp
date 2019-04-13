#include "GLES3Renderer.h"


CGLES3Pipeline::CGLES3Pipeline(void)
	: m_program(0)
	, m_pShaders{ nullptr }
{
	m_ptrDescriptorLayouts[DESCRIPTOR_SET_ENGINE] = GLES3Renderer()->NewDescriptorLayout(DESCRIPTOR_SET_ENGINE);
	m_ptrDescriptorLayouts[DESCRIPTOR_SET_CAMERA] = GLES3Renderer()->NewDescriptorLayout(DESCRIPTOR_SET_CAMERA);
	m_ptrDescriptorLayouts[DESCRIPTOR_SET_PASS] = GLES3Renderer()->NewDescriptorLayout(DESCRIPTOR_SET_PASS);
	m_ptrDescriptorLayouts[DESCRIPTOR_SET_INPUTATTACHMENT] = GLES3Renderer()->NewDescriptorLayout(DESCRIPTOR_SET_INPUTATTACHMENT);
}

CGLES3Pipeline::~CGLES3Pipeline(void)
{
	Destroy();
}

const CGfxDescriptorLayoutPtr CGLES3Pipeline::GetDescriptorLayout(uint32_t indexDescriptorSet) const
{
	return m_ptrDescriptorLayouts[indexDescriptorSet];
}

bool CGLES3Pipeline::CreateProgram(const CGLES3Shader *pVertexShader, const CGLES3Shader *pFragmentShader, const CGLES3Shader *pComputeShader)
{
	m_pShaders[vertex_shader] = (CGLES3Shader *)pVertexShader;
	m_pShaders[fragment_shader] = (CGLES3Shader *)pFragmentShader;
	m_pShaders[compute_shader] = (CGLES3Shader *)pComputeShader;

	m_program = glCreateProgram();
	{
		for (int index = 0; index < compute_shader - vertex_shader + 1; index++) {
			if (m_pShaders[index]) {
				glAttachShader(m_program, (uint32_t)m_pShaders[index]->GetShader());
			}
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
			const eastl::unordered_map<eastl::string, InputAttachmentBinding> &inputAttachmentBindings = m_pShaders[index]->GetSprivCross().GetInputAttachmentBindings();

			for (const auto &itPushConstant : pushConstantRanges) {
				SetUniformLocation(HashValue(itPushConstant.first.c_str()));
			}

			for (const auto &itUniformBlock : uniformBlockBindings) {
				if (m_ptrDescriptorLayouts[itUniformBlock.second.set]->SetUniformBlockBinding(HashValue(itUniformBlock.first.c_str()), itUniformBlock.second.binding)) {
					SetUniformBlockBinding(HashValue(itUniformBlock.first.c_str()), itUniformBlock.second.binding);
				}
			}

			for (const auto &itSampledImage : sampledImageBindings) {
				if (m_ptrDescriptorLayouts[itSampledImage.second.set]->SetSampledImageBinding(HashValue(itSampledImage.first.c_str()), itSampledImage.second.binding)) {
					SetSampledImageLocation(HashValue(itSampledImage.first.c_str()));
				}
			}

			for (const auto &itInputAttachment : inputAttachmentBindings) {
				if (m_ptrDescriptorLayouts[itInputAttachment.second.set]->SetInputAttachmentBinding(HashValue(itInputAttachment.first.c_str()), itInputAttachment.second.binding)) {
					SetInputAttachmentLocation(HashValue(itInputAttachment.first.c_str()), itInputAttachment.second.inputAttachmentIndex);
				}
			}
		}
	}

	m_ptrDescriptorLayouts[DESCRIPTOR_SET_ENGINE]->Create();
	m_ptrDescriptorLayouts[DESCRIPTOR_SET_CAMERA]->Create();
	m_ptrDescriptorLayouts[DESCRIPTOR_SET_PASS]->Create();
	m_ptrDescriptorLayouts[DESCRIPTOR_SET_INPUTATTACHMENT]->Create();

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

	m_uniformLocations.clear();
	m_uniformBlockBindings.clear();
	m_sampledImageLocations.clear();
	m_sampledImageTextureUnits.clear();

	m_ptrDescriptorLayouts[DESCRIPTOR_SET_ENGINE]->Destroy();
	m_ptrDescriptorLayouts[DESCRIPTOR_SET_CAMERA]->Destroy();
	m_ptrDescriptorLayouts[DESCRIPTOR_SET_PASS]->Destroy();
	m_ptrDescriptorLayouts[DESCRIPTOR_SET_INPUTATTACHMENT]->Destroy();
}

void CGLES3Pipeline::SetUniformBlockBinding(uint32_t name, uint32_t binding)
{
	if (m_uniformBlockBindings.find(name) == m_uniformBlockBindings.end()) {
		uint32_t indexBinding = glGetUniformBlockIndex(m_program, szName);

		if (indexBinding != GL_INVALID_INDEX) {
			m_uniformBlockBindings[name] = binding;
			glUniformBlockBinding(m_program, indexBinding, binding);
		}
	}
}

void CGLES3Pipeline::SetUniformLocation(uint32_t name)
{
	if (m_uniformLocations.find(name) == m_uniformLocations.end()) {
		uint32_t location = glGetUniformLocation(m_program, szName);

		if (location != GL_INVALID_INDEX) {
			m_uniformLocations[name] = location;
		}
	}
}

void CGLES3Pipeline::SetSampledImageLocation(uint32_t name)
{
	if (m_sampledImageLocations.find(name) == m_sampledImageLocations.end()) {
		uint32_t location = glGetUniformLocation(m_program, szName);

		if (location != GL_INVALID_INDEX) {
			m_sampledImageTextureUnits[name] = m_sampledImageLocations.size();
			m_sampledImageLocations[name] = location;
		}
	}
}

void CGLES3Pipeline::SetInputAttachmentLocation(uint32_t name, uint32_t inputAttachmentIndex)
{
	if (m_sampledImageLocations.find(name) == m_sampledImageLocations.end()) {
		uint32_t location = glGetUniformLocation(m_program, szName);

		if (location != GL_INVALID_INDEX) {
			m_sampledImageTextureUnits[name] = m_sampledImageLocations.size();
			m_sampledImageLocations[name] = location;
			m_inputAttachmentNames[inputAttachmentIndex] = name;
		}
	}
}

uint32_t CGLES3Pipeline::GetUniformBlockBinding(uint32_t name) const
{
	const auto &itUniformBlockBinding = m_uniformBlockBindings.find(name);

	if (itUniformBlockBinding != m_uniformBlockBindings.end()) {
		return itUniformBlockBinding->second;
	}
	else {
		return -1;
	}
}

uint32_t CGLES3Pipeline::GetUniformLocation(uint32_t name) const
{
	const auto &itUniformLocation = m_uniformLocations.find(name);

	if (itUniformLocation != m_uniformLocations.end()) {
		return itUniformLocation->second;
	}
	else {
		return -1;
	}
}

uint32_t CGLES3Pipeline::GetSampledImageLocation(uint32_t name) const
{
	const auto &itSampledImageLocation = m_sampledImageLocations.find(name);

	if (itSampledImageLocation != m_sampledImageLocations.end()) {
		return itSampledImageLocation->second;
	}
	else {
		return -1;
	}
}

uint32_t CGLES3Pipeline::GetInputAttachmentName(uint32_t inputAttachmentIndex) const
{
	const auto &itInputAttachmentName = m_inputAttachmentNames.find(inputAttachmentIndex);

	if (itInputAttachmentName != m_inputAttachmentNames.end()) {
		return itInputAttachmentName->second;
	}
	else {
		return INVALID_HASHNAME;
	}
}

void CGLES3Pipeline::Bind(const PipelineState *pState) const
{
	if (pState) {
		GLBindState(pState);
	}

	GLUseProgram(m_program);
}

bool CGLES3Pipeline::BindDescriptorSet(const CGfxDescriptorSetPtr ptrDescriptorSet) const
{
	const CGfxDescriptorLayoutPtr ptrDescriptorLayout = ptrDescriptorSet->GetDescriptorLayout();

	if (ptrDescriptorLayout->GetSetIndex() < 0 || ptrDescriptorLayout->GetSetIndex() >= DESCRIPTOR_SET_COUNT) {
		return false;
	}

	if (ptrDescriptorLayout->IsCompatible(m_ptrDescriptorLayouts[ptrDescriptorLayout->GetSetIndex()]) == false) {
		return false;
	}

	for (const auto &itUniformBlock : m_uniformBlockBindings) {
		if (const DescriptorBufferInfo *pDescriptorBufferInfo = ptrDescriptorSet->GetDescriptorBufferInfo(itUniformBlock.first)) {
			((CGLES3UniformBuffer *)pDescriptorBufferInfo->ptrUniformBuffer.GetPointer())->Bind(itUniformBlock.second, pDescriptorBufferInfo->offset, pDescriptorBufferInfo->range);
		}
	}

	for (const auto &itSampledImage : m_sampledImageLocations) {
		if (const DescriptorImageInfo *pDescriptorImageInfo = ptrDescriptorSet->GetDescriptorImageInfo(itSampledImage.first)) {
			const auto &itTextureUnit = m_sampledImageTextureUnits.find(itSampledImage.first);

			if (pDescriptorImageInfo->ptrTexture2D.IsValid()) {
				GLUniform1i(itSampledImage.second, itTextureUnit->second);
				((CGLES3Sampler *)pDescriptorImageInfo->pSampler)->Bind(itTextureUnit->second);
				((CGLES3Texture2D *)pDescriptorImageInfo->ptrTexture2D.GetPointer())->Bind(itTextureUnit->second);
				continue;
			}

			if (pDescriptorImageInfo->ptrTexture2DArray.IsValid()) {
				GLUniform1i(itSampledImage.second, itTextureUnit->second);
				((CGLES3Sampler *)pDescriptorImageInfo->pSampler)->Bind(itTextureUnit->second);
				((CGLES3Texture2DArray *)pDescriptorImageInfo->ptrTexture2DArray.GetPointer())->Bind(itTextureUnit->second);
				continue;
			}

			if (pDescriptorImageInfo->ptrTextureCubeMap.IsValid()) {
				GLUniform1i(itSampledImage.second, itTextureUnit->second);
				((CGLES3Sampler *)pDescriptorImageInfo->pSampler)->Bind(itTextureUnit->second);
				((CGLES3TextureCubeMap *)pDescriptorImageInfo->ptrTextureCubeMap.GetPointer())->Bind(itTextureUnit->second);
				continue;
			}

			if (pDescriptorImageInfo->ptrTextureInputAttachment.IsValid()) {
				GLUniform1i(itSampledImage.second, itTextureUnit->second);
				((CGLES3Sampler *)pDescriptorImageInfo->pSampler)->Bind(itTextureUnit->second);
				((CGLES3RenderTexture *)pDescriptorImageInfo->ptrTextureInputAttachment.GetPointer())->Bind(itTextureUnit->second);
				continue;
			}
		}
	}

	return true;
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
