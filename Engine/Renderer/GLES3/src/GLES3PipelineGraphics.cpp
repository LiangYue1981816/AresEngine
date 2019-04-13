#include "GLES3Renderer.h"


CGLES3PipelineGraphics::CGLES3PipelineGraphics(uint32_t name)
	: CGfxPipelineGraphics(name)
	, m_pPipeline(nullptr)
{
	m_pPipeline = new CGLES3Pipeline;
}

CGLES3PipelineGraphics::~CGLES3PipelineGraphics(void)
{
	delete m_pPipeline;
}

const CGfxDescriptorLayoutPtr CGLES3PipelineGraphics::GetDescriptorLayout(uint32_t indexDescriptorSet) const
{
	return m_pPipeline->GetDescriptorLayout(indexDescriptorSet);
}

uint32_t CGLES3PipelineGraphics::GetInputAttachmentName(uint32_t inputAttachmentIndex) const
{
	return m_pPipeline->GetInputAttachmentName(inputAttachmentIndex);
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

			if (m_pPipeline->CreateProgram((const CGLES3Shader *)pVertexShader, (const CGLES3Shader *)pFragmentShader, nullptr) == false) {
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

void CGLES3PipelineGraphics::Destroy(void)
{
	m_pPipeline->Destroy();
}

void CGLES3PipelineGraphics::Bind(void) const
{
	m_pPipeline->Bind(&m_state);
}

bool CGLES3PipelineGraphics::BindDescriptorSet(const CGfxDescriptorSetPtr ptrDescriptorSet) const
{
	return m_pPipeline->BindDescriptorSet(ptrDescriptorSet);
}

void CGLES3PipelineGraphics::Uniform1i(uint32_t name, int v0) const
{
	m_pPipeline->Uniform1i(name, v0);
}

void CGLES3PipelineGraphics::Uniform2i(uint32_t name, int v0, int v1) const
{
	m_pPipeline->Uniform2i(name, v0, v1);
}

void CGLES3PipelineGraphics::Uniform3i(uint32_t name, int v0, int v1, int v2) const
{
	m_pPipeline->Uniform3i(name, v0, v1, v2);
}

void CGLES3PipelineGraphics::Uniform4i(uint32_t name, int v0, int v1, int v2, int v3) const
{
	m_pPipeline->Uniform4i(name, v0, v1, v2, v3);
}

void CGLES3PipelineGraphics::Uniform1f(uint32_t name, float v0) const
{
	m_pPipeline->Uniform1f(name, v0);
}

void CGLES3PipelineGraphics::Uniform2f(uint32_t name, float v0, float v1) const
{
	m_pPipeline->Uniform2f(name, v0, v1);
}

void CGLES3PipelineGraphics::Uniform3f(uint32_t name, float v0, float v1, float v2) const
{
	m_pPipeline->Uniform3f(name, v0, v1, v2);
}

void CGLES3PipelineGraphics::Uniform4f(uint32_t name, float v0, float v1, float v2, float v3) const
{
	m_pPipeline->Uniform4f(name, v0, v1, v2, v3);
}

void CGLES3PipelineGraphics::Uniform1iv(uint32_t name, int count, const int *value) const
{
	m_pPipeline->Uniform1iv(name, count, value);
}

void CGLES3PipelineGraphics::Uniform2iv(uint32_t name, int count, const int *value) const
{
	m_pPipeline->Uniform2iv(name, count, value);
}

void CGLES3PipelineGraphics::Uniform3iv(uint32_t name, int count, const int *value) const
{
	m_pPipeline->Uniform3iv(name, count, value);
}

void CGLES3PipelineGraphics::Uniform4iv(uint32_t name, int count, const int *value) const
{
	m_pPipeline->Uniform4iv(name, count, value);
}

void CGLES3PipelineGraphics::Uniform1fv(uint32_t name, int count, const float *value) const
{
	m_pPipeline->Uniform1fv(name, count, value);
}

void CGLES3PipelineGraphics::Uniform2fv(uint32_t name, int count, const float *value) const
{
	m_pPipeline->Uniform2fv(name, count, value);
}

void CGLES3PipelineGraphics::Uniform3fv(uint32_t name, int count, const float *value) const
{
	m_pPipeline->Uniform3fv(name, count, value);
}

void CGLES3PipelineGraphics::Uniform4fv(uint32_t name, int count, const float *value) const
{
	m_pPipeline->Uniform4fv(name, count, value);
}

void CGLES3PipelineGraphics::UniformMatrix2fv(uint32_t name, int count, const float *value) const
{
	m_pPipeline->UniformMatrix2fv(name, count, value);
}

void CGLES3PipelineGraphics::UniformMatrix3fv(uint32_t name, int count, const float *value) const
{
	m_pPipeline->UniformMatrix3fv(name, count, value);
}

void CGLES3PipelineGraphics::UniformMatrix4fv(uint32_t name, int count, const float *value) const
{
	m_pPipeline->UniformMatrix4fv(name, count, value);
}
