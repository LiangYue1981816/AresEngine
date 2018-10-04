#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxPipelineCompute.h"


CGfxPipelineCompute::CGfxPipelineCompute(uint32_t name)
	: CGfxPipelineBase(name)
{

}

CGfxPipelineCompute::~CGfxPipelineCompute(void)
{

}

bool CGfxPipelineCompute::Create(const CGfxShader *pComputeShader)
{
	if (pComputeShader == NULL) {
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
	CreateLocations(pComputeShader);

	return true;
}

bool CGfxPipelineCompute::CreateProgram(const CGfxShader *pComputeShader)
{
	glBindProgramPipeline(m_pipeline);
	{
		glUseProgramStages(m_pipeline, glGetShaderKind(shaderc_compute_shader), pComputeShader->GetProgram());
	}
	glBindProgramPipeline(0);

	return true;
}

void CGfxPipelineCompute::BindPipeline(void) const
{
	glBindProgramPipeline(m_pipeline);
}
