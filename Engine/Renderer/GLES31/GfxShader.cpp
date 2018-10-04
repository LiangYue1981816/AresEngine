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

		return true;
	}
	catch (int) {
		Destroy();
		return false;
	}
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
}

bool CGfxShader::IsValid(void) const
{
	return m_program != 0;
}

uint32_t CGfxShader::GetKind(void) const
{
	return m_kind;
}

uint32_t CGfxShader::GetProgram(void) const
{
	return m_program;
}

spirv_cross::CompilerGLSL* CGfxShader::GetCompiler(void) const
{
	return m_pShaderCompiler;
}
