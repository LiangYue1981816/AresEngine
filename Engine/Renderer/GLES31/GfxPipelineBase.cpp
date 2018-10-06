#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxPipelineBase.h"


CGfxPipelineBase::CGfxPipelineBase(uint32_t name)
	: m_name(name)
	, m_pipeline(0)
	, m_pShaders{ NULL }
{
	glGenProgramPipelines(1, &m_pipeline);
}

CGfxPipelineBase::~CGfxPipelineBase(void)
{
	glDeleteProgramPipelines(1, &m_pipeline);
}

uint32_t CGfxPipelineBase::GetName(void) const
{
	return m_name;
}

bool CGfxPipelineBase::IsUniformValid(uint32_t name) const
{
	for (int index = 0; index < shaderc_compute_shader - shaderc_vertex_shader + 1; index++) {
		if (m_pShaders[index] && m_pShaders[index]->IsUniformValid(name)) {
			return true;
		}
	}
	return false;
}

bool CGfxPipelineBase::IsTextureValid(uint32_t name) const
{
	for (int index = 0; index < shaderc_compute_shader - shaderc_vertex_shader + 1; index++) {
		if (m_pShaders[index] && m_pShaders[index]->IsTextureValid(name)) {
			return true;
		}
	}
	return false;
}

bool CGfxPipelineBase::BindTexture2D(uint32_t name, uint32_t texture, uint32_t sampler, uint32_t unit) const
{
	bool isBinded = false;

	for (int index = 0; index < shaderc_compute_shader - shaderc_vertex_shader + 1; index++) {
		if (m_pShaders[index]) {
			isBinded = m_pShaders[index]->BindTexture2D(name, texture, sampler, unit) || isBinded;
		}
	}

	return isBinded;
}

bool CGfxPipelineBase::BindTexture2DArray(uint32_t name, uint32_t texture, uint32_t sampler, uint32_t unit) const
{
	bool isBinded = false;

	for (int index = 0; index < shaderc_compute_shader - shaderc_vertex_shader + 1; index++) {
		if (m_pShaders[index]) {
			isBinded = m_pShaders[index]->BindTexture2DArray(name, texture, sampler, unit) || isBinded;
		}
	}

	return isBinded;
}

bool CGfxPipelineBase::BindTextureCubeMap(uint32_t name, uint32_t texture, uint32_t sampler, uint32_t unit) const
{
	bool isBinded = false;

	for (int index = 0; index < shaderc_compute_shader - shaderc_vertex_shader + 1; index++) {
		if (m_pShaders[index]) {
			isBinded = m_pShaders[index]->BindTextureCubeMap(name, texture, sampler, unit) || isBinded;
		}
	}

	return isBinded;
}

bool CGfxPipelineBase::BindUniformBuffer(uint32_t name, uint32_t buffer, uint32_t size, int offset) const
{
	bool isBinded = false;

	for (int index = 0; index < shaderc_compute_shader - shaderc_vertex_shader + 1; index++) {
		if (m_pShaders[index]) {
			isBinded = m_pShaders[index]->BindUniformBuffer(name, buffer, size, offset) || isBinded;
		}
	}

	return isBinded;
}
