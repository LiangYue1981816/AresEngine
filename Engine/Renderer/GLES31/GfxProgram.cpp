#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxProgram.h"


static size_t fsize(FILE *stream)
{
	long pos;
	size_t size;

	pos = ftell(stream);
	{
		fseek(stream, 0, SEEK_END);
		size = ftell(stream);
	}
	fseek(stream, pos, SEEK_SET);

	return size;
}

static eastl::string LoadShader(const char *szFileName)
{
	if (FILE *pFile = fopen(szFileName, "rb")) {
		static char szSource[128 * 1024];
		size_t size = fsize(pFile);

		fread(szSource, 1, size, pFile);
		fclose(pFile);

		return szSource;
	}

	return "";
}

static eastl::vector<GLuint> LoadShaderBinary(const char *szFileName)
{
	eastl::vector<GLuint> words;

	if (FILE *pFile = fopen(szFileName, "rb")) {
		size_t size = fsize(pFile);
		words.resize(size / sizeof(GLuint));
		fread(words.data(), sizeof(GLuint), words.size(), pFile);
		fclose(pFile);
	}

	return words;
}


CGfxProgram::CGfxProgram(void)
	: m_program(0)
	, m_vertexShader(0)
	, m_fragmentShader(0)
	, m_pShaderCompilers{ NULL }
{

}

CGfxProgram::~CGfxProgram(void)
{
	Free();
}

bool CGfxProgram::Load(const char *szVertexFileName, const char *szFragmentFileName)
{
	try {
		Free();

		if (LoadShader(szVertexFileName, GL_VERTEX_SHADER, m_vertexShader, m_pShaderCompilers[0]) == false) throw 0;
		if (LoadShader(szFragmentFileName, GL_FRAGMENT_SHADER, m_fragmentShader, m_pShaderCompilers[1]) == false) throw 1;
		if (CreateProgram() == false) throw 2;
		if (CreateLocations() == false) throw 3;

		return true;
	}
	catch (int) {
		Free();
		return false;
	}
}

bool CGfxProgram::LoadShader(const char *szFileName, GLenum type, GLuint &shader, spirv_cross::CompilerGLSL *&pShaderCompiler)
{
	shader = 0;
	pShaderCompiler = NULL;

	char szFullPath[260];
	Renderer()->GetShaderFullPath(szFileName, szFullPath);

	eastl::vector<GLuint> words = LoadShaderBinary(szFullPath);
	pShaderCompiler = new spirv_cross::CompilerGLSL(words.data(), words.size());

	spirv_cross::CompilerGLSL::Options options;
	options.version = 310;
	options.es = true;
	options.vertex.fixup_clipspace = false;
	pShaderCompiler->set_options(options);

	const eastl::string strSource = pShaderCompiler->compile().c_str();
	const char *szSource[1] = { strSource.c_str() };

	shader = glCreateShader(type);
	glShaderSource(shader, 1, szSource, NULL);
	glCompileShader(shader);

	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (success == GL_FALSE) {
		static char szError[128 * 1024];

		GLsizei length = 0;
		memset(szError, sizeof(szError), 0);
		glGetShaderInfoLog(shader, sizeof(szError), &length, szError);

		LogOutput("GfxRenderer", "Shader: %s\n", szFileName);
		LogOutput("GfxRenderer", "%s\n", strSource.c_str());
		LogOutput("GfxRenderer", "Compile Error:\n");
		LogOutput("GfxRenderer", "%s\n", szError);
	}

	return success == GL_TRUE;
}

bool CGfxProgram::CreateProgram(void)
{
	m_program = glCreateProgram();
	glAttachShader(m_program, m_vertexShader);
	glAttachShader(m_program, m_fragmentShader);
	glLinkProgram(m_program);

	GLint success;
	glGetProgramiv(m_program, GL_LINK_STATUS, &success);

	if (success == GL_FALSE) {
		static char szError[128 * 1024];

		GLsizei length = 0;
		memset(szError, sizeof(szError), 0);
		glGetProgramInfoLog(m_program, sizeof(szError), &length, szError);

		LogOutput("GfxRenderer", "Program Link Error:\n");
		LogOutput("GfxRenderer", "%s\n", szError);
	}

	return success == GL_TRUE;
}

bool CGfxProgram::CreateLocations(void)
{
	for (int index = 0; index < 2; index++) {
		const spirv_cross::ShaderResources shaderResources = m_pShaderCompilers[index]->get_shader_resources();

		for (const auto &itUniform : shaderResources.uniform_buffers) {
			if (m_pShaderCompilers[index]->get_type(itUniform.type_id).basetype == spirv_cross::SPIRType::Struct) {
				SetUniformLocation(itUniform.name.c_str());
			}
		}

		for (const auto &itSampledImage : shaderResources.sampled_images) {
			if (m_pShaderCompilers[index]->get_type(itSampledImage.type_id).basetype == spirv_cross::SPIRType::SampledImage) {
				SetTextureLocation(itSampledImage.name.c_str());
			}
		}
	}

	return true;
}

void CGfxProgram::Free(void)
{
	if (m_program) {
		glDeleteProgram(m_program);
	}

	if (m_vertexShader) {
		glDeleteShader(m_vertexShader);
	}

	if (m_fragmentShader) {
		glDeleteShader(m_fragmentShader);
	}

	if (m_pShaderCompilers[0]) {
		delete m_pShaderCompilers[0];
	}

	if (m_pShaderCompilers[1]) {
		delete m_pShaderCompilers[1];
	}

	m_program = 0;
	m_vertexShader = 0;
	m_fragmentShader = 0;
	m_pShaderCompilers[0] = NULL;
	m_pShaderCompilers[1] = NULL;

	m_names.clear();
	m_uniformBlockLocations.clear();
	m_sampledImageLocations.clear();
}

bool CGfxProgram::SetUniformLocation(const char *szName)
{
	GLuint name = HashValue(szName);

	if (m_uniformBlockLocations.find(name) == m_uniformBlockLocations.end()) {
		GLuint location = glGetUniformBlockIndex(m_program, szName);

		if (location != GL_INVALID_INDEX) {
			m_names[name] = szName;
			m_uniformBlockLocations[name] = location;
			glUniformBlockBinding(m_program, location, location);
			return true;
		}
	}

	return false;
}

bool CGfxProgram::SetTextureLocation(const char *szName)
{
	GLuint name = HashValue(szName);

	if (m_sampledImageLocations.find(name) == m_sampledImageLocations.end()) {
		GLuint location = glGetUniformLocation(m_program, szName);

		if (location != GL_INVALID_INDEX) {
			m_names[name] = szName;
			m_sampledImageLocations[name] = location;
			return true;
		}
	}

	return false;
}

void CGfxProgram::UseProgram(void) const
{
	glUseProgram(m_program);
}

bool CGfxProgram::BindUniformBuffer(GLuint name, GLuint buffer, GLsizeiptr size, GLintptr offset) const
{
	const auto &itLocation = m_uniformBlockLocations.find(name);

	if (itLocation != m_uniformBlockLocations.end()) {
		glBindBufferRange(GL_UNIFORM_BUFFER, itLocation->second, buffer, offset, size);
		return true;
	}

	return false;
}

bool CGfxProgram::BindTexture2D(GLuint name, GLuint texture, GLuint sampler, GLuint unit) const
{
	const auto &itLocation = m_sampledImageLocations.find(name);

	if (itLocation != m_sampledImageLocations.end()) {
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindSampler(unit, sampler);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(itLocation->second, unit);
		return true;
	}

	return false;
}

bool CGfxProgram::BindTextureArray(GLuint name, GLuint texture, GLuint sampler, GLuint unit) const
{
	const auto &itLocation = m_sampledImageLocations.find(name);

	if (itLocation != m_sampledImageLocations.end()) {
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindSampler(unit, sampler);
		glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
		glUniform1i(itLocation->second, unit);
		return true;
	}

	return false;
}

bool CGfxProgram::BindTextureCubeMap(GLuint name, GLuint texture, GLuint sampler, GLuint unit) const
{
	const auto &itLocation = m_sampledImageLocations.find(name);

	if (itLocation != m_sampledImageLocations.end()) {
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindSampler(unit, sampler);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
		glUniform1i(itLocation->second, unit);
		return true;
	}

	return false;
}

bool CGfxProgram::IsValid(void) const
{
	return m_program != 0;
}

bool CGfxProgram::IsUniformValid(GLuint name) const
{
	return m_uniformBlockLocations.find(name) != m_uniformBlockLocations.end();
}

bool CGfxProgram::IsTextureValid(GLuint name) const
{
	return m_sampledImageLocations.find(name) != m_sampledImageLocations.end();
}
