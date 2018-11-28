#if GLES_VER == 300

#include "GfxHeader.h"


CGLES3Shader::CGLES3Shader(uint32_t name)
	: CGfxShader(name)

	, m_kind(-1)
	, m_shader(0)

	, m_pShaderCompiler(nullptr)
{

}

CGLES3Shader::~CGLES3Shader(void)
{
	Destroy();
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

			m_pShaderCompiler = new spirv_cross::CompilerGLSL(words, numWords);
			m_pShaderCompiler->set_options(options);

			const std::string strSource = m_pShaderCompiler->compile();
			const char *szSource = strSource.c_str();

#ifdef DEBUG
			LogOutput(nullptr, "\n");
			LogOutput(LOG_TAG_RENDERER, "\n%s\n", szSource);
#endif

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

				LogOutput(LOG_TAG_RENDERER, "%s\n", szSource);
				LogOutput(LOG_TAG_RENDERER, "Compile Error:\n");
				LogOutput(LOG_TAG_RENDERER, "%s\n", szError);

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

	delete m_pShaderCompiler;
	m_pShaderCompiler = nullptr;
}

bool CGLES3Shader::IsValid(void) const
{
	return m_shader != 0;
}

uint32_t CGLES3Shader::GetKind(void) const
{
	return m_kind;
}

uint32_t CGLES3Shader::GetShader(void) const
{
	return m_shader;
}

const spirv_cross::CompilerGLSL* CGLES3Shader::GetShaderCompiler(void) const
{
	return m_pShaderCompiler;
}

#endif
