#include "GLES3Renderer.h"


CGLES3Shader::CGLES3Shader(uint32_t name)
	: CGfxShader(name)

	, m_kind(-1)
	, m_shader(0)
{

}

CGLES3Shader::~CGLES3Shader(void)
{
	Destroy();
}

uint32_t CGLES3Shader::GetShader(void) const
{
	return m_shader;
}

uint32_t CGLES3Shader::GetKind(void) const
{
	return m_kind;
}

const CGfxSprivCross& CGLES3Shader::GetSprivCross(void) const
{
	return m_spriv;
}

bool CGLES3Shader::Create(const uint32_t* words, size_t numWords, shader_kind kind)
{
	Destroy();
	{
		do {
			const eastl::string strSource = m_spriv.Create(words, numWords, 310);
			const char* szSource = strSource.c_str();

			m_kind = kind;
			m_shader = glCreateShader(glGetShaderType(kind));
			glShaderSource(m_shader, 1, &szSource, nullptr);
			glCompileShader(m_shader);

			GLint success;
			glGetShaderiv(m_shader, GL_COMPILE_STATUS, &success);

			if (success == GL_FALSE) {
				GLsizei length = 0;
				char szError[128 * 1024] = { 0 };

				glGetShaderInfoLog(m_shader, sizeof(szError), &length, szError);

				LogOutput(nullptr, "%s\n", szSource);
				LogOutput(nullptr, "Compile Error:\n");
				LogOutput(nullptr, "%s\n", szError);

				break;
			}

			return true;
		} while (false);
	}
	Destroy();
	return false;
}

void CGLES3Shader::Destroy(void)
{
	if (m_shader) {
		glDeleteShader(m_shader);
	}

	m_kind = -1;
	m_shader = 0;
}

bool CGLES3Shader::IsValid(void) const
{
	return m_shader != 0;
}
