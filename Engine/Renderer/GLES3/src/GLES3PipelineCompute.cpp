#include "GLES3Renderer.h"


CGLES3PipelineCompute::CGLES3PipelineCompute(uint32_t name)
	: CGfxPipelineCompute(name)
{

}

CGLES3PipelineCompute::~CGLES3PipelineCompute(void)
{
	Destroy();
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

	Destroy();
	{
		do {
			m_pShaders[compute_shader] = (CGLES3Shader *)pComputeShader;

			if (CreateProgram() == false) {
				break;
			}

			if (CreateLayouts() == false) {
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
	CGLES3Pipeline::Destroy();
}

void CGLES3PipelineCompute::Bind(void) const
{
	GLUseProgram(m_program);
}
