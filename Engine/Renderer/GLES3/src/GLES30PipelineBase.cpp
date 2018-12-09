#if GLES_VER == 300

#include "GLES3Renderer.h"


CGLES3PipelineBase::CGLES3PipelineBase(void)
	: m_program(0)
	, m_pShaders{ nullptr }
{

}

CGLES3PipelineBase::~CGLES3PipelineBase(void)
{

}

bool CGLES3PipelineBase::CreateLayouts(void)
{
	for (int index = 0; index < compute_shader - vertex_shader + 1; index++) {
		if (m_pShaders[index]) {
			if (const spirv_cross::CompilerGLSL *pShaderCompiler = m_pShaders[index]->GetShaderCompiler()) {
				const spirv_cross::ShaderResources shaderResources = pShaderCompiler->get_shader_resources();

				for (const auto &itPushConstant : shaderResources.push_constant_buffers) {
					if (pShaderCompiler->get_type(itPushConstant.base_type_id).basetype == spirv_cross::SPIRType::Struct) {
						for (uint32_t index = 0; index < pShaderCompiler->get_member_count(itPushConstant.base_type_id); index++) {
							const std::string member = pShaderCompiler->get_member_name(itPushConstant.base_type_id, index);
							const std::string name = itPushConstant.name + "." + member;
							SetUniformLocation(name.c_str());
						}
					}
				}

				for (const auto &itUniform : shaderResources.uniform_buffers) {
					if (pShaderCompiler->get_type(itUniform.base_type_id).basetype == spirv_cross::SPIRType::Struct) {
						SetUniformBlockBinding(itUniform.name.c_str(), pShaderCompiler->get_decoration(itUniform.id, spv::DecorationBinding));
					}
				}

				for (const auto &itSampledImage : shaderResources.sampled_images) {
					if (pShaderCompiler->get_type(itSampledImage.base_type_id).basetype == spirv_cross::SPIRType::SampledImage) {
						SetSampledImageLocation(itSampledImage.name.c_str());
					}
				}

				for (const auto &itSubpassInput : shaderResources.subpass_inputs) {
					if (pShaderCompiler->get_type(itSubpassInput.base_type_id).basetype == spirv_cross::SPIRType::Image) {
						SetSampledImageLocation(itSubpassInput.name.c_str());
					}
				}
			}
		}
	}

	return true;
}

void CGLES3PipelineBase::SetUniformLocation(const char *szName)
{
	uint32_t name = HashValue(szName);

	if (m_uniformLocations.find(name) == m_uniformLocations.end()) {
		uint32_t location = glGetUniformLocation(m_program, szName);

		if (location != GL_INVALID_INDEX) {
			m_uniformLocations[name] = location;
		}
	}
}

void CGLES3PipelineBase::SetUniformBlockBinding(const char *szName, uint32_t binding)
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

void CGLES3PipelineBase::SetSampledImageLocation(const char *szName)
{
	uint32_t name = HashValue(szName);

	if (m_sampledImageLocations.find(name) == m_sampledImageLocations.end()) {
		uint32_t location = glGetUniformLocation(m_program, szName);

		if (location != GL_INVALID_INDEX) {
			m_sampledImageLocations[name] = location;
		}
	}
}

bool CGLES3PipelineBase::BindTexture(uint32_t name, CGLES3TextureBase *pTexture, CGLES3Sampler *pSampler, uint32_t unit)
{
	const auto &itLocation = m_sampledImageLocations.find(name);

	if (itLocation != m_sampledImageLocations.end()) {
		GLActiveTexture(unit);
		GLUniform1i(itLocation->second, unit);
		pSampler->Bind(unit);
		pTexture->Bind(unit);
		return true;
	}

	return false;
}

bool CGLES3PipelineBase::BindUniformBuffer(uint32_t name, CGLES3UniformBuffer *pUniformBuffer, uint32_t size, int offset)
{
	const auto &itBinding = m_uniformBlockBindings.find(name);

	if (itBinding != m_uniformBlockBindings.end()) {
		pUniformBuffer->Bind(itBinding->second, offset, size);
		return true;
	}

	return false;
}

bool CGLES3PipelineBase::Uniform1i(uint32_t name, int v0) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLUniform1i(itLocation->second, v0);
		return true;
	}

	return false;
}

bool CGLES3PipelineBase::Uniform2i(uint32_t name, int v0, int v1) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLUniform2i(itLocation->second, v0, v1);
		return true;
	}

	return false;
}

bool CGLES3PipelineBase::Uniform3i(uint32_t name, int v0, int v1, int v2) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLUniform3i(itLocation->second, v0, v1, v2);
		return true;
	}

	return false;
}

bool CGLES3PipelineBase::Uniform4i(uint32_t name, int v0, int v1, int v2, int v3) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLUniform4i(itLocation->second, v0, v1, v2, v3);
		return true;
	}

	return false;
}

bool CGLES3PipelineBase::Uniform1f(uint32_t name, float v0) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLUniform1f(itLocation->second, v0);
		return true;
	}

	return false;
}

bool CGLES3PipelineBase::Uniform2f(uint32_t name, float v0, float v1) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLUniform2f(itLocation->second, v0, v1);
		return true;
	}

	return false;
}

bool CGLES3PipelineBase::Uniform3f(uint32_t name, float v0, float v1, float v2) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLUniform3f(itLocation->second, v0, v1, v2);
		return true;
	}

	return false;
}

bool CGLES3PipelineBase::Uniform4f(uint32_t name, float v0, float v1, float v2, float v3) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLUniform4f(itLocation->second, v0, v1, v2, v3);
		return true;
	}

	return false;
}

bool CGLES3PipelineBase::Uniform1iv(uint32_t name, int count, const int *value) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLUniform1iv(itLocation->second, count, value);
		return true;
	}

	return false;
}

bool CGLES3PipelineBase::Uniform2iv(uint32_t name, int count, const int *value) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLUniform2iv(itLocation->second, count, value);
		return true;
	}

	return false;
}

bool CGLES3PipelineBase::Uniform3iv(uint32_t name, int count, const int *value) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLUniform3iv(itLocation->second, count, value);
		return true;
	}

	return false;
}

bool CGLES3PipelineBase::Uniform4iv(uint32_t name, int count, const int *value) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLUniform4iv(itLocation->second, count, value);
		return true;
	}

	return false;
}

bool CGLES3PipelineBase::Uniform1fv(uint32_t name, int count, const float *value) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLUniform1fv(itLocation->second, count, value);
		return true;
	}

	return false;
}

bool CGLES3PipelineBase::Uniform2fv(uint32_t name, int count, const float *value) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLUniform2fv(itLocation->second, count, value);
		return true;
	}

	return false;
}

bool CGLES3PipelineBase::Uniform3fv(uint32_t name, int count, const float *value) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLUniform3fv(itLocation->second, count, value);
		return true;
	}

	return false;
}

bool CGLES3PipelineBase::Uniform4fv(uint32_t name, int count, const float *value) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLUniform4fv(itLocation->second, count, value);
		return true;
	}

	return false;
}

bool CGLES3PipelineBase::UniformMatrix2fv(uint32_t name, int count, const float *value) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLUniformMatrix2fv(itLocation->second, count, value);
		return true;
	}

	return false;
}

bool CGLES3PipelineBase::UniformMatrix3fv(uint32_t name, int count, const float *value) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLUniformMatrix3fv(itLocation->second, count, value);
		return true;
	}

	return false;
}

bool CGLES3PipelineBase::UniformMatrix4fv(uint32_t name, int count, const float *value) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLUniformMatrix4fv(itLocation->second, count, value);
		return true;
	}

	return false;
}

bool CGLES3PipelineBase::IsTextureValid(uint32_t name) const
{
	return m_sampledImageLocations.find(name) != m_sampledImageLocations.end();
}

bool CGLES3PipelineBase::IsUniformValid(uint32_t name) const
{
	return m_uniformLocations.find(name) != m_uniformLocations.end();
}

bool CGLES3PipelineBase::IsUniformBlockValid(uint32_t name) const
{
	return m_uniformBlockBindings.find(name) != m_uniformBlockBindings.end();
}

#endif
