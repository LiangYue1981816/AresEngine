#include "VKRenderer.h"


CVKPipelineGraphics::CVKPipelineGraphics(CVKDevice *pDevice, CVKPipelineGraphicsManager *pManager, uint32_t name)
	: CVKPipeline(pDevice)
	, CGfxPipelineGraphics(name)
	, m_name(name)
	, m_pManager(pManager)
{

}

CVKPipelineGraphics::~CVKPipelineGraphics(void)
{

}

uint32_t CVKPipelineGraphics::GetName(void) const
{
	return m_name;
}

bool CVKPipelineGraphics::Create(const CGfxRenderPass *pRenderPass, const CGfxShader *pVertexShader, const CGfxShader *pFragmentShader, const PipelineState &state, uint32_t indexSubpass, uint32_t vertexBinding, uint32_t instanceBinding)
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

	eastl::vector<VkDescriptorSetLayout> layouts;
	eastl::vector<VkPipelineShaderStageCreateInfo> shaders;
	eastl::vector<VkVertexInputBindingDescription> inputBindingDescriptions;
	eastl::vector<VkVertexInputAttributeDescription> inputAttributeDescriptions;
	CALL_BOOL_FUNCTION_RETURN_BOOL(CreateLayouts(layouts));
	CALL_BOOL_FUNCTION_RETURN_BOOL(CreateShaderStages(shaders));
	CALL_BOOL_FUNCTION_RETURN_BOOL(CreateVertexInputState(inputBindingDescriptions, inputAttributeDescriptions, vertexBinding, instanceBinding));

	VkPipelineVertexInputStateCreateInfo vertexInputeState = {};
	vertexInputeState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputeState.pNext = nullptr;
	vertexInputeState.flags = 0;
	vertexInputeState.vertexBindingDescriptionCount = inputBindingDescriptions.size();
	vertexInputeState.pVertexBindingDescriptions = inputBindingDescriptions.data();
	vertexInputeState.vertexAttributeDescriptionCount = inputAttributeDescriptions.size();
	vertexInputeState.pVertexAttributeDescriptions = inputAttributeDescriptions.data();

	VkPipelineInputAssemblyStateCreateInfo inputAssemblyState = {};
	inputAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssemblyState.pNext = nullptr;
	inputAssemblyState.flags = 0;
	inputAssemblyState.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssemblyState.primitiveRestartEnable = VK_FALSE;

	VkPipelineTessellationStateCreateInfo tessellationState = {};
	tessellationState.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
	tessellationState.pNext = nullptr;
	tessellationState.flags = 0;
	tessellationState.patchControlPoints = 0;

	VkPipelineViewportStateCreateInfo viewportState = {};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.pNext = nullptr;
	viewportState.flags = 0;
	viewportState.viewportCount = 1;
	viewportState.scissorCount = 1;

	VkPipelineRasterizationStateCreateInfo rasterizationState = {};
	rasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizationState.pNext = nullptr;
	rasterizationState.flags = 0;
	rasterizationState.depthClampEnable = VK_FALSE;
	rasterizationState.rasterizerDiscardEnable = VK_FALSE;
	rasterizationState.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizationState.cullMode = CVKHelper::TranslateCullModeFlags(state.bEnableCullFace, state.cullFace);
	rasterizationState.frontFace = CVKHelper::TranslateFrontFace(state.frontFace);
	rasterizationState.depthBiasEnable;
	rasterizationState.depthBiasConstantFactor;
	rasterizationState.depthBiasClamp;
	rasterizationState.depthBiasSlopeFactor;
	rasterizationState.lineWidth = 1.0f;

	VkPipelineMultisampleStateCreateInfo multisampleState = {};
	multisampleState.sType;
	multisampleState.pNext;
	multisampleState.flags;
	multisampleState.rasterizationSamples;
	multisampleState.sampleShadingEnable;
	multisampleState.minSampleShading;
	multisampleState.pSampleMask;
	multisampleState.alphaToCoverageEnable;
	multisampleState.alphaToOneEnable;

	VkPipelineDepthStencilStateCreateInfo depthStencilState = {};
	depthStencilState.sType;
	depthStencilState.pNext;
	depthStencilState.flags;
	depthStencilState.depthTestEnable;
	depthStencilState.depthWriteEnable;
	depthStencilState.depthCompareOp;
	depthStencilState.depthBoundsTestEnable;
	depthStencilState.stencilTestEnable;
	depthStencilState.front;
	depthStencilState.back;
	depthStencilState.minDepthBounds;
	depthStencilState.maxDepthBounds;

	VkPipelineColorBlendStateCreateInfo colorBlendState = {};
	colorBlendState.sType;
	colorBlendState.pNext;
	colorBlendState.flags;
	colorBlendState.logicOpEnable;
	colorBlendState.logicOp;
	colorBlendState.attachmentCount;
	colorBlendState.pAttachments;
	colorBlendState.blendConstants[0];
	colorBlendState.blendConstants[1];
	colorBlendState.blendConstants[2];
	colorBlendState.blendConstants[3];

	VkPipelineDynamicStateCreateInfo dynamicState = {};
	dynamicState.sType;
	dynamicState.pNext;
	dynamicState.flags;
	dynamicState.dynamicStateCount;
	dynamicState.pDynamicStates;

	VkPipelineLayoutCreateInfo layoutCreateInfo = {};
	layoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	layoutCreateInfo.pNext = nullptr;
	layoutCreateInfo.flags = 0;
	layoutCreateInfo.setLayoutCount = layouts.size();
	layoutCreateInfo.pSetLayouts = layouts.data();
	layoutCreateInfo.pushConstantRangeCount = 0;
	layoutCreateInfo.pPushConstantRanges = nullptr;
	CALL_VK_FUNCTION_RETURN_BOOL(vkCreatePipelineLayout(m_pDevice->GetDevice(), &layoutCreateInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkPipelineLayout));

	VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
	pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineCreateInfo.pNext = nullptr;
	pipelineCreateInfo.flags = 0;
	pipelineCreateInfo.stageCount = shaders.size();
	pipelineCreateInfo.pStages = shaders.data();
	pipelineCreateInfo.pVertexInputState = &vertexInputeState;
	pipelineCreateInfo.pInputAssemblyState = &inputAssemblyState;
	pipelineCreateInfo.pTessellationState = &tessellationState;
	pipelineCreateInfo.pViewportState = &viewportState;
	pipelineCreateInfo.pRasterizationState = &rasterizationState;
	pipelineCreateInfo.pMultisampleState = &multisampleState;
	pipelineCreateInfo.pDepthStencilState = &depthStencilState;
	pipelineCreateInfo.pColorBlendState = &colorBlendState;
	pipelineCreateInfo.pDynamicState = &dynamicState;
	pipelineCreateInfo.layout = m_vkPipelineLayout;
	pipelineCreateInfo.renderPass = (VkRenderPass)pRenderPass->GetRenderPass();
	pipelineCreateInfo.subpass = indexSubpass;
	pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
	pipelineCreateInfo.basePipelineIndex = 0;
	CALL_VK_FUNCTION_RETURN_BOOL(vkCreateGraphicsPipelines(m_pDevice->GetDevice(), VK_NULL_HANDLE, 1, &pipelineCreateInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkPipeline));

	return true;
}

void CVKPipelineGraphics::Destroy(void)
{

}

bool CVKPipelineGraphics::IsTextureValid(uint32_t name) const
{
	return true;
}

bool CVKPipelineGraphics::IsUniformValid(uint32_t name) const
{
	return true;
}

bool CVKPipelineGraphics::IsUniformBlockValid(uint32_t name) const
{
	return true;
}
