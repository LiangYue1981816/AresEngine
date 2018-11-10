#include "GLES3Renderer.h"
#include "GLES3PipelineCompute.h"


CGLES3PipelineCompute::CGLES3PipelineCompute(uint32_t name)
	: CGLES3PipelineBase(name)
{

}

CGLES3PipelineCompute::~CGLES3PipelineCompute(void)
{
	Destroy();
}

bool CGLES3PipelineCompute::Create(const CGfxShader *pComputeShader)
{
	if (pComputeShader == nullptr) {
		return false;
	}

	if (pComputeShader->IsValid() == false) {
		return false;
	}

	if (pComputeShader->GetKind() != shaderc_compute_shader) {
		return false;
	}

	Destroy();

	m_pShaders[shaderc_compute_shader] = (CGLES3Shader *)pComputeShader;
	glUseProgramStages(m_pipeline, glGetProgramStage(shaderc_compute_shader), m_pShaders[shaderc_compute_shader]->GetProgram());

	return true;
}

void CGLES3PipelineCompute::Destroy(void)
{
	m_pShaders[shaderc_compute_shader] = nullptr;
	glUseProgramStages(m_pipeline, glGetProgramStage(shaderc_compute_shader), 0);
}

void CGLES3PipelineCompute::Bind(void *pParam)
{
	GLBindProgramPipeline(m_pipeline);
}
