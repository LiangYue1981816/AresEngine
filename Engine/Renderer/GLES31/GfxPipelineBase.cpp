#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxPipelineBase.h"


CGfxPipelineBase::CGfxPipelineBase(uint32_t name)
	: m_name(name)
	, m_pipeline(0)
{
	glGenProgramPipelines(1, &m_pipeline);
}

CGfxPipelineBase::~CGfxPipelineBase(void)
{
	Destroy();
	glDeleteProgramPipelines(1, &m_pipeline);
}

uint32_t CGfxPipelineBase::GetName(void) const
{
	return m_name;
}

bool CGfxPipelineBase::CreateLocations(const CGfxShader *pShader)
{
	const spirv_cross::ShaderResources shaderResources = pShader->GetCompiler()->get_shader_resources();

	for (const auto &itUniform : shaderResources.uniform_buffers) {
		if (pShader->GetCompiler()->get_type(itUniform.type_id).basetype == spirv_cross::SPIRType::Struct) {
			SetUniformLocation(itUniform.name.c_str(), pShader->GetProgram());
		}
	}

	for (const auto &itSampledImage : shaderResources.sampled_images) {
		if (pShader->GetCompiler()->get_type(itSampledImage.type_id).basetype == spirv_cross::SPIRType::SampledImage) {
			SetTextureLocation(itSampledImage.name.c_str(), pShader->GetProgram());
		}
	}

	return true;
}

void CGfxPipelineBase::Destroy(void)
{
	glBindProgramPipeline(m_pipeline);
	{
		glUseProgramStages(m_pipeline, glGetShaderKind(shaderc_vertex_shader), 0);
		glUseProgramStages(m_pipeline, glGetShaderKind(shaderc_fragment_shader), 0);
		glUseProgramStages(m_pipeline, glGetShaderKind(shaderc_compute_shader), 0);
	}
	glBindProgramPipeline(0);

	m_uniformBlockLocations.clear();
	m_sampledImageLocations.clear();
}

bool CGfxPipelineBase::SetUniformLocation(const char *szName, uint32_t program)
{
	uint32_t name = HashValue(szName);

	if (m_uniformBlockLocations.find(name) == m_uniformBlockLocations.end()) {
		uint32_t location = glGetUniformBlockIndex(program, szName);

		if (location != GL_INVALID_INDEX) {
			m_uniformBlockLocations[name] = location;
			glUniformBlockBinding(program, location, location);
			return true;
		}
	}

	return false;
}

bool CGfxPipelineBase::SetTextureLocation(const char *szName, uint32_t program)
{
	uint32_t name = HashValue(szName);

	if (m_sampledImageLocations.find(name) == m_sampledImageLocations.end()) {
		uint32_t location = glGetUniformLocation(program, szName);

		if (location != GL_INVALID_INDEX) {
			m_sampledImageLocations[name] = location;
			return true;
		}
	}

	return false;
}

bool CGfxPipelineBase::IsUniformValid(uint32_t name) const
{
	return m_uniformBlockLocations.find(name) != m_uniformBlockLocations.end();
}

bool CGfxPipelineBase::IsTextureValid(uint32_t name) const
{
	return m_sampledImageLocations.find(name) != m_sampledImageLocations.end();
}


bool CGfxPipelineBase::BindTexture2D(uint32_t name, uint32_t texture, uint32_t sampler, uint32_t unit) const
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

bool CGfxPipelineBase::BindTexture2DArray(uint32_t name, uint32_t texture, uint32_t sampler, uint32_t unit) const
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

bool CGfxPipelineBase::BindTextureCubeMap(uint32_t name, uint32_t texture, uint32_t sampler, uint32_t unit) const
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

bool CGfxPipelineBase::BindUniformBuffer(uint32_t name, uint32_t buffer, uint32_t size, int offset) const
{
	const auto &itLocation = m_uniformBlockLocations.find(name);

	if (itLocation != m_uniformBlockLocations.end()) {
		glBindBufferRange(GL_UNIFORM_BUFFER, itLocation->second, buffer, offset, size);
		return true;
	}

	return false;
}
