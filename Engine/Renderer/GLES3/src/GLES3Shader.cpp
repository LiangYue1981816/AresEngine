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

HANDLE CGLES3Shader::GetShader(void) const
{
	return (HANDLE)m_program;
}

uint32_t CGLES3Shader::GetKind(void) const
{
	return m_kind;
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

bool CGLES3Shader::Create(const uint32_t *words, size_t numWords, shader_kind kind)
{
	Destroy();
	{
		do {
			const std::string strSource = m_spriv.Create(words, numWords, 310);
			const char *szSource = strSource.c_str();

#ifdef DEBUG
//			LogOutput(nullptr, "\n");
//			LogOutput(LOG_TAG_RENDERER, "\n%s\n", szSource);
#endif

			m_kind = kind;
			m_program = glCreateShaderProgramv(glGetShaderType(kind), 1, &szSource);
			if (m_program == 0) break;
			if (CreateLayouts() == false) break;

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

bool CGLES3Shader::CreateLayouts(void)
{
	const eastl::unordered_map<eastl::string, PushConstantRange> &pushConstantRanges = m_spriv.GetPushConstantRanges();
	const eastl::unordered_map<eastl::string, DescriptorSetBinding> &uniformBlockBindings = m_spriv.GetUniformBlockBindings();
	const eastl::unordered_map<eastl::string, DescriptorSetBinding> &sampledImageBindings = m_spriv.GetSampledImageBindings();
	const eastl::unordered_map<eastl::string, DescriptorSetBinding> &inputAttachmentBindings = m_spriv.GetInputAttachmentBindings();

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
