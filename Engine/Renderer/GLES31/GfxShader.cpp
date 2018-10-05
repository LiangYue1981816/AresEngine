#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxShader.h"


CGfxShader::CGfxShader(uint32_t name)
	: m_name(name)

	, m_kind(-1)
	, m_program(0)

	, m_pShaderCompiler(NULL)
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
	try {
		Destroy();

		spirv_cross::CompilerGLSL::Options options;
		options.version = 310;
		options.es = true;
		options.vertex.fixup_clipspace = false;

		m_pShaderCompiler = new spirv_cross::CompilerGLSL(words, numWords);
		m_pShaderCompiler->set_options(options);

		const std::string strSource = m_pShaderCompiler->compile();
		const char *szSource = strSource.c_str();

		m_kind = kind;
		m_program = glCreateShaderProgramv(glGetShaderKind(kind), 1, &szSource);
		if (m_program == 0) throw 0;
		if (CreateLayouts() == false) throw 1;

		return true;
	}
	catch (int) {
		Destroy();
		return false;
	}
}

bool CGfxShader::CreateLayouts(void)
{
	const spirv_cross::ShaderResources shaderResources = m_pShaderCompiler->get_shader_resources();

	for (const auto &itUniform : shaderResources.uniform_buffers) {
		if (m_pShaderCompiler->get_type(itUniform.type_id).basetype == spirv_cross::SPIRType::Struct) {
			const uint32_t binding = m_pShaderCompiler->get_decoration(itUniform.id, spv::DecorationBinding);
			SetUniformBlockBinding(itUniform.name.c_str(), binding);
		}
	}

	for (const auto &itSampledImage : shaderResources.sampled_images) {
		if (m_pShaderCompiler->get_type(itSampledImage.type_id).basetype == spirv_cross::SPIRType::SampledImage) {
			const uint32_t binding = m_pShaderCompiler->get_decoration(itSampledImage.id, spv::DecorationBinding);
			SetSampledImageLocation(itSampledImage.name.c_str(), binding);
		}
	}

	return true;
}

void CGfxShader::Destroy(void)
{
	if (m_program) {
		glDeleteProgram(m_program);
	}

	if (m_pShaderCompiler) {
		delete m_pShaderCompiler;
	}

	m_kind = -1;
	m_program = 0;
	m_pShaderCompiler = NULL;

	m_uniformBlockBindings.clear();
	m_sampledImageLocations.clear();
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

void CGfxShader::SetSampledImageLocation(const char *szName, uint32_t binding)
{
	uint32_t name = HashValue(szName);

	if (m_sampledImageLocations.find(name) == m_sampledImageLocations.end()) {
		uint32_t location = glGetUniformLocation(m_program, szName);

		if (location != GL_INVALID_INDEX) {
			m_sampledImageLocations[name] = location;
		}
	}
}

bool CGfxShader::BindTexture2D(uint32_t name, uint32_t texture, uint32_t sampler, uint32_t unit) const
{
	const auto &itLocation = m_sampledImageLocations.find(name);

	if (itLocation != m_sampledImageLocations.end()) {
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindSampler(unit, sampler);
		glBindTexture(GL_TEXTURE_2D, texture);
		glProgramUniform1i(m_program, itLocation->second, unit);
		return true;
	}

	return false;
}

bool CGfxShader::BindTexture2DArray(uint32_t name, uint32_t texture, uint32_t sampler, uint32_t unit) const
{
	const auto &itLocation = m_sampledImageLocations.find(name);

	if (itLocation != m_sampledImageLocations.end()) {
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindSampler(unit, sampler);
		glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
		glProgramUniform1i(m_program, itLocation->second, unit);
		return true;
	}

	return false;
}

bool CGfxShader::BindTextureCubeMap(uint32_t name, uint32_t texture, uint32_t sampler, uint32_t unit) const
{
	const auto &itLocation = m_sampledImageLocations.find(name);

	if (itLocation != m_sampledImageLocations.end()) {
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindSampler(unit, sampler);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
		glProgramUniform1i(m_program, itLocation->second, unit);
		return true;
	}

	return false;
}

bool CGfxShader::BindUniformBuffer(uint32_t name, uint32_t buffer, uint32_t size, int offset) const
{
	const auto &itBinding = m_uniformBlockBindings.find(name);

	if (itBinding != m_uniformBlockBindings.end()) {
		glBindBufferRange(GL_UNIFORM_BUFFER, itBinding->second, buffer, offset, size);
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
