#include "VKRenderer.h"


CVKPipelineCompute::CVKPipelineCompute(CVKDevice *pDevice, CVKPipelineComputeManager *pManager, uint32_t name)
	: CVKPipeline(pDevice)
	, CGfxPipelineCompute(name)
	, m_name(name)
	, m_pManager(pManager)
{

}

CVKPipelineCompute::~CVKPipelineCompute(void)
{
	Destroy();
}

uint32_t CVKPipelineCompute::GetName(void) const
{
	return m_name;
}

HANDLE CVKPipelineCompute::GetPipeline(void) const
{
	return (HANDLE)m_vkPipeline;
}

bool CVKPipelineCompute::Create(const CGfxShader *pComputeShader)
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

	m_pShaders[compute_shader] = (CVKShader *)pComputeShader;

	eastl::vector<VkDescriptorSetLayout> layouts;
	eastl::vector<VkPushConstantRange> pushConstantRanges;
	CALL_BOOL_FUNCTION_RETURN_BOOL(CreateLayouts(layouts, pushConstantRanges));

	VkPipelineShaderStageCreateInfo shaderStageCreateInfo = {};
	shaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shaderStageCreateInfo.pNext = nullptr;
	shaderStageCreateInfo.flags = 0;
	shaderStageCreateInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
	shaderStageCreateInfo.module = (VkShaderModule)m_pShaders[compute_shader]->GetShader();
	shaderStageCreateInfo.pName = "main";
	shaderStageCreateInfo.pSpecializationInfo = nullptr;

	VkComputePipelineCreateInfo pipelineCreateInfo = {};
	pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
	pipelineCreateInfo.pNext = nullptr;
	pipelineCreateInfo.flags = 0;
	pipelineCreateInfo.stage = shaderStageCreateInfo;
	pipelineCreateInfo.layout = m_vkPipelineLayout;
	pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
	pipelineCreateInfo.basePipelineIndex = 0;
	CALL_VK_FUNCTION_RETURN_BOOL(vkCreateComputePipelines(m_pDevice->GetDevice(), VK_NULL_HANDLE, 1, &pipelineCreateInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkPipeline));

	return true;
}

void CVKPipelineCompute::Destroy(void)
{
	if (m_vkPipeline) {
		vkDestroyPipeline(m_pDevice->GetDevice(), m_vkPipeline, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
	}

	if (m_vkPipelineLayout) {
		vkDestroyPipelineLayout(m_pDevice->GetDevice(), m_vkPipelineLayout, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
	}

	m_vkPipeline = VK_NULL_HANDLE;
	m_vkPipelineLayout = VK_NULL_HANDLE;
}

bool CVKPipelineCompute::IsTextureValid(uint32_t name) const
{
	return CVKPipeline::IsTextureValid(name);
}

bool CVKPipelineCompute::IsUniformValid(uint32_t name) const
{
	return CVKPipeline::IsUniformValid(name);
}

bool CVKPipelineCompute::IsUniformBlockValid(uint32_t name) const
{
	return CVKPipeline::IsUniformBlockValid(name);
}

uint32_t CVKPipelineCompute::GetTextureBinding(uint32_t set, uint32_t name) const
{
	if (set >= 0 && set < DESCRIPTOR_SET_COUNT) {
		return m_pDescriptorSetLayouts[set]->GetTextureBinding(name).binding;
	}
	else {
		return 0xffffffff;
	}
}

uint32_t CVKPipelineCompute::GetUniformBlockBinding(uint32_t set, uint32_t name) const
{
	if (set >= 0 && set < DESCRIPTOR_SET_COUNT) {
		return m_pDescriptorSetLayouts[set]->GetUniformBlockBinding(name).binding;
	}
	else {
		return 0xffffffff;
	}
}
