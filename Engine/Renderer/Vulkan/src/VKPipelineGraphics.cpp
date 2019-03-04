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
	Destroy();
}

uint32_t CVKPipelineGraphics::GetName(void) const
{
	return m_name;
}

HANDLE CVKPipelineGraphics::GetPipeline(void) const
{
	return (HANDLE)m_vkPipeline;
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
	{
		do {
			m_pShaders[vertex_shader] = (CVKShader *)pVertexShader;
			m_pShaders[fragment_shader] = (CVKShader *)pFragmentShader;

			eastl::vector<VkPipelineShaderStageCreateInfo> shaders;
			CALL_BOOL_FUNCTION_BREAK(CreateShaderStages(shaders));

			eastl::vector<VkDescriptorSetLayout> layouts;
			eastl::vector<VkPushConstantRange> pushConstantRanges;
			CALL_BOOL_FUNCTION_BREAK(CreateLayouts(layouts, pushConstantRanges));

			eastl::vector<VkVertexInputBindingDescription> inputBindingDescriptions;
			eastl::vector<VkVertexInputAttributeDescription> inputAttributeDescriptions;
			CALL_BOOL_FUNCTION_BREAK(CreateVertexInputState(inputBindingDescriptions, inputAttributeDescriptions, vertexBinding, instanceBinding));

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
			inputAssemblyState.topology = CVKHelper::TranslatePrimitiveTopology(state.topology);
			inputAssemblyState.primitiveRestartEnable = state.bEnablePrimitiveRestart ? VK_TRUE : VK_FALSE;

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
			rasterizationState.rasterizerDiscardEnable = state.bEnableRasterizerDiscard ? VK_TRUE : VK_FALSE;
			rasterizationState.polygonMode = CVKHelper::TranslatePolytonMode(state.polygonMode);
			rasterizationState.cullMode = CVKHelper::TranslateCullModeFlags(state.bEnableCullFace, state.cullFace);
			rasterizationState.frontFace = CVKHelper::TranslateFrontFace(state.frontFace);
			rasterizationState.depthBiasEnable = state.bEnableDepthBias ? VK_TRUE : VK_FALSE;
			rasterizationState.depthBiasSlopeFactor = state.depthBiasSlopeFactor;
			rasterizationState.depthBiasConstantFactor = state.depthBiasConstantFactor;
			rasterizationState.depthBiasClamp = 0.0f;
			rasterizationState.lineWidth = 1.0f;

			VkPipelineMultisampleStateCreateInfo multisampleState = {};
			multisampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
			multisampleState.pNext = nullptr;
			multisampleState.flags = 0;
			multisampleState.rasterizationSamples = CVKHelper::TranslateSampleCount(state.samples);
			multisampleState.sampleShadingEnable = VK_FALSE;
			multisampleState.minSampleShading = 1.0f;
			multisampleState.pSampleMask = nullptr;
			multisampleState.alphaToCoverageEnable = state.bEnableAlphaToCoverage ? VK_TRUE : VK_FALSE;
			multisampleState.alphaToOneEnable = VK_FALSE;

			VkPipelineDepthStencilStateCreateInfo depthStencilState = {};
			depthStencilState.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
			depthStencilState.pNext = nullptr;
			depthStencilState.flags = 0;
			depthStencilState.depthTestEnable = state.bEnableDepthTest ? VK_TRUE : VK_FALSE;
			depthStencilState.depthWriteEnable = state.bEnableDepthWrite ? VK_TRUE : VK_FALSE;
			depthStencilState.depthCompareOp = CVKHelper::TranslateCompareOp(state.depthCompareOp);
			depthStencilState.depthBoundsTestEnable = VK_FALSE;
			depthStencilState.minDepthBounds = 0.0f;
			depthStencilState.maxDepthBounds = 1.0f;
			depthStencilState.stencilTestEnable = state.bEnableStencilTest ? VK_TRUE : VK_FALSE;
			depthStencilState.front.failOp = CVKHelper::TranslateStencilOp(state.stencilFrontOpSFail);
			depthStencilState.front.passOp = CVKHelper::TranslateStencilOp(state.stencilFrontOpDPass);
			depthStencilState.front.depthFailOp = CVKHelper::TranslateStencilOp(state.stencilFrontOpDFail);
			depthStencilState.front.compareOp = CVKHelper::TranslateCompareOp(state.stencilFrontCompareOp);
			depthStencilState.front.compareMask = state.stencilFrontCompareMask;
			depthStencilState.front.writeMask = state.stencilFrontWriteMask;
			depthStencilState.front.reference = state.stencilFrontCompareRef;
			depthStencilState.back.failOp = CVKHelper::TranslateStencilOp(state.stencilBackOpSFail);
			depthStencilState.back.passOp = CVKHelper::TranslateStencilOp(state.stencilBackOpDPass);
			depthStencilState.back.depthFailOp = CVKHelper::TranslateStencilOp(state.stencilBackOpDFail);
			depthStencilState.back.compareOp = CVKHelper::TranslateCompareOp(state.stencilBackCompareOp);
			depthStencilState.back.compareMask = state.stencilBackCompareMask;
			depthStencilState.back.writeMask = state.stencilBackWriteMask;
			depthStencilState.back.reference = state.stencilBackCompareRef;

			eastl::vector<VkPipelineColorBlendAttachmentState> colorBlendAttachments;
			for (uint32_t indexAttachment = 0; indexAttachment < pRenderPass->GetSubpassOutputAttachmentCount(indexSubpass); indexAttachment++) {
				VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
				colorBlendAttachment.blendEnable = state.bEnableBlend ? VK_TRUE : VK_FALSE;
				colorBlendAttachment.srcColorBlendFactor = CVKHelper::TranslateBlendFactor(state.blendSrcRGB);
				colorBlendAttachment.dstColorBlendFactor = CVKHelper::TranslateBlendFactor(state.blendDstRGB);
				colorBlendAttachment.colorBlendOp = CVKHelper::TranslateBlendOp(state.blendOpRGB);
				colorBlendAttachment.srcAlphaBlendFactor = CVKHelper::TranslateBlendFactor(state.blendSrcAlpha);
				colorBlendAttachment.dstAlphaBlendFactor = CVKHelper::TranslateBlendFactor(state.blendDstAlpha);
				colorBlendAttachment.alphaBlendOp = CVKHelper::TranslateBlendOp(state.blendOpAlpha);
				colorBlendAttachment.colorWriteMask = (state.bEnableColorRedWrite ? VK_COLOR_COMPONENT_R_BIT : 0) | (state.bEnableColorGreenWrite ? VK_COLOR_COMPONENT_G_BIT : 0) | (state.bEnableColorBlueWrite ? VK_COLOR_COMPONENT_B_BIT : 0) | (state.bEnableColorAlphaWrite ? VK_COLOR_COMPONENT_A_BIT : 0);
				colorBlendAttachments.emplace_back(colorBlendAttachment);
			}

			VkPipelineColorBlendStateCreateInfo colorBlendState = {};
			colorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
			colorBlendState.pNext = nullptr;
			colorBlendState.flags = 0;
			colorBlendState.logicOpEnable = VK_FALSE;
			colorBlendState.logicOp = VK_LOGIC_OP_COPY;
			colorBlendState.attachmentCount = colorBlendAttachments.size();
			colorBlendState.pAttachments = colorBlendAttachments.data();
			colorBlendState.blendConstants[0] = state.blendColorRed;
			colorBlendState.blendConstants[1] = state.blendColorGreen;
			colorBlendState.blendConstants[2] = state.blendColorBlue;
			colorBlendState.blendConstants[3] = state.blendColorAlpha;

			eastl::vector<VkDynamicState> dynamicStates;
			dynamicStates.emplace_back(VK_DYNAMIC_STATE_VIEWPORT);
			dynamicStates.emplace_back(VK_DYNAMIC_STATE_SCISSOR);

			VkPipelineDynamicStateCreateInfo dynamicState = {};
			dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
			dynamicState.pNext = nullptr;
			dynamicState.flags = 0;
			dynamicState.dynamicStateCount = dynamicStates.size();
			dynamicState.pDynamicStates = dynamicStates.data();

			VkPipelineLayoutCreateInfo layoutCreateInfo = {};
			layoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
			layoutCreateInfo.pNext = nullptr;
			layoutCreateInfo.flags = 0;
			layoutCreateInfo.setLayoutCount = layouts.size();
			layoutCreateInfo.pSetLayouts = layouts.data();
			layoutCreateInfo.pushConstantRangeCount = pushConstantRanges.size();
			layoutCreateInfo.pPushConstantRanges = pushConstantRanges.data();
			CALL_VK_FUNCTION_BREAK(vkCreatePipelineLayout(m_pDevice->GetDevice(), &layoutCreateInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkPipelineLayout));

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
			CALL_VK_FUNCTION_BREAK(vkCreateGraphicsPipelines(m_pDevice->GetDevice(), VK_NULL_HANDLE, 1, &pipelineCreateInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkPipeline));

			return true;
		} while (false);
	}
	Destroy();
	return false;
}

void CVKPipelineGraphics::Destroy(void)
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

bool CVKPipelineGraphics::IsTextureValid(uint32_t name) const
{
	return CVKPipeline::IsTextureValid(name);
}

bool CVKPipelineGraphics::IsUniformValid(uint32_t name) const
{
	return CVKPipeline::IsUniformValid(name);
}

bool CVKPipelineGraphics::IsUniformBlockValid(uint32_t name) const
{
	return CVKPipeline::IsUniformBlockValid(name);
}

uint32_t CVKPipelineGraphics::GetTextureBinding(uint32_t set, uint32_t name) const
{
	if (set >= 0 && set < DESCRIPTOR_SET_COUNT) {
		return m_pDescriptorLayouts[set]->GetTextureBinding(name).binding;
	}
	else {
		return 0xffffffff;
	}
}

uint32_t CVKPipelineGraphics::GetUniformBlockBinding(uint32_t set, uint32_t name) const
{
	if (set >= 0 && set < DESCRIPTOR_SET_COUNT) {
		return m_pDescriptorLayouts[set]->GetUniformBlockBinding(name).binding;
	}
	else {
		return 0xffffffff;
	}
}
