#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxPipelineCompute.h"


CGfxPipelineCompute::CGfxPipelineCompute(uint32_t name)
	: CGfxPipelineBase(name)
{

}

CGfxPipelineCompute::~CGfxPipelineCompute(void)
{
	Destroy();
}

bool CGfxPipelineCompute::Create(const CGfxShader *pComputeShader)
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
	CreateProgram(pComputeShader);

	return true;
}

void CGfxPipelineCompute::CreateProgram(const CGfxShader *pComputeShader)
{
	m_pShaders[shaderc_compute_shader] = (CGfxShader *)pComputeShader;
	glUseProgramStages(m_pipeline, glGetProgramStage(shaderc_compute_shader), m_pShaders[shaderc_compute_shader]->GetProgram());
}

void CGfxPipelineCompute::Destroy(void)
{
	m_pShaders[shaderc_compute_shader] = nullptr;
	glUseProgramStages(m_pipeline, glGetProgramStage(shaderc_compute_shader), 0);
}

void CGfxPipelineCompute::BindPipeline(void) const
{
	GLBindProgramPipeline(m_pipeline);
}
