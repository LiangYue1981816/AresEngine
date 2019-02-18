#include "GLES3Renderer.h"


CGLES3PipelineCompute::CGLES3PipelineCompute(uint32_t name)
	: CGfxPipelineCompute(name)
	, m_name(name)
{

}

CGLES3PipelineCompute::~CGLES3PipelineCompute(void)
{
	Destroy();
}

uint32_t CGLES3PipelineCompute::GetName(void) const
{
	return m_name;
}

HANDLE CGLES3PipelineCompute::GetPipeline(void) const
{
	return (HANDLE)m_program;
}

bool CGLES3PipelineCompute::Create(const CGfxShader *pComputeShader)
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

	return true;
}

void CGLES3PipelineCompute::Destroy(void)
{

}

bool CGLES3PipelineCompute::IsTextureValid(uint32_t name) const
{
	return CGLES3Pipeline::IsTextureValid(name);
}

bool CGLES3PipelineCompute::IsUniformValid(uint32_t name) const
{
	return CGLES3Pipeline::IsUniformValid(name);
}

bool CGLES3PipelineCompute::IsUniformBlockValid(uint32_t name) const
{
	return CGLES3Pipeline::IsUniformBlockValid(name);
}

void CGLES3PipelineCompute::Bind(void)
{

}
