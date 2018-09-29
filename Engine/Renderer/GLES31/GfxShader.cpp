#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxShader.h"


CGfxShader::CGfxShader(uint32_t name)
	: m_name(name)
	, m_kind(0)
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

bool CGfxShader::Create(const uint32_t *words, size_t numWords, shaderc_shader_kind kind)
{
	m_pShaderCompiler = new spirv_cross::CompilerGLSL(words, numWords);

	spirv_cross::CompilerGLSL::Options options;
	options.version = 310;
	options.es = true;
	options.vertex.fixup_clipspace = false;
	m_pShaderCompiler->set_options(options);

	const eastl::string strSource = m_pShaderCompiler->compile().c_str();
	const char *szSource = strSource.c_str();

	m_kind = kind;
	m_program = glCreateShaderProgramv(glGetShaderKind(kind), 1, &szSource);

	return m_program != 0;
}

void CGfxShader::Destroy(void)
{
	if (m_program) {
		glDeleteProgram(m_program);
	}

	if (m_pShaderCompiler) {
		delete m_pShaderCompiler;
	}

	m_kind = 0;
	m_program = 0;
	m_pShaderCompiler = NULL;
}

GLuint CGfxShader::GetKind(void) const
{
	return m_kind;
}

GLuint CGfxShader::GetProgram(void) const
{
	return m_program;
}

spirv_cross::CompilerGLSL* CGfxShader::GetCompiler(void) const
{
	return m_pShaderCompiler;
}
