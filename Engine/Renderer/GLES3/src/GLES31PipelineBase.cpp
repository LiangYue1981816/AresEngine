#if GLES_VER == 310

#include "GfxHeader.h"


CGLES3PipelineBase::CGLES3PipelineBase(void)
	: m_pipeline(0)
	, m_pShaders{ nullptr }
{
	glGenProgramPipelines(1, &m_pipeline);
}

CGLES3PipelineBase::~CGLES3PipelineBase(void)
{
	glDeleteProgramPipelines(1, &m_pipeline);
}

bool CGLES3PipelineBase::BindTexture(uint32_t name, CGLES3TextureBase *pTexture, CGLES3Sampler *pSampler, uint32_t unit)
{
	bool rcode = false;

	for (int index = 0; index < compute_shader - vertex_shader + 1; index++) {
		if (m_pShaders[index]) {
			rcode = m_pShaders[index]->BindTexture(name, pTexture, pSampler, unit) || rcode;
		}
	}

	return rcode;
}

bool CGLES3PipelineBase::BindUniformBuffer(uint32_t name, CGLES3UniformBuffer *pUniformBuffer, uint32_t size, int offset)
{
	bool rcode = false;

	for (int index = 0; index < compute_shader - vertex_shader + 1; index++) {
		if (m_pShaders[index]) {
			rcode = m_pShaders[index]->BindUniformBuffer(name, pUniformBuffer, size, offset) || rcode;
		}
	}

	return rcode;
}

bool CGLES3PipelineBase::Uniform1i(uint32_t name, int v0) const
{
	bool rcode = false;

	for (int index = 0; index < compute_shader - vertex_shader + 1; index++) {
		if (m_pShaders[index]) {
			rcode = m_pShaders[index]->Uniform1i(name, v0) || rcode;
		}
	}

	return rcode;
}

bool CGLES3PipelineBase::Uniform2i(uint32_t name, int v0, int v1) const
{
	bool rcode = false;

	for (int index = 0; index < compute_shader - vertex_shader + 1; index++) {
		if (m_pShaders[index]) {
			rcode = m_pShaders[index]->Uniform2i(name, v0, v1) || rcode;
		}
	}

	return rcode;
}

bool CGLES3PipelineBase::Uniform3i(uint32_t name, int v0, int v1, int v2) const
{
	bool rcode = false;

	for (int index = 0; index < compute_shader - vertex_shader + 1; index++) {
		if (m_pShaders[index]) {
			rcode = m_pShaders[index]->Uniform3i(name, v0, v1, v2) || rcode;
		}
	}

	return rcode;
}

bool CGLES3PipelineBase::Uniform4i(uint32_t name, int v0, int v1, int v2, int v3) const
{
	bool rcode = false;

	for (int index = 0; index < compute_shader - vertex_shader + 1; index++) {
		if (m_pShaders[index]) {
			rcode = m_pShaders[index]->Uniform4i(name, v0, v1, v2, v3) || rcode;
		}
	}

	return rcode;
}

bool CGLES3PipelineBase::Uniform1f(uint32_t name, float v0) const
{
	bool rcode = false;

	for (int index = 0; index < compute_shader - vertex_shader + 1; index++) {
		if (m_pShaders[index]) {
			rcode = m_pShaders[index]->Uniform1f(name, v0) || rcode;
		}
	}

	return rcode;
}

bool CGLES3PipelineBase::Uniform2f(uint32_t name, float v0, float v1) const
{
	bool rcode = false;

	for (int index = 0; index < compute_shader - vertex_shader + 1; index++) {
		if (m_pShaders[index]) {
			rcode = m_pShaders[index]->Uniform2f(name, v0, v1) || rcode;
		}
	}

	return rcode;
}

bool CGLES3PipelineBase::Uniform3f(uint32_t name, float v0, float v1, float v2) const
{
	bool rcode = false;

	for (int index = 0; index < compute_shader - vertex_shader + 1; index++) {
		if (m_pShaders[index]) {
			rcode = m_pShaders[index]->Uniform3f(name, v0, v1, v2) || rcode;
		}
	}

	return rcode;
}

bool CGLES3PipelineBase::Uniform4f(uint32_t name, float v0, float v1, float v2, float v3) const
{
	bool rcode = false;

	for (int index = 0; index < compute_shader - vertex_shader + 1; index++) {
		if (m_pShaders[index]) {
			rcode = m_pShaders[index]->Uniform4f(name, v0, v1, v2, v3) || rcode;
		}
	}

	return rcode;
}

bool CGLES3PipelineBase::Uniform1iv(uint32_t name, int count, const int *value) const
{
	bool rcode = false;

	for (int index = 0; index < compute_shader - vertex_shader + 1; index++) {
		if (m_pShaders[index]) {
			rcode = m_pShaders[index]->Uniform1iv(name, count, value) || rcode;
		}
	}

	return rcode;
}

bool CGLES3PipelineBase::Uniform2iv(uint32_t name, int count, const int *value) const
{
	bool rcode = false;

	for (int index = 0; index < compute_shader - vertex_shader + 1; index++) {
		if (m_pShaders[index]) {
			rcode = m_pShaders[index]->Uniform2iv(name, count, value) || rcode;
		}
	}

	return rcode;
}

bool CGLES3PipelineBase::Uniform3iv(uint32_t name, int count, const int *value) const
{
	bool rcode = false;

	for (int index = 0; index < compute_shader - vertex_shader + 1; index++) {
		if (m_pShaders[index]) {
			rcode = m_pShaders[index]->Uniform3iv(name, count, value) || rcode;
		}
	}

	return rcode;
}

bool CGLES3PipelineBase::Uniform4iv(uint32_t name, int count, const int *value) const
{
	bool rcode = false;

	for (int index = 0; index < compute_shader - vertex_shader + 1; index++) {
		if (m_pShaders[index]) {
			rcode = m_pShaders[index]->Uniform4iv(name, count, value) || rcode;
		}
	}

	return rcode;
}

bool CGLES3PipelineBase::Uniform1fv(uint32_t name, int count, const float *value) const
{
	bool rcode = false;

	for (int index = 0; index < compute_shader - vertex_shader + 1; index++) {
		if (m_pShaders[index]) {
			rcode = m_pShaders[index]->Uniform1fv(name, count, value) || rcode;
		}
	}

	return rcode;
}

bool CGLES3PipelineBase::Uniform2fv(uint32_t name, int count, const float *value) const
{
	bool rcode = false;

	for (int index = 0; index < compute_shader - vertex_shader + 1; index++) {
		if (m_pShaders[index]) {
			rcode = m_pShaders[index]->Uniform2fv(name, count, value) || rcode;
		}
	}

	return rcode;
}

bool CGLES3PipelineBase::Uniform3fv(uint32_t name, int count, const float *value) const
{
	bool rcode = false;

	for (int index = 0; index < compute_shader - vertex_shader + 1; index++) {
		if (m_pShaders[index]) {
			rcode = m_pShaders[index]->Uniform3fv(name, count, value) || rcode;
		}
	}

	return rcode;
}

bool CGLES3PipelineBase::Uniform4fv(uint32_t name, int count, const float *value) const
{
	bool rcode = false;

	for (int index = 0; index < compute_shader - vertex_shader + 1; index++) {
		if (m_pShaders[index]) {
			rcode = m_pShaders[index]->Uniform4fv(name, count, value) || rcode;
		}
	}

	return rcode;
}

bool CGLES3PipelineBase::UniformMatrix2fv(uint32_t name, int count, const float *value) const
{
	bool rcode = false;

	for (int index = 0; index < compute_shader - vertex_shader + 1; index++) {
		if (m_pShaders[index]) {
			rcode = m_pShaders[index]->UniformMatrix2fv(name, count, value) || rcode;
		}
	}

	return rcode;
}

bool CGLES3PipelineBase::UniformMatrix3fv(uint32_t name, int count, const float *value) const
{
	bool rcode = false;

	for (int index = 0; index < compute_shader - vertex_shader + 1; index++) {
		if (m_pShaders[index]) {
			rcode = m_pShaders[index]->UniformMatrix3fv(name, count, value) || rcode;
		}
	}

	return rcode;
}

bool CGLES3PipelineBase::UniformMatrix4fv(uint32_t name, int count, const float *value) const
{
	bool rcode = false;

	for (int index = 0; index < compute_shader - vertex_shader + 1; index++) {
		if (m_pShaders[index]) {
			rcode = m_pShaders[index]->UniformMatrix4fv(name, count, value) || rcode;
		}
	}

	return rcode;
}

bool CGLES3PipelineBase::IsTextureValid(uint32_t name) const
{
	for (int index = 0; index < compute_shader - vertex_shader + 1; index++) {
		if (m_pShaders[index] && m_pShaders[index]->IsTextureValid(name)) {
			return true;
		}
	}

	return false;
}

bool CGLES3PipelineBase::IsUniformValid(uint32_t name) const
{
	for (int index = 0; index < compute_shader - vertex_shader + 1; index++) {
		if (m_pShaders[index] && m_pShaders[index]->IsUniformValid(name)) {
			return true;
		}
	}

	return false;
}

bool CGLES3PipelineBase::IsUniformBlockValid(uint32_t name) const
{
	for (int index = 0; index < compute_shader - vertex_shader + 1; index++) {
		if (m_pShaders[index] && m_pShaders[index]->IsUniformBlockValid(name)) {
			return true;
		}
	}

	return false;
}

#endif
