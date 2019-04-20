#include "GLES3Renderer.h"


CGLES3PipelineCompute::CGLES3PipelineCompute(uint32_t name)
	: CGfxPipelineCompute(name)
	, m_pPipeline(nullptr)
{
	m_pPipeline = new CGLES3Pipeline;
}

CGLES3PipelineCompute::~CGLES3PipelineCompute(void)
{
	delete m_pPipeline;
}

const CGfxDescriptorLayoutPtr CGLES3PipelineCompute::GetDescriptorLayout(int indexDescriptorSet) const
{
	return m_pPipeline->GetDescriptorLayout(indexDescriptorSet);
}

bool CGLES3PipelineCompute::Create(const CGfxShader* pComputeShader)
{
	if (pComputeShader == nullptr) {
		return false;
	}

	if (pComputeShader->IsValid() == false) {
		return false;
	}

	if (pComputeShader->GetKind() != compute_shader) {
		return false;
	}

	Destroy();
	{
		do {
			if (m_pPipeline->CreateProgram(nullptr, nullptr, (const CGLES3Shader*)pComputeShader) == false) {
				break;
			}

			if (m_pPipeline->CreateLayouts() == false) {
				break;
			}

			return true;
		} while (false);
	}
	Destroy();
	return true;
}

void CGLES3PipelineCompute::Destroy(void)
{
	m_pPipeline->Destroy();
}

void CGLES3PipelineCompute::Bind(void) const
{
	m_pPipeline->Bind(nullptr);
}

bool CGLES3PipelineCompute::BindDescriptorSet(const CGfxDescriptorSetPtr ptrDescriptorSet) const
{
	return m_pPipeline->BindDescriptorSet(ptrDescriptorSet);
}

void CGLES3PipelineCompute::Uniform1i(uint32_t name, int v0) const
{
	m_pPipeline->Uniform1i(name, v0);
}

void CGLES3PipelineCompute::Uniform2i(uint32_t name, int v0, int v1) const
{
	m_pPipeline->Uniform2i(name, v0, v1);
}

void CGLES3PipelineCompute::Uniform3i(uint32_t name, int v0, int v1, int v2) const
{
	m_pPipeline->Uniform3i(name, v0, v1, v2);
}

void CGLES3PipelineCompute::Uniform4i(uint32_t name, int v0, int v1, int v2, int v3) const
{
	m_pPipeline->Uniform4i(name, v0, v1, v2, v3);
}

void CGLES3PipelineCompute::Uniform1f(uint32_t name, float v0) const
{
	m_pPipeline->Uniform1f(name, v0);
}

void CGLES3PipelineCompute::Uniform2f(uint32_t name, float v0, float v1) const
{
	m_pPipeline->Uniform2f(name, v0, v1);
}

void CGLES3PipelineCompute::Uniform3f(uint32_t name, float v0, float v1, float v2) const
{
	m_pPipeline->Uniform3f(name, v0, v1, v2);
}

void CGLES3PipelineCompute::Uniform4f(uint32_t name, float v0, float v1, float v2, float v3) const
{
	m_pPipeline->Uniform4f(name, v0, v1, v2, v3);
}

void CGLES3PipelineCompute::Uniform1iv(uint32_t name, int count, const int* value) const
{
	m_pPipeline->Uniform1iv(name, count, value);
}

void CGLES3PipelineCompute::Uniform2iv(uint32_t name, int count, const int* value) const
{
	m_pPipeline->Uniform2iv(name, count, value);
}

void CGLES3PipelineCompute::Uniform3iv(uint32_t name, int count, const int* value) const
{
	m_pPipeline->Uniform3iv(name, count, value);
}

void CGLES3PipelineCompute::Uniform4iv(uint32_t name, int count, const int* value) const
{
	m_pPipeline->Uniform4iv(name, count, value);
}

void CGLES3PipelineCompute::Uniform1fv(uint32_t name, int count, const float* value) const
{
	m_pPipeline->Uniform1fv(name, count, value);
}

void CGLES3PipelineCompute::Uniform2fv(uint32_t name, int count, const float* value) const
{
	m_pPipeline->Uniform2fv(name, count, value);
}

void CGLES3PipelineCompute::Uniform3fv(uint32_t name, int count, const float* value) const
{
	m_pPipeline->Uniform3fv(name, count, value);
}

void CGLES3PipelineCompute::Uniform4fv(uint32_t name, int count, const float* value) const
{
	m_pPipeline->Uniform4fv(name, count, value);
}

void CGLES3PipelineCompute::UniformMatrix2fv(uint32_t name, int count, const float* value) const
{
	m_pPipeline->UniformMatrix2fv(name, count, value);
}

void CGLES3PipelineCompute::UniformMatrix3fv(uint32_t name, int count, const float* value) const
{
	m_pPipeline->UniformMatrix3fv(name, count, value);
}

void CGLES3PipelineCompute::UniformMatrix4fv(uint32_t name, int count, const float* value) const
{
	m_pPipeline->UniformMatrix4fv(name, count, value);
}
