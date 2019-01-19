#if GLES_VER == 310

#include "GLES3Renderer.h"


CGLES3Shader::CGLES3Shader(uint32_t name)
	: CGfxShader(name)
	, m_name(name)

	, m_kind(-1)
	, m_program(0)
{

}

CGLES3Shader::~CGLES3Shader(void)
{
	Destroy();
}

uint32_t CGLES3Shader::GetName(void) const
{
	return m_name;
}

uint32_t CGLES3Shader::GetKind(void) const
{
	return m_kind;
}

HANDLE CGLES3Shader::GetShader(void) const
{
	return (HANDLE)m_program;
}

bool CGLES3Shader::Create(const uint32_t *words, size_t numWords, shader_kind kind)
{
	Destroy();
	{
		do {
			spirv_cross::CompilerGLSL::Options options;
			options.version = 310;
			options.es = true;
			options.vertex.fixup_clipspace = false;

			spirv_cross::CompilerGLSL compiler(words, numWords);
			compiler.set_options(options);

			const std::string strSource = compiler.compile();
			const char *szSource = strSource.c_str();

#ifdef DEBUG
//			LogOutput(nullptr, "\n");
//			LogOutput(LOG_TAG_RENDERER, "\n%s\n", szSource);
#endif

			m_kind = kind;
			m_program = glCreateShaderProgramv(glGetShaderType(kind), 1, &szSource);
			if (m_program == 0) break;
			if (CreateLayouts(&compiler) == false) break;

			return true;
		} while (false);
	}
	Destroy();
	return false;
}

void CGLES3Shader::Destroy(void)
{
	if (m_program) {
		glDeleteProgram(m_program);
	}

	m_kind = -1;
	m_program = 0;

	m_uniformLocations.clear();
	m_uniformBlockBindings.clear();
	m_sampledImageLocations.clear();
}

bool CGLES3Shader::CreateLayouts(const spirv_cross::CompilerGLSL *pShaderCompiler)
{
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

	return true;
}

void CGLES3Shader::SetUniformLocation(const char *szName)
{
	uint32_t name = HashValue(szName);

	if (m_uniformLocations.find(name) == m_uniformLocations.end()) {
		uint32_t location = glGetUniformLocation(m_program, szName);

		if (location != GL_INVALID_INDEX) {
			m_uniformLocations[name] = location;
		}
	}
}

void CGLES3Shader::SetUniformBlockBinding(const char *szName, uint32_t binding)
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

void CGLES3Shader::SetSampledImageLocation(const char *szName)
{
	uint32_t name = HashValue(szName);

	if (m_sampledImageLocations.find(name) == m_sampledImageLocations.end()) {
		uint32_t location = glGetUniformLocation(m_program, szName);

		if (location != GL_INVALID_INDEX) {
			m_sampledImageLocations[name] = location;
		}
	}
}

bool CGLES3Shader::BindTexture(uint32_t name, CGLES3Texture *pTexture, CGLES3Sampler *pSampler, uint32_t unit)
{
	const auto &itLocation = m_sampledImageLocations.find(name);

	if (itLocation != m_sampledImageLocations.end()) {
		GLProgramUniform1i(m_program, itLocation->second, unit);
		pSampler->Bind(unit);
		pTexture->Bind(unit);
		return true;
	}

	return false;
}

bool CGLES3Shader::BindUniformBuffer(uint32_t name, CGLES3UniformBuffer *pUniformBuffer, uint32_t size, int offset)
{
	const auto &itBinding = m_uniformBlockBindings.find(name);

	if (itBinding != m_uniformBlockBindings.end()) {
		pUniformBuffer->Bind(itBinding->second, offset, size);
		return true;
	}

	return false;
}

bool CGLES3Shader::Uniform1i(uint32_t name, int v0) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLProgramUniform1i(m_program, itLocation->second, v0);
		return true;
	}

	return false;
}

bool CGLES3Shader::Uniform2i(uint32_t name, int v0, int v1) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLProgramUniform2i(m_program, itLocation->second, v0, v1);
		return true;
	}

	return false;
}

bool CGLES3Shader::Uniform3i(uint32_t name, int v0, int v1, int v2) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLProgramUniform3i(m_program, itLocation->second, v0, v1, v2);
		return true;
	}

	return false;
}

bool CGLES3Shader::Uniform4i(uint32_t name, int v0, int v1, int v2, int v3) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLProgramUniform4i(m_program, itLocation->second, v0, v1, v2, v3);
		return true;
	}

	return false;
}

bool CGLES3Shader::Uniform1f(uint32_t name, float v0) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLProgramUniform1f(m_program, itLocation->second, v0);
		return true;
	}

	return false;
}

bool CGLES3Shader::Uniform2f(uint32_t name, float v0, float v1) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLProgramUniform2f(m_program, itLocation->second, v0, v1);
		return true;
	}

	return false;
}

bool CGLES3Shader::Uniform3f(uint32_t name, float v0, float v1, float v2) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLProgramUniform3f(m_program, itLocation->second, v0, v1, v2);
		return true;
	}

	return false;
}

bool CGLES3Shader::Uniform4f(uint32_t name, float v0, float v1, float v2, float v3) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLProgramUniform4f(m_program, itLocation->second, v0, v1, v2, v3);
		return true;
	}

	return false;
}

bool CGLES3Shader::Uniform1iv(uint32_t name, int count, const int *value) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLProgramUniform1iv(m_program, itLocation->second, count, value);
		return true;
	}

	return false;
}

bool CGLES3Shader::Uniform2iv(uint32_t name, int count, const int *value) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLProgramUniform2iv(m_program, itLocation->second, count, value);
		return true;
	}

	return false;
}

bool CGLES3Shader::Uniform3iv(uint32_t name, int count, const int *value) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLProgramUniform3iv(m_program, itLocation->second, count, value);
		return true;
	}

	return false;
}

bool CGLES3Shader::Uniform4iv(uint32_t name, int count, const int *value) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLProgramUniform4iv(m_program, itLocation->second, count, value);
		return true;
	}

	return false;
}

bool CGLES3Shader::Uniform1fv(uint32_t name, int count, const float *value) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLProgramUniform1fv(m_program, itLocation->second, count, value);
		return true;
	}

	return false;
}

bool CGLES3Shader::Uniform2fv(uint32_t name, int count, const float *value) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLProgramUniform2fv(m_program, itLocation->second, count, value);
		return true;
	}

	return false;
}

bool CGLES3Shader::Uniform3fv(uint32_t name, int count, const float *value) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLProgramUniform3fv(m_program, itLocation->second, count, value);
		return true;
	}

	return false;
}

bool CGLES3Shader::Uniform4fv(uint32_t name, int count, const float *value) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLProgramUniform4fv(m_program, itLocation->second, count, value);
		return true;
	}

	return false;
}

bool CGLES3Shader::UniformMatrix2fv(uint32_t name, int count, const float *value) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLProgramUniformMatrix2fv(m_program, itLocation->second, count, value);
		return true;
	}

	return false;
}

bool CGLES3Shader::UniformMatrix3fv(uint32_t name, int count, const float *value) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLProgramUniformMatrix3fv(m_program, itLocation->second, count, value);
		return true;
	}

	return false;
}

bool CGLES3Shader::UniformMatrix4fv(uint32_t name, int count, const float *value) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLProgramUniformMatrix4fv(m_program, itLocation->second, count, value);
		return true;
	}

	return false;
}

bool CGLES3Shader::IsValid(void) const
{
	return m_program != 0;
}

bool CGLES3Shader::IsTextureValid(uint32_t name) const
{
	return m_sampledImageLocations.find(name) != m_sampledImageLocations.end();
}

bool CGLES3Shader::IsUniformValid(uint32_t name) const
{
	return m_uniformLocations.find(name) != m_uniformLocations.end();
}

bool CGLES3Shader::IsUniformBlockValid(uint32_t name) const
{
	return m_uniformBlockBindings.find(name) != m_uniformBlockBindings.end();
}

#endif
