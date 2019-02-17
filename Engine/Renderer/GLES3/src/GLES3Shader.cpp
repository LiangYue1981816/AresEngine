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

const CGfxSprivCross& CGLES3Shader::GetSprivCross(void) const
{
	return m_spriv;
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
}

bool CGLES3Shader::IsValid(void) const
{
	return m_program != 0;
}
