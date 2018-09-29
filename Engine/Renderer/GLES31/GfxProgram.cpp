#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxProgram.h"


CGfxProgram::CGfxProgram(GLuint64 name)
	: m_name(name)
	, m_program(0)
	, m_pShaders{ NULL }
{
	m_state.bEnableCullFace = GL_TRUE;
	m_state.bEnableDepthTest = GL_TRUE;
	m_state.bEnableDepthWrite = GL_TRUE;
	m_state.bEnableColorWrite[0] = GL_TRUE;
	m_state.bEnableColorWrite[1] = GL_TRUE;
	m_state.bEnableColorWrite[2] = GL_TRUE;
	m_state.bEnableColorWrite[3] = GL_TRUE;
	m_state.bEnableBlend = GL_FALSE;
	m_state.bEnablePolygonOffset = GL_FALSE;
	m_state.cullFace = GL_BACK;
	m_state.frontFace = GL_CCW;
	m_state.depthFunc = GL_LESS;
	m_state.srcBlendFactor = GL_SRC_ALPHA;
	m_state.dstBlendFactor = GL_ONE_MINUS_SRC_ALPHA;
	m_state.polygonOffsetFactor = 0.0f;
	m_state.polygonOffsetUnits = 0.0f;

	glGenProgramPipelines(1, &m_program);
}

CGfxProgram::~CGfxProgram(void)
{
	Destroy();
	glDeleteProgramPipelines(1, &m_program);
}

uint64_t CGfxProgram::GetName(void) const
{
	return m_name;
}

bool CGfxProgram::CreateCompute(CGfxShader *pComputeShader)
{
	m_pShaders[shaderc_shader_kind::shaderc_vertex_shader] = NULL;
	m_pShaders[shaderc_shader_kind::shaderc_fragment_shader] = NULL;
	m_pShaders[shaderc_shader_kind::shaderc_compute_shader] = pComputeShader;

	CreateProgram();
	CreateLocations();

	return true;
}

bool CGfxProgram::CreateGraphics(CGfxShader *pVertexShader, CGfxShader *pFragmentShader)
{
	m_pShaders[shaderc_shader_kind::shaderc_vertex_shader] = pVertexShader;
	m_pShaders[shaderc_shader_kind::shaderc_fragment_shader] = pFragmentShader;
	m_pShaders[shaderc_shader_kind::shaderc_compute_shader] = NULL;

	CreateProgram();
	CreateLocations();

	return true;
}

bool CGfxProgram::CreateProgram(void)
{
	glBindProgramPipeline(m_program);
	{
		for (int kind = shaderc_shader_kind::shaderc_vertex_shader; kind <= shaderc_shader_kind::shaderc_compute_shader; kind++) {
			glUseProgramStages(m_program, glGetShaderKind((shaderc_shader_kind)kind), m_pShaders[kind] ? m_pShaders[kind]->GetProgram() : 0);
		}
	}
	glBindProgramPipeline(0);

	return true;
}

bool CGfxProgram::CreateLocations(void)
{
	for (int kind = shaderc_shader_kind::shaderc_vertex_shader; kind <= shaderc_shader_kind::shaderc_compute_shader; kind++) {
		if (m_pShaders[kind] == NULL) {
			continue;
		}

		const spirv_cross::ShaderResources shaderResources = m_pShaders[kind]->GetCompiler()->get_shader_resources();

		for (const auto &itUniform : shaderResources.uniform_buffers) {
			if (m_pShaders[kind]->GetCompiler()->get_type(itUniform.type_id).basetype == spirv_cross::SPIRType::Struct) {
				SetUniformLocation(itUniform.name.c_str(), m_pShaders[kind]->GetProgram());
			}
		}

		for (const auto &itSampledImage : shaderResources.sampled_images) {
			if (m_pShaders[kind]->GetCompiler()->get_type(itSampledImage.type_id).basetype == spirv_cross::SPIRType::SampledImage) {
				SetTextureLocation(itSampledImage.name.c_str(), m_pShaders[kind]->GetProgram());
			}
		}
	}

	return true;
}

void CGfxProgram::Destroy(void)
{
	glBindProgramPipeline(m_program);
	{
		for (int kind = shaderc_shader_kind::shaderc_vertex_shader; kind <= shaderc_shader_kind::shaderc_compute_shader; kind++) {
			glUseProgramStages(m_program, glGetShaderKind((shaderc_shader_kind)kind), 0);
			m_pShaders[kind] = NULL;
		}
	}
	glBindProgramPipeline(0);

	m_names.clear();
	m_uniformBlockLocations.clear();
	m_sampledImageLocations.clear();
}

bool CGfxProgram::SetUniformLocation(const char *szName, GLuint program)
{
	GLuint name = HashValue(szName);

	if (m_uniformBlockLocations.find(name) == m_uniformBlockLocations.end()) {
		GLuint location = glGetUniformBlockIndex(program, szName);

		if (location != GL_INVALID_INDEX) {
			m_names[name] = szName;
			m_uniformBlockLocations[name] = location;
			glUniformBlockBinding(program, location, location);
			return true;
		}
	}

	return false;
}

bool CGfxProgram::SetTextureLocation(const char *szName, GLuint program)
{
	GLuint name = HashValue(szName);

	if (m_sampledImageLocations.find(name) == m_sampledImageLocations.end()) {
		GLuint location = glGetUniformLocation(program, szName);

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
	if (m_state.bEnableCullFace) {
		glEnable(GL_CULL_FACE);
	}
	else {
		glDisable(GL_CULL_FACE);
	}

	if (m_state.bEnableDepthTest) {
		glEnable(GL_DEPTH_TEST);
	}
	else {
		glDisable(GL_DEPTH_TEST);
	}

	if (m_state.bEnableDepthWrite) {
		glDepthMask(GL_TRUE);
	}
	else {
		glDepthMask(GL_FALSE);
	}

	if (m_state.bEnableBlend) {
		glEnable(GL_BLEND);
	}
	else {
		glDisable(GL_BLEND);
	}

	if (m_state.bEnablePolygonOffset) {
		glEnable(GL_POLYGON_OFFSET_FILL);
	}
	else {
		glDisable(GL_POLYGON_OFFSET_FILL);
	}

	glCullFace(m_state.cullFace);
	glFrontFace(m_state.frontFace);
	glDepthFunc(m_state.depthFunc);
	glBlendFunc(m_state.srcBlendFactor, m_state.dstBlendFactor);
	glPolygonOffset(m_state.polygonOffsetFactor, m_state.polygonOffsetUnits);
	glColorMask(m_state.bEnableColorWrite[0] ? GL_TRUE : GL_FALSE, m_state.bEnableColorWrite[1] ? GL_TRUE : GL_FALSE, m_state.bEnableColorWrite[2] ? GL_TRUE : GL_FALSE, m_state.bEnableColorWrite[3] ? GL_TRUE : GL_FALSE);

	glBindProgramPipeline(m_program);
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

void CGfxProgram::SetEnableCullFace(bool bEnable, GLenum cullFace, GLenum frontFace)
{
	m_state.bEnableCullFace = bEnable;
	m_state.cullFace = cullFace;
	m_state.frontFace = frontFace;
}

void CGfxProgram::SetEnableDepthTest(bool bEnable, GLenum depthFunc)
{
	m_state.bEnableDepthTest = bEnable;
	m_state.depthFunc = depthFunc;
}

void CGfxProgram::SetEnableDepthWrite(bool bEnable)
{
	m_state.bEnableDepthWrite = bEnable;
}

void CGfxProgram::SetEnableColorWrite(bool bEnableRed, bool bEnableGreen, bool bEnableBlue, bool bEnableAlpha)
{
	m_state.bEnableColorWrite[0] = bEnableRed;
	m_state.bEnableColorWrite[1] = bEnableGreen;
	m_state.bEnableColorWrite[2] = bEnableBlue;
	m_state.bEnableColorWrite[3] = bEnableAlpha;
}

void CGfxProgram::SetEnableBlend(bool bEnable, GLenum srcFactor, GLenum dstFactor)
{
	m_state.bEnableBlend = bEnable;
	m_state.srcBlendFactor = srcFactor;
	m_state.dstBlendFactor = dstFactor;
}

void CGfxProgram::SetEnablePolygonOffset(bool bEnable, GLfloat factor, GLfloat units)
{
	m_state.bEnablePolygonOffset = bEnable;
	m_state.polygonOffsetFactor = factor;
	m_state.polygonOffsetUnits = units;
}

bool CGfxProgram::IsEnableBlend(void) const
{
	return m_state.bEnableBlend == GL_TRUE ? true : false;
}

bool CGfxProgram::IsUniformValid(GLuint name) const
{
	return m_uniformBlockLocations.find(name) != m_uniformBlockLocations.end();
}

bool CGfxProgram::IsTextureValid(GLuint name) const
{
	return m_sampledImageLocations.find(name) != m_sampledImageLocations.end();
}
