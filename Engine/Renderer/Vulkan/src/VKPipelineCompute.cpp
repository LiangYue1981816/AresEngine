#include "VKRenderer.h"


CVKPipelineCompute::CVKPipelineCompute(CVKDevice* pDevice, uint32_t name)
	: CGfxPipelineCompute(name)
	, m_pDevice(pDevice)
	, m_pPipeline(nullptr)
{
	ASSERT(m_pDevice);

	m_pPipeline = new CVKPipeline(m_pDevice);
}

CVKPipelineCompute::~CVKPipelineCompute(void)
{
	delete m_pPipeline;
}

const VkPipelineLayout CVKPipelineCompute::GetPipelineLayout(void) const
{
	return m_pPipeline->GetPipelineLayout();
}

const CGfxDescriptorLayoutPtr CVKPipelineCompute::GetDescriptorLayout(int indexDescriptorSet) const
{
	return m_pPipeline->GetDescriptorLayout(indexDescriptorSet);
}

bool CVKPipelineCompute::Create(const CGfxShader* pComputeShader)
{
	return m_pPipeline->Create(pComputeShader);
}

void CVKPipelineCompute::Destroy(void)
{
	m_pPipeline->Destroy();
}

void CVKPipelineCompute::Bind(VkCommandBuffer vkCommandBuffer)
{
	m_pPipeline->Bind(vkCommandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE);
}

void CVKPipelineCompute::Uniform1i(VkCommandBuffer vkCommandBuffer, uint32_t name, int v0) const
{
	m_pPipeline->Uniform1i(vkCommandBuffer, name, v0);
}

void CVKPipelineCompute::Uniform2i(VkCommandBuffer vkCommandBuffer, uint32_t name, int v0, int v1) const
{
	m_pPipeline->Uniform2i(vkCommandBuffer, name, v0, v1);
}

void CVKPipelineCompute::Uniform3i(VkCommandBuffer vkCommandBuffer, uint32_t name, int v0, int v1, int v2) const
{
	m_pPipeline->Uniform3i(vkCommandBuffer, name, v0, v1, v2);
}

void CVKPipelineCompute::Uniform4i(VkCommandBuffer vkCommandBuffer, uint32_t name, int v0, int v1, int v2, int v3) const
{
	m_pPipeline->Uniform4i(vkCommandBuffer, name, v0, v1, v2, v3);
}

void CVKPipelineCompute::Uniform1f(VkCommandBuffer vkCommandBuffer, uint32_t name, float v0) const
{
	m_pPipeline->Uniform1f(vkCommandBuffer, name, v0);
}

void CVKPipelineCompute::Uniform2f(VkCommandBuffer vkCommandBuffer, uint32_t name, float v0, float v1) const
{
	m_pPipeline->Uniform2f(vkCommandBuffer, name, v0, v1);
}

void CVKPipelineCompute::Uniform3f(VkCommandBuffer vkCommandBuffer, uint32_t name, float v0, float v1, float v2) const
{
	m_pPipeline->Uniform3f(vkCommandBuffer, name, v0, v1, v2);
}

void CVKPipelineCompute::Uniform4f(VkCommandBuffer vkCommandBuffer, uint32_t name, float v0, float v1, float v2, float v3) const
{
	m_pPipeline->Uniform4f(vkCommandBuffer, name, v0, v1, v2, v3);
}

void CVKPipelineCompute::Uniform1iv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const int* value) const
{
	m_pPipeline->Uniform1iv(vkCommandBuffer, name, count, value);
}

void CVKPipelineCompute::Uniform2iv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const int* value) const
{
	m_pPipeline->Uniform2iv(vkCommandBuffer, name, count, value);
}

void CVKPipelineCompute::Uniform3iv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const int* value) const
{
	m_pPipeline->Uniform3iv(vkCommandBuffer, name, count, value);
}

void CVKPipelineCompute::Uniform4iv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const int* value) const
{
	m_pPipeline->Uniform4iv(vkCommandBuffer, name, count, value);
}

void CVKPipelineCompute::Uniform1fv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const float* value) const
{
	m_pPipeline->Uniform1fv(vkCommandBuffer, name, count, value);
}

void CVKPipelineCompute::Uniform2fv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const float* value) const
{
	m_pPipeline->Uniform2fv(vkCommandBuffer, name, count, value);
}

void CVKPipelineCompute::Uniform3fv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const float* value) const
{
	m_pPipeline->Uniform3fv(vkCommandBuffer, name, count, value);
}

void CVKPipelineCompute::Uniform4fv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const float* value) const
{
	m_pPipeline->Uniform4fv(vkCommandBuffer, name, count, value);
}

void CVKPipelineCompute::UniformMatrix2fv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const float* value) const
{
	m_pPipeline->UniformMatrix2fv(vkCommandBuffer, name, count, value);
}

void CVKPipelineCompute::UniformMatrix3fv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const float* value) const
{
	m_pPipeline->UniformMatrix3fv(vkCommandBuffer, name, count, value);
}

void CVKPipelineCompute::UniformMatrix4fv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const float* value) const
{
	m_pPipeline->UniformMatrix4fv(vkCommandBuffer, name, count, value);
}
