#include "VKRenderer.h"


CVKPipelineGraphics::CVKPipelineGraphics(CVKDevice* pDevice, uint32_t name)
	: CGfxPipelineGraphics(name)
	, m_pDevice(pDevice)
	, m_pPipeline(nullptr)
{
	ASSERT(m_pDevice);

	m_pPipeline = new CVKPipeline(m_pDevice);
}

CVKPipelineGraphics::~CVKPipelineGraphics(void)
{
	delete m_pPipeline;
}

const CGfxDescriptorLayoutPtr CVKPipelineGraphics::GetDescriptorLayout(int indexDescriptorSet) const
{
	return m_pPipeline->GetDescriptorLayout(indexDescriptorSet);
}

const uint32_t CVKPipelineGraphics::GetInputAttachmentName(uint32_t inputAttachmentIndex) const
{
	return m_pPipeline->GetInputAttachmentName(inputAttachmentIndex);
}

const bool CVKPipelineGraphics::IsCompatibleVertexFormat(uint32_t binding, uint32_t format) const
{
	return m_pPipeline->IsCompatibleVertexFormat(binding, format);
}

bool CVKPipelineGraphics::Create(const CGfxRenderPass* pRenderPass, const CGfxShader* pVertexShader, const CGfxShader* pFragmentShader, const PipelineState& state, int indexSubpass, int vertexBinding, int instanceBinding)
{
	return m_pPipeline->Create(pRenderPass, pVertexShader, pFragmentShader, state, indexSubpass, vertexBinding, instanceBinding);
}

void CVKPipelineGraphics::Destroy(void)
{
	m_pPipeline->Destroy();
}

void CVKPipelineGraphics::Bind(VkCommandBuffer vkCommandBuffer)
{
	m_pPipeline->Bind(vkCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS);
}

void CVKPipelineGraphics::Uniform1i(VkCommandBuffer vkCommandBuffer, uint32_t name, int v0) const
{
	m_pPipeline->Uniform1i(vkCommandBuffer, name, v0);
}

void CVKPipelineGraphics::Uniform2i(VkCommandBuffer vkCommandBuffer, uint32_t name, int v0, int v1) const
{
	m_pPipeline->Uniform2i(vkCommandBuffer, name, v0, v1);
}

void CVKPipelineGraphics::Uniform3i(VkCommandBuffer vkCommandBuffer, uint32_t name, int v0, int v1, int v2) const
{
	m_pPipeline->Uniform3i(vkCommandBuffer, name, v0, v1, v2);
}

void CVKPipelineGraphics::Uniform4i(VkCommandBuffer vkCommandBuffer, uint32_t name, int v0, int v1, int v2, int v3) const
{
	m_pPipeline->Uniform4i(vkCommandBuffer, name, v0, v1, v2, v3);
}

void CVKPipelineGraphics::Uniform1f(VkCommandBuffer vkCommandBuffer, uint32_t name, float v0) const
{
	m_pPipeline->Uniform1f(vkCommandBuffer, name, v0);
}

void CVKPipelineGraphics::Uniform2f(VkCommandBuffer vkCommandBuffer, uint32_t name, float v0, float v1) const
{
	m_pPipeline->Uniform2f(vkCommandBuffer, name, v0, v1);
}

void CVKPipelineGraphics::Uniform3f(VkCommandBuffer vkCommandBuffer, uint32_t name, float v0, float v1, float v2) const
{
	m_pPipeline->Uniform3f(vkCommandBuffer, name, v0, v1, v2);
}

void CVKPipelineGraphics::Uniform4f(VkCommandBuffer vkCommandBuffer, uint32_t name, float v0, float v1, float v2, float v3) const
{
	m_pPipeline->Uniform4f(vkCommandBuffer, name, v0, v1, v2, v3);
}

void CVKPipelineGraphics::Uniform1iv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const int* value) const
{
	m_pPipeline->Uniform1iv(vkCommandBuffer, name, count, value);
}

void CVKPipelineGraphics::Uniform2iv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const int* value) const
{
	m_pPipeline->Uniform2iv(vkCommandBuffer, name, count, value);
}

void CVKPipelineGraphics::Uniform3iv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const int* value) const
{
	m_pPipeline->Uniform3iv(vkCommandBuffer, name, count, value);
}

void CVKPipelineGraphics::Uniform4iv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const int* value) const
{
	m_pPipeline->Uniform4iv(vkCommandBuffer, name, count, value);
}

void CVKPipelineGraphics::Uniform1fv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const float* value) const
{
	m_pPipeline->Uniform1fv(vkCommandBuffer, name, count, value);
}

void CVKPipelineGraphics::Uniform2fv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const float* value) const
{
	m_pPipeline->Uniform2fv(vkCommandBuffer, name, count, value);
}

void CVKPipelineGraphics::Uniform3fv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const float* value) const
{
	m_pPipeline->Uniform3fv(vkCommandBuffer, name, count, value);
}

void CVKPipelineGraphics::Uniform4fv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const float* value) const
{
	m_pPipeline->Uniform4fv(vkCommandBuffer, name, count, value);
}

void CVKPipelineGraphics::UniformMatrix2fv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const float* value) const
{
	m_pPipeline->UniformMatrix2fv(vkCommandBuffer, name, count, value);
}

void CVKPipelineGraphics::UniformMatrix3fv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const float* value) const
{
	m_pPipeline->UniformMatrix3fv(vkCommandBuffer, name, count, value);
}

void CVKPipelineGraphics::UniformMatrix4fv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const float* value) const
{
	m_pPipeline->UniformMatrix4fv(vkCommandBuffer, name, count, value);
}
