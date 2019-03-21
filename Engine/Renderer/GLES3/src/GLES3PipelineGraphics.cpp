#include "GLES3Renderer.h"


CGLES3PipelineGraphics::CGLES3PipelineGraphics(uint32_t name)
	: CGfxPipelineGraphics(name)
{

}

CGLES3PipelineGraphics::~CGLES3PipelineGraphics(void)
{
	Destroy();
}

HANDLE CGLES3PipelineGraphics::GetPipeline(void) const
{
	return (HANDLE)m_program;
}

const PipelineState* CGLES3PipelineGraphics::GetPipelineState(void) const
{
	return &m_state;
}

bool CGLES3PipelineGraphics::Create(const CGfxRenderPass *pRenderPass, const CGfxShader *pVertexShader, const CGfxShader *pFragmentShader, const PipelineState &state, uint32_t indexSubpass, uint32_t vertexBinding, uint32_t instanceBinding)
{
	if (pRenderPass == nullptr) {
		return false;
	}

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
	{
		do {
			m_state = state;

			m_pShaders[vertex_shader] = (CGLES3Shader *)pVertexShader;
			m_pShaders[fragment_shader] = (CGLES3Shader *)pFragmentShader;

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

void CGLES3PipelineGraphics::Destroy(void)
{
	CGLES3Pipeline::Destroy();
}

void CGLES3PipelineGraphics::Bind(void) const
{
	GLBindState(&m_state);
	GLUseProgram(m_program);
}
