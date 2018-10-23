#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxShader.h"


CGfxShader::CGfxShader(uint32_t name)
	: m_name(name)

	, m_kind(-1)
	, m_program(0)
{

}

CGfxShader::~CGfxShader(void)
{
	Destroy();
}

uint32_t CGfxShader::GetName(void) const
{
	return m_name;
}

bool CGfxShader::Load(const char *szFileName, shaderc_shader_kind kind)
{
	Destroy();

	std::vector<uint32_t> words;
	if (CGfxShaderCompiler::LoadShaderBinary(Renderer()->GetResourceFullName(szFileName), words) == false) return false;
	if (Create(words.data(), words.size(), kind) == false) return false;

	return true;
}

bool CGfxShader::Create(const uint32_t *words, size_t numWords, shaderc_shader_kind kind)
{
	spirv_cross::CompilerGLSL *pShaderCompiler = NULL;

	try {
		Destroy();

		spirv_cross::CompilerGLSL::Options options;
		options.version = 310;
		options.es = true;
		options.vertex.fixup_clipspace = false;

		pShaderCompiler = new spirv_cross::CompilerGLSL(words, numWords);
		pShaderCompiler->set_options(options);

		const std::string strSource = pShaderCompiler->compile();
		const char *szSource = strSource.c_str();

#ifdef DEBUG
		LogOutput(NULL, "\n");
		LogOutput(LOG_TAG_RENDERER, "\n%s\n", szSource);
#endif

		m_kind = kind;
		m_program = glCreateShaderProgramv(glGetShaderType(kind), 1, &szSource);
		if (m_program == 0) throw 0;
		if (CreateLayouts(pShaderCompiler) == false) throw 1;

		delete pShaderCompiler;

		return true;
	}
	catch (int) {
		Destroy();
		delete pShaderCompiler;

		return false;
	}
}

bool CGfxShader::CreateLayouts(const spirv_cross::CompilerGLSL *pShaderCompiler)
{
	const spirv_cross::ShaderResources shaderResources = pShaderCompiler->get_shader_resources();

	for (const auto &itPushConstant : shaderResources.push_constant_buffers) {
		if (pShaderCompiler->get_type(itPushConstant.base_type_id).basetype == spirv_cross::SPIRType::Struct) {
			for (uint32_t index = 0; index < pShaderCompiler->get_member_count(itPushConstant.base_type_id); index++) {
				const std::string &member = pShaderCompiler->get_member_name(itPushConstant.base_type_id, index);
				const std::string name = itPushConstant.name + "." + member;
				SetUniformLocation(name.c_str());
			}
		}
	}

	for (const auto &itUniform : shaderResources.uniform_buffers) {
		if (pShaderCompiler->get_type(itUniform.base_type_id).basetype == spirv_cross::SPIRType::Struct) {
			const uint32_t binding = pShaderCompiler->get_decoration(itUniform.id, spv::DecorationBinding);
			SetUniformBlockBinding(itUniform.name.c_str(), binding);
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

void CGfxShader::Destroy(void)
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

void CGfxShader::SetUniformLocation(const char *szName)
{
	uint32_t name = HashValue(szName);

	if (m_uniformLocations.find(name) == m_uniformLocations.end()) {
		uint32_t location = glGetUniformLocation(m_program, szName);

		if (location != GL_INVALID_INDEX) {
			m_uniformLocations[name] = location;
		}
	}
}

void CGfxShader::SetUniformBlockBinding(const char *szName, uint32_t binding)
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

void CGfxShader::SetSampledImageLocation(const char *szName)
{
	uint32_t name = HashValue(szName);

	if (m_sampledImageLocations.find(name) == m_sampledImageLocations.end()) {
		uint32_t location = glGetUniformLocation(m_program, szName);

		if (location != GL_INVALID_INDEX) {
			m_sampledImageLocations[name] = location;
		}
	}
}

bool CGfxShader::BindTexture(uint32_t name, uint32_t target, uint32_t texture, uint32_t sampler, uint32_t unit) const
{
	const auto &itLocation = m_sampledImageLocations.find(name);

	if (itLocation != m_sampledImageLocations.end()) {
		GLActiveTexture(unit);
		GLBindSampler(unit, sampler);
		GLBindTexture(unit, target, texture);
		GLProgramUniform1i(m_program, itLocation->second, unit);
		return true;
	}

	return false;
}

bool CGfxShader::BindUniformBuffer(uint32_t name, uint32_t buffer, uint32_t size, int offset) const
{
	const auto &itBinding = m_uniformBlockBindings.find(name);

	if (itBinding != m_uniformBlockBindings.end()) {
		GLBindBufferRange(GL_UNIFORM_BUFFER, itBinding->second, buffer, offset, size);
		return true;
	}

	return false;
}

bool CGfxShader::Uniform1f(uint32_t name, float v0) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLProgramUniform1f(m_program, itLocation->second, v0);
		return true;
	}

	return false;
}

bool CGfxShader::Uniform2f(uint32_t name, float v0, float v1) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLProgramUniform2f(m_program, itLocation->second, v0, v1);
		return true;
	}

	return false;
}

bool CGfxShader::Uniform3f(uint32_t name, float v0, float v1, float v2) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLProgramUniform3f(m_program, itLocation->second, v0, v1, v2);
		return true;
	}

	return false;
}

bool CGfxShader::Uniform4f(uint32_t name, float v0, float v1, float v2, float v3) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLProgramUniform4f(m_program, itLocation->second, v0, v1, v2, v3);
		return true;
	}

	return false;
}

bool CGfxShader::Uniform1fv(uint32_t name, uint32_t count, const float *value) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		glProgramUniform1fv(m_program, itLocation->second, count, value);
		return true;
	}

	return false;
}

bool CGfxShader::Uniform2fv(uint32_t name, uint32_t count, const float *value) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		glProgramUniform2fv(m_program, itLocation->second, count, value);
		return true;
	}

	return false;
}

bool CGfxShader::Uniform3fv(uint32_t name, uint32_t count, const float *value) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		glProgramUniform3fv(m_program, itLocation->second, count, value);
		return true;
	}

	return false;
}

bool CGfxShader::Uniform4fv(uint32_t name, uint32_t count, const float *value) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		glProgramUniform4fv(m_program, itLocation->second, count, value);
		return true;
	}

	return false;
}

bool CGfxShader::UniformMatrix2fv(uint32_t name, uint32_t count, const float *value) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		glProgramUniformMatrix2fv(m_program, itLocation->second, count, GL_FALSE, value);
		return true;
	}

	return false;
}

bool CGfxShader::UniformMatrix3fv(uint32_t name, uint32_t count, const float *value) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		glProgramUniformMatrix3fv(m_program, itLocation->second, count, GL_FALSE, value);
		return true;
	}

	return false;
}

bool CGfxShader::UniformMatrix4fv(uint32_t name, uint32_t count, const float *value) const
{
	const auto &itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		glProgramUniformMatrix4fv(m_program, itLocation->second, count, GL_FALSE, value);
		return true;
	}

	return false;
}

bool CGfxShader::IsValid(void) const
{
	return m_program != 0;
}

bool CGfxShader::IsUniformValid(uint32_t name) const
{
	return m_uniformLocations.find(name) != m_uniformLocations.end();
}

bool CGfxShader::IsUniformBlockValid(uint32_t name) const
{
	return m_uniformBlockBindings.find(name) != m_uniformBlockBindings.end();
}

bool CGfxShader::IsTextureValid(uint32_t name) const
{
	return m_sampledImageLocations.find(name) != m_sampledImageLocations.end();
}

uint32_t CGfxShader::GetKind(void) const
{
	return m_kind;
}

uint32_t CGfxShader::GetProgram(void) const
{
	return m_program;
}
