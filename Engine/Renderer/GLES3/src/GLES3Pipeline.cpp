#include "GLES3Renderer.h"


CGLES3Pipeline::CGLES3Pipeline(void)
	: m_program(0)
	, m_pShaders{ nullptr }
{

}

CGLES3Pipeline::~CGLES3Pipeline(void)
{

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

			for (const auto &itUniform : uniformBlockBindings) {
				SetUniformBlockBinding(itUniform.first.c_str(), itUniform.second.binding);
			}

			for (const auto &itSampledImage : sampledImageBindings) {
				SetSampledImageLocation(itSampledImage.first.c_str());
			}

			for (const auto &itInputAttachment : inputAttachmentBindings) {
				SetSampledImageLocation(itInputAttachment.first.c_str());
			}
		}
	}

	return true;
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

void CGLES3Pipeline::BindTexture(uint32_t name, CGLES3Texture *pTexture, CGLES3Sampler *pSampler) const
{
	const auto &itLocation = m_sampledImageLocations.find(name);
	const auto &itTextureUnit = m_sampledImageTextureUnits.find(name);

	if (itLocation != m_sampledImageLocations.end() && itTextureUnit != m_sampledImageTextureUnits.end()) {
		GLUniform1i(itLocation->second, itTextureUnit->second);
		pSampler->Bind(itTextureUnit->second);
		pTexture->Bind(itTextureUnit->second);
	}
}

void CGLES3Pipeline::BindRenderTexture(uint32_t name, CGLES3RenderTexture *pRenderTexture, CGLES3Sampler *pSampler) const
{
	const auto &itLocation = m_sampledImageLocations.find(name);
	const auto &itTextureUnit = m_sampledImageTextureUnits.find(name);

	if (itLocation != m_sampledImageLocations.end() && itTextureUnit != m_sampledImageTextureUnits.end()) {
		GLUniform1i(itLocation->second, itTextureUnit->second);
		pSampler->Bind(itTextureUnit->second);
		pRenderTexture->Bind(itTextureUnit->second);
	}
}

void CGLES3Pipeline::BindUniformBuffer(uint32_t name, CGLES3UniformBuffer *pUniformBuffer, uint32_t size, int offset) const
{
	const auto &itBinding = m_uniformBlockBindings.find(name);

	if (itBinding != m_uniformBlockBindings.end()) {
		pUniformBuffer->Bind(itBinding->second, offset, size);
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
