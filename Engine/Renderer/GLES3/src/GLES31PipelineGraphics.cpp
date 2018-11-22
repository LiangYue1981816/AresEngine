#if GLES_VER == 310

#include "GfxHeader.h"


CGLES3PipelineGraphics::CGLES3PipelineGraphics(uint32_t name)
	: CGLES3PipelineBase(name)
{

}

CGLES3PipelineGraphics::~CGLES3PipelineGraphics(void)
{
	Destroy();
}

bool CGLES3PipelineGraphics::Create(const CGfxShader *pVertexShader, const CGfxShader *pFragmentShader, const PipelineState &state)
{
	if (pVertexShader == nullptr) {
		return false;
	}

	if (pVertexShader->IsValid() == false) {
		return false;
	}

	if (pVertexShader->GetKind() != vertex_shader) {
		return false;
	}

	if (pFragmentShader == nullptr) {
		return false;
	}

	if (pFragmentShader->IsValid() == false) {
		return false;
	}

	if (pFragmentShader->GetKind() != fragment_shader) {
		return false;
	}

	Destroy();

	m_state = state;
	m_pShaders[vertex_shader] = (CGLES3Shader *)pVertexShader;
	m_pShaders[fragment_shader] = (CGLES3Shader *)pFragmentShader;
	glUseProgramStages(m_pipeline, glGetProgramStage(vertex_shader), m_pShaders[vertex_shader]->GetProgram());
	glUseProgramStages(m_pipeline, glGetProgramStage(fragment_shader), m_pShaders[fragment_shader]->GetProgram());

	return true;
}

void CGLES3PipelineGraphics::Destroy(void)
{
	m_pShaders[vertex_shader] = nullptr;
	m_pShaders[fragment_shader] = nullptr;
	glUseProgramStages(m_pipeline, glGetProgramStage(vertex_shader), 0);
	glUseProgramStages(m_pipeline, glGetProgramStage(fragment_shader), 0);
}

void CGLES3PipelineGraphics::Bind(void *pParam)
{
	GLBindState(&m_state);
	GLBindProgramPipeline(m_pipeline);
}

#endif
