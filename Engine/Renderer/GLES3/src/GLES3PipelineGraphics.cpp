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

const CGfxDescriptorLayoutPtr CGLES3PipelineGraphics::GetDescriptorLayout(int indexDescriptorSet) const
{
	return m_pPipeline->GetDescriptorLayout(indexDescriptorSet);
}

const uint32_t CGLES3PipelineGraphics::GetInputAttachmentName(int indexInputAttachment) const
{
	return m_pPipeline->GetInputAttachmentName(indexInputAttachment);
}

bool CGLES3PipelineGraphics::Create(const CGfxRenderPass* pRenderPass, const CGfxShader* pVertexShader, const CGfxShader* pFragmentShader, const PipelineState& state, int indexSubpass, int vertexBinding, int instanceBinding)
{
	m_state = state;
	return m_pPipeline->Create(pRenderPass, pVertexShader, pFragmentShader, state, indexSubpass, vertexBinding, instanceBinding);
}

void CGLES3PipelineGraphics::Destroy(void)
{
	m_pPipeline->Destroy();
}

bool CGLES3PipelineGraphics::IsTransparency(void) const
{
	return m_state.bEnableBlend;
}

bool CGLES3PipelineGraphics::IsCompatibleVertexFormat(uint32_t binding, uint32_t format) const
{
	return m_pPipeline->IsCompatibleVertexFormat(binding, format);
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

void CGLES3PipelineGraphics::Uniform1iv(uint32_t name, int count, const int* value) const
{
	m_pPipeline->Uniform1iv(name, count, value);
}

void CGLES3PipelineGraphics::Uniform2iv(uint32_t name, int count, const int* value) const
{
	m_pPipeline->Uniform2iv(name, count, value);
}

void CGLES3PipelineGraphics::Uniform3iv(uint32_t name, int count, const int* value) const
{
	m_pPipeline->Uniform3iv(name, count, value);
}

void CGLES3PipelineGraphics::Uniform4iv(uint32_t name, int count, const int* value) const
{
	m_pPipeline->Uniform4iv(name, count, value);
}

void CGLES3PipelineGraphics::Uniform1fv(uint32_t name, int count, const float* value) const
{
	m_pPipeline->Uniform1fv(name, count, value);
}

void CGLES3PipelineGraphics::Uniform2fv(uint32_t name, int count, const float* value) const
{
	m_pPipeline->Uniform2fv(name, count, value);
}

void CGLES3PipelineGraphics::Uniform3fv(uint32_t name, int count, const float* value) const
{
	m_pPipeline->Uniform3fv(name, count, value);
}

void CGLES3PipelineGraphics::Uniform4fv(uint32_t name, int count, const float* value) const
{
	m_pPipeline->Uniform4fv(name, count, value);
}

void CGLES3PipelineGraphics::UniformMatrix2fv(uint32_t name, int count, const float* value) const
{
	m_pPipeline->UniformMatrix2fv(name, count, value);
}

void CGLES3PipelineGraphics::UniformMatrix3fv(uint32_t name, int count, const float* value) const
{
	m_pPipeline->UniformMatrix3fv(name, count, value);
}

void CGLES3PipelineGraphics::UniformMatrix4fv(uint32_t name, int count, const float* value) const
{
	m_pPipeline->UniformMatrix4fv(name, count, value);
}
