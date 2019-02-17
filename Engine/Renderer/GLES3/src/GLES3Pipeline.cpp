#include "GLES3Renderer.h"


CGLES3Pipeline::CGLES3Pipeline(void)
	: m_pipeline(0)
	, m_pShaders{ nullptr }
{
	glGenProgramPipelines(1, &m_pipeline);
}

CGLES3Pipeline::~CGLES3Pipeline(void)
{
	glDeleteProgramPipelines(1, &m_pipeline);
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
				SetUniformLocation(m_pShaders[index], itPushConstant.first.c_str());
			}

			for (const auto &itUniform : uniformBlockBindings) {
				SetUniformBlockBinding(m_pShaders[index], itUniform.first.c_str(), itUniform.second.binding);
			}

			for (const auto &itSampledImage : sampledImageBindings) {
				SetSampledImageLocation(m_pShaders[index], itSampledImage.first.c_str());
			}

			for (const auto &itInputAttachment : inputAttachmentBindings) {
				SetSampledImageLocation(m_pShaders[index], itInputAttachment.first.c_str());
			}
		}
	}

	return true;
}

void CGLES3Pipeline::SetUniformLocation(const CGLES3Shader *pShader, const char *szName)
{
	uint32_t name = HashValue(szName);

	if (m_uniformLocations.find(name) == m_uniformLocations.end()) {
		uint32_t location = glGetUniformLocation((uint32_t)pShader->GetShader(), szName);

		if (location != GL_INVALID_INDEX) {
			m_uniformLocations[name] = location;
		}
	}
}

void CGLES3Pipeline::SetUniformBlockBinding(const CGLES3Shader *pShader, const char *szName, uint32_t binding)
{
	uint32_t name = HashValue(szName);

	if (m_uniformBlockBindings.find(name) == m_uniformBlockBindings.end()) {
		uint32_t indexBinding = glGetUniformBlockIndex((uint32_t)pShader->GetShader(), szName);

		if (indexBinding != GL_INVALID_INDEX) {
			m_uniformBlockBindings[name] = binding;
			glUniformBlockBinding((uint32_t)pShader->GetShader(), indexBinding, binding);
		}
	}
}

void CGLES3Pipeline::SetSampledImageLocation(const CGLES3Shader *pShader, const char *szName)
{
	uint32_t name = HashValue(szName);

	if (m_sampledImageLocations.find(name) == m_sampledImageLocations.end()) {
		uint32_t location = glGetUniformLocation((uint32_t)pShader->GetShader(), szName);

		if (location != GL_INVALID_INDEX) {
			m_sampledImageLocations[name] = location;
		}
	}
}

void CGLES3Pipeline::BindTexture(uint32_t name, CGLES3Texture *pTexture, CGLES3Sampler *pSampler, uint32_t unit)
{
	/*
	for (int index = 0; index < compute_shader - vertex_shader + 1; index++) {
		if (m_pShaders[index] && m_pShaders[index]->IsTextureValid(name)) {
			GLProgramUniform1i((uint32_t)m_pShaders[index]->GetShader(), m_pShaders[index]->GetTextureLocation(name), unit);
			pSampler->Bind(unit);
			pTexture->Bind(unit);
		}
	}
	*/
}

void CGLES3Pipeline::BindUniformBuffer(uint32_t name, CGLES3UniformBuffer *pUniformBuffer, uint32_t size, int offset)
{
	/*
	for (int index = 0; index < compute_shader - vertex_shader + 1; index++) {
		if (m_pShaders[index] && m_pShaders[index]->IsUniformBlockValid(name)) {
			pUniformBuffer->Bind(m_pShaders[index]->GetUniformBlockBinding(name), offset, size);
		}
	}
	*/
}

void CGLES3Pipeline::Uniform1i(uint32_t name, int v0) const
{
	/*
	for (int index = 0; index < compute_shader - vertex_shader + 1; index++) {
		if (m_pShaders[index] && m_pShaders[index]->IsUniformValid(name)) {
			GLProgramUniform1i((uint32_t)m_pShaders[index]->GetShader(), m_pShaders[index]->GetUniformLocation(name), v0);
		}
	}
	*/
}

void CGLES3Pipeline::Uniform2i(uint32_t name, int v0, int v1) const
{
	/*
	for (int index = 0; index < compute_shader - vertex_shader + 1; index++) {
		if (m_pShaders[index] && m_pShaders[index]->IsUniformValid(name)) {
			GLProgramUniform2i((uint32_t)m_pShaders[index]->GetShader(), m_pShaders[index]->GetUniformLocation(name), v0, v1);
		}
	}
	*/
}

void CGLES3Pipeline::Uniform3i(uint32_t name, int v0, int v1, int v2) const
{
	/*
	for (int index = 0; index < compute_shader - vertex_shader + 1; index++) {
		if (m_pShaders[index] && m_pShaders[index]->IsUniformValid(name)) {
			GLProgramUniform3i((uint32_t)m_pShaders[index]->GetShader(), m_pShaders[index]->GetUniformLocation(name), v0, v1, v2);
		}
	}
	*/
}

void CGLES3Pipeline::Uniform4i(uint32_t name, int v0, int v1, int v2, int v3) const
{
	/*
	for (int index = 0; index < compute_shader - vertex_shader + 1; index++) {
		if (m_pShaders[index] && m_pShaders[index]->IsUniformValid(name)) {
			GLProgramUniform4i((uint32_t)m_pShaders[index]->GetShader(), m_pShaders[index]->GetUniformLocation(name), v0, v1, v2, v3);
		}
	}
	*/
}

void CGLES3Pipeline::Uniform1f(uint32_t name, float v0) const
{
	/*
	for (int index = 0; index < compute_shader - vertex_shader + 1; index++) {
		if (m_pShaders[index] && m_pShaders[index]->IsUniformValid(name)) {
			GLProgramUniform1f((uint32_t)m_pShaders[index]->GetShader(), m_pShaders[index]->GetUniformLocation(name), v0);
		}
	}
	*/
}

void CGLES3Pipeline::Uniform2f(uint32_t name, float v0, float v1) const
{
	/*
	for (int index = 0; index < compute_shader - vertex_shader + 1; index++) {
		if (m_pShaders[index] && m_pShaders[index]->IsUniformValid(name)) {
			GLProgramUniform2f((uint32_t)m_pShaders[index]->GetShader(), m_pShaders[index]->GetUniformLocation(name), v0, v1);
		}
	}
	*/
}

void CGLES3Pipeline::Uniform3f(uint32_t name, float v0, float v1, float v2) const
{
	/*
	for (int index = 0; index < compute_shader - vertex_shader + 1; index++) {
		if (m_pShaders[index] && m_pShaders[index]->IsUniformValid(name)) {
			GLProgramUniform3f((uint32_t)m_pShaders[index]->GetShader(), m_pShaders[index]->GetUniformLocation(name), v0, v1, v2);
		}
	}
	*/
}

void CGLES3Pipeline::Uniform4f(uint32_t name, float v0, float v1, float v2, float v3) const
{
	/*
	for (int index = 0; index < compute_shader - vertex_shader + 1; index++) {
		if (m_pShaders[index] && m_pShaders[index]->IsUniformValid(name)) {
			GLProgramUniform4f((uint32_t)m_pShaders[index]->GetShader(), m_pShaders[index]->GetUniformLocation(name), v0, v1, v2, v3);
		}
	}
	*/
}

void CGLES3Pipeline::Uniform1iv(uint32_t name, int count, const int *value) const
{
	/*
	for (int index = 0; index < compute_shader - vertex_shader + 1; index++) {
		if (m_pShaders[index] && m_pShaders[index]->IsUniformValid(name)) {
			GLProgramUniform1iv((uint32_t)m_pShaders[index]->GetShader(), m_pShaders[index]->GetUniformLocation(name), count, value);
		}
	}
	*/
}

void CGLES3Pipeline::Uniform2iv(uint32_t name, int count, const int *value) const
{
	/*
	for (int index = 0; index < compute_shader - vertex_shader + 1; index++) {
		if (m_pShaders[index] && m_pShaders[index]->IsUniformValid(name)) {
			GLProgramUniform2iv((uint32_t)m_pShaders[index]->GetShader(), m_pShaders[index]->GetUniformLocation(name), count, value);
		}
	}
	*/
}

void CGLES3Pipeline::Uniform3iv(uint32_t name, int count, const int *value) const
{
	/*
	for (int index = 0; index < compute_shader - vertex_shader + 1; index++) {
		if (m_pShaders[index] && m_pShaders[index]->IsUniformValid(name)) {
			GLProgramUniform3iv((uint32_t)m_pShaders[index]->GetShader(), m_pShaders[index]->GetUniformLocation(name), count, value);
		}
	}
	*/
}

void CGLES3Pipeline::Uniform4iv(uint32_t name, int count, const int *value) const
{
	/*
	for (int index = 0; index < compute_shader - vertex_shader + 1; index++) {
		if (m_pShaders[index] && m_pShaders[index]->IsUniformValid(name)) {
			GLProgramUniform4iv((uint32_t)m_pShaders[index]->GetShader(), m_pShaders[index]->GetUniformLocation(name), count, value);
		}
	}
	*/
}

void CGLES3Pipeline::Uniform1fv(uint32_t name, int count, const float *value) const
{
	/*
	for (int index = 0; index < compute_shader - vertex_shader + 1; index++) {
		if (m_pShaders[index] && m_pShaders[index]->IsUniformValid(name)) {
			GLProgramUniform1fv((uint32_t)m_pShaders[index]->GetShader(), m_pShaders[index]->GetUniformLocation(name), count, value);
		}
	}
	*/
}

void CGLES3Pipeline::Uniform2fv(uint32_t name, int count, const float *value) const
{
	/*
	for (int index = 0; index < compute_shader - vertex_shader + 1; index++) {
		if (m_pShaders[index] && m_pShaders[index]->IsUniformValid(name)) {
			GLProgramUniform2fv((uint32_t)m_pShaders[index]->GetShader(), m_pShaders[index]->GetUniformLocation(name), count, value);
		}
	}
	*/
}

void CGLES3Pipeline::Uniform3fv(uint32_t name, int count, const float *value) const
{
	/*
	for (int index = 0; index < compute_shader - vertex_shader + 1; index++) {
		if (m_pShaders[index] && m_pShaders[index]->IsUniformValid(name)) {
			GLProgramUniform3fv((uint32_t)m_pShaders[index]->GetShader(), m_pShaders[index]->GetUniformLocation(name), count, value);
		}
	}
	*/
}

void CGLES3Pipeline::Uniform4fv(uint32_t name, int count, const float *value) const
{
	/*
	for (int index = 0; index < compute_shader - vertex_shader + 1; index++) {
		if (m_pShaders[index] && m_pShaders[index]->IsUniformValid(name)) {
			GLProgramUniform4fv((uint32_t)m_pShaders[index]->GetShader(), m_pShaders[index]->GetUniformLocation(name), count, value);
		}
	}
	*/
}

void CGLES3Pipeline::UniformMatrix2fv(uint32_t name, int count, const float *value) const
{
	/*
	for (int index = 0; index < compute_shader - vertex_shader + 1; index++) {
		if (m_pShaders[index] && m_pShaders[index]->IsUniformValid(name)) {
			GLProgramUniformMatrix2fv((uint32_t)m_pShaders[index]->GetShader(), m_pShaders[index]->GetUniformLocation(name), count, value);
		}
	}
	*/
}

void CGLES3Pipeline::UniformMatrix3fv(uint32_t name, int count, const float *value) const
{
	/*
	for (int index = 0; index < compute_shader - vertex_shader + 1; index++) {
		if (m_pShaders[index] && m_pShaders[index]->IsUniformValid(name)) {
			GLProgramUniformMatrix3fv((uint32_t)m_pShaders[index]->GetShader(), m_pShaders[index]->GetUniformLocation(name), count, value);
		}
	}
	*/
}

void CGLES3Pipeline::UniformMatrix4fv(uint32_t name, int count, const float *value) const
{
	/*
	for (int index = 0; index < compute_shader - vertex_shader + 1; index++) {
		if (m_pShaders[index] && m_pShaders[index]->IsUniformValid(name)) {
			GLProgramUniformMatrix4fv((uint32_t)m_pShaders[index]->GetShader(), m_pShaders[index]->GetUniformLocation(name), count, value);
		}
	}
	*/
}

bool CGLES3Pipeline::IsTextureValid(uint32_t name) const
{
	return m_sampledImageLocations.find(name) != m_sampledImageLocations.end();
}

bool CGLES3Pipeline::IsUniformValid(uint32_t name) const
{
	return m_uniformLocations.find(name) != m_uniformLocations.end();
}

bool CGLES3Pipeline::IsUniformBlockValid(uint32_t name) const
{
	return m_uniformBlockBindings.find(name) != m_uniformBlockBindings.end();
}
