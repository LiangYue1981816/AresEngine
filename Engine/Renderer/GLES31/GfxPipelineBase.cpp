#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxPipelineBase.h"


CGfxPipelineBase::CGfxPipelineBase(uint32_t name)
	: m_name(name)
	, m_pipeline(0)
	, m_pShaders{ nullptr }
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

bool CGfxPipelineBase::IsUniformBlockValid(uint32_t name) const
{
	for (int index = 0; index < shaderc_compute_shader - shaderc_vertex_shader + 1; index++) {
		if (m_pShaders[index] && m_pShaders[index]->IsUniformBlockValid(name)) {
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

bool CGfxPipelineBase::BindTexture(uint32_t name, uint32_t target, uint32_t texture, uint32_t sampler, uint32_t unit) const
{
	bool rcode = false;

	for (int index = 0; index < shaderc_compute_shader - shaderc_vertex_shader + 1; index++) {
		if (m_pShaders[index]) {
			rcode = m_pShaders[index]->BindTexture(name, target, texture, sampler, unit) || rcode;
		}
	}

	return rcode;
}

bool CGfxPipelineBase::BindUniformBuffer(uint32_t name, uint32_t buffer, uint32_t size, int offset) const
{
	bool rcode = false;

	for (int index = 0; index < shaderc_compute_shader - shaderc_vertex_shader + 1; index++) {
		if (m_pShaders[index]) {
			rcode = m_pShaders[index]->BindUniformBuffer(name, buffer, size, offset) || rcode;
		}
	}

	return rcode;
}

bool CGfxPipelineBase::Uniform1f(uint32_t name, float v0) const
{
	bool rcode = false;

	for (int index = 0; index < shaderc_compute_shader - shaderc_vertex_shader + 1; index++) {
		if (m_pShaders[index]) {
			rcode = m_pShaders[index]->Uniform1f(name, v0) || rcode;
		}
	}

	return rcode;
}

bool CGfxPipelineBase::Uniform2f(uint32_t name, float v0, float v1) const
{
	bool rcode = false;

	for (int index = 0; index < shaderc_compute_shader - shaderc_vertex_shader + 1; index++) {
		if (m_pShaders[index]) {
			rcode = m_pShaders[index]->Uniform2f(name, v0, v1) || rcode;
		}
	}

	return rcode;
}

bool CGfxPipelineBase::Uniform3f(uint32_t name, float v0, float v1, float v2) const
{
	bool rcode = false;

	for (int index = 0; index < shaderc_compute_shader - shaderc_vertex_shader + 1; index++) {
		if (m_pShaders[index]) {
			rcode = m_pShaders[index]->Uniform3f(name, v0, v1, v2) || rcode;
		}
	}

	return rcode;
}

bool CGfxPipelineBase::Uniform4f(uint32_t name, float v0, float v1, float v2, float v3) const
{
	bool rcode = false;

	for (int index = 0; index < shaderc_compute_shader - shaderc_vertex_shader + 1; index++) {
		if (m_pShaders[index]) {
			rcode = m_pShaders[index]->Uniform4f(name, v0, v1, v2, v3) || rcode;
		}
	}

	return rcode;
}

bool CGfxPipelineBase::Uniform1fv(uint32_t name, int count, const float *value) const
{
	bool rcode = false;

	for (int index = 0; index < shaderc_compute_shader - shaderc_vertex_shader + 1; index++) {
		if (m_pShaders[index]) {
			rcode = m_pShaders[index]->Uniform1fv(name, count, value) || rcode;
		}
	}

	return rcode;
}

bool CGfxPipelineBase::Uniform2fv(uint32_t name, int count, const float *value) const
{
	bool rcode = false;

	for (int index = 0; index < shaderc_compute_shader - shaderc_vertex_shader + 1; index++) {
		if (m_pShaders[index]) {
			rcode = m_pShaders[index]->Uniform2fv(name, count, value) || rcode;
		}
	}

	return rcode;
}

bool CGfxPipelineBase::Uniform3fv(uint32_t name, int count, const float *value) const
{
	bool rcode = false;

	for (int index = 0; index < shaderc_compute_shader - shaderc_vertex_shader + 1; index++) {
		if (m_pShaders[index]) {
			rcode = m_pShaders[index]->Uniform3fv(name, count, value) || rcode;
		}
	}

	return rcode;
}

bool CGfxPipelineBase::Uniform4fv(uint32_t name, int count, const float *value) const
{
	bool rcode = false;

	for (int index = 0; index < shaderc_compute_shader - shaderc_vertex_shader + 1; index++) {
		if (m_pShaders[index]) {
			rcode = m_pShaders[index]->Uniform4fv(name, count, value) || rcode;
		}
	}

	return rcode;
}

bool CGfxPipelineBase::UniformMatrix2fv(uint32_t name, int count, const float *value) const
{
	bool rcode = false;

	for (int index = 0; index < shaderc_compute_shader - shaderc_vertex_shader + 1; index++) {
		if (m_pShaders[index]) {
			rcode = m_pShaders[index]->UniformMatrix2fv(name, count, value) || rcode;
		}
	}

	return rcode;
}

bool CGfxPipelineBase::UniformMatrix3fv(uint32_t name, int count, const float *value) const
{
	bool rcode = false;

	for (int index = 0; index < shaderc_compute_shader - shaderc_vertex_shader + 1; index++) {
		if (m_pShaders[index]) {
			rcode = m_pShaders[index]->UniformMatrix3fv(name, count, value) || rcode;
		}
	}

	return rcode;
}

bool CGfxPipelineBase::UniformMatrix4fv(uint32_t name, int count, const float *value) const
{
	bool rcode = false;

	for (int index = 0; index < shaderc_compute_shader - shaderc_vertex_shader + 1; index++) {
		if (m_pShaders[index]) {
			rcode = m_pShaders[index]->UniformMatrix4fv(name, count, value) || rcode;
		}
	}

	return rcode;
}
