#include "VKRenderer.h"


CVKPipeline::CVKPipeline(CVKDevice* pDevice)
	: m_pDevice(pDevice)

	, m_pShaders{ nullptr }
	, m_vkPipeline(VK_NULL_HANDLE)
	, m_vkPipelineLayout(VK_NULL_HANDLE)
{
	m_ptrDescriptorLayouts[DESCRIPTOR_SET_ENGINE] = VKRenderer()->NewDescriptorLayout(DESCRIPTOR_SET_ENGINE);
	m_ptrDescriptorLayouts[DESCRIPTOR_SET_CAMERA] = VKRenderer()->NewDescriptorLayout(DESCRIPTOR_SET_CAMERA);
	m_ptrDescriptorLayouts[DESCRIPTOR_SET_PASS] = VKRenderer()->NewDescriptorLayout(DESCRIPTOR_SET_PASS);
	m_ptrDescriptorLayouts[DESCRIPTOR_SET_INPUTATTACHMENT] = VKRenderer()->NewDescriptorLayout(DESCRIPTOR_SET_INPUTATTACHMENT);
}

CVKPipeline::~CVKPipeline(void)
{
	Destroy();
}

const VkPipelineLayout CVKPipeline::GetPipelineLayout(void) const
{
	ASSERT(m_vkPipelineLayout);
	return m_vkPipelineLayout;
}

const CGfxDescriptorLayoutPtr CVKPipeline::GetDescriptorLayout(int indexDescriptorSet) const
{
	if (indexDescriptorSet >= 0 && indexDescriptorSet < DESCRIPTOR_SET_COUNT) {
		return m_ptrDescriptorLayouts[indexDescriptorSet];
	}
	else {
		return nullptr;
	}
}

bool CVKPipeline::CreateLayouts(void)
{
	for (int indexShader = 0; indexShader < compute_shader - vertex_shader + 1; indexShader++) {
		if (m_pShaders[indexShader] && m_pShaders[indexShader]->IsValid()) {
			const eastl::unordered_map<eastl::string, PushConstantRange>& pushConstantRanges = m_pShaders[indexShader]->GetSprivCross().GetPushConstantRanges();
			const eastl::unordered_map<eastl::string, DescriptorSetBinding>& uniformBlockBindings = m_pShaders[indexShader]->GetSprivCross().GetUniformBlockBindings();
			const eastl::unordered_map<eastl::string, DescriptorSetBinding>& sampledImageBindings = m_pShaders[indexShader]->GetSprivCross().GetSampledImageBindings();
			const eastl::unordered_map<eastl::string, InputAttachmentBinding>& inputAttachmentBindings = m_pShaders[indexShader]->GetSprivCross().GetInputAttachmentBindings();

			for (const auto& itPushConstant : pushConstantRanges) {
				uint32_t name = HashValue(itPushConstant.first.c_str());
				m_pushConstantRanges[name].stageFlags = vkGetShaderStageFlagBits((shader_kind)indexShader);
				m_pushConstantRanges[name].offset = itPushConstant.second.offset;
				m_pushConstantRanges[name].size = itPushConstant.second.range;
			}

			for (const auto& itUniformBlock : uniformBlockBindings) {
				m_ptrDescriptorLayouts[itUniformBlock.second.set]->SetUniformBlockBinding(HashValue(itUniformBlock.first.c_str()), itUniformBlock.second.binding);
			}

			for (const auto& itSampledImage : sampledImageBindings) {
				m_ptrDescriptorLayouts[itSampledImage.second.set]->SetSampledImageBinding(HashValue(itSampledImage.first.c_str()), itSampledImage.second.binding);
			}

			for (const auto& itInputAttachment : inputAttachmentBindings) {
				m_inputAttachmentNames[itInputAttachment.second.inputAttachmentIndex] = HashValue(itInputAttachment.first.c_str());
				m_ptrDescriptorLayouts[itInputAttachment.second.set]->SetInputAttachmentBinding(HashValue(itInputAttachment.first.c_str()), itInputAttachment.second.binding);
			}
		}
	}

	CALL_BOOL_FUNCTION_RETURN_BOOL(m_ptrDescriptorLayouts[DESCRIPTOR_SET_ENGINE]->Create());
	CALL_BOOL_FUNCTION_RETURN_BOOL(m_ptrDescriptorLayouts[DESCRIPTOR_SET_CAMERA]->Create());
	CALL_BOOL_FUNCTION_RETURN_BOOL(m_ptrDescriptorLayouts[DESCRIPTOR_SET_PASS]->Create());
	CALL_BOOL_FUNCTION_RETURN_BOOL(m_ptrDescriptorLayouts[DESCRIPTOR_SET_INPUTATTACHMENT]->Create());

	eastl::vector<VkDescriptorSetLayout> layouts;
	eastl::vector<VkPushConstantRange> pushConstantRanges;

	for (const auto& itDescriptorSet : m_ptrDescriptorLayouts) {
		layouts.emplace_back(((CVKDescriptorLayout*)itDescriptorSet.GetPointer())->GetDescriptorLayout());
	}

	for (const auto& itPushConstantRange : m_pushConstantRanges) {
		pushConstantRanges.emplace_back(itPushConstantRange.second);
	}

	VkPipelineLayoutCreateInfo layoutCreateInfo = {};
	layoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	layoutCreateInfo.pNext = nullptr;
	layoutCreateInfo.flags = 0;
	layoutCreateInfo.setLayoutCount = layouts.size();
	layoutCreateInfo.pSetLayouts = layouts.data();
	layoutCreateInfo.pushConstantRangeCount = pushConstantRanges.size();
	layoutCreateInfo.pPushConstantRanges = pushConstantRanges.data();
	CALL_VK_FUNCTION_RETURN_BOOL(vkCreatePipelineLayout(m_pDevice->GetDevice(), &layoutCreateInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkPipelineLayout));

	return true;
}

bool CVKPipeline::CreateShaderStages(eastl::vector<VkPipelineShaderStageCreateInfo>& shaders)
{
	for (int indexShader = 0; indexShader < compute_shader - vertex_shader + 1; indexShader++) {
		if (m_pShaders[indexShader] && m_pShaders[indexShader]->IsValid()) {
			VkPipelineShaderStageCreateInfo shaderStageCreateInfo = {};
			shaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			shaderStageCreateInfo.pNext = nullptr;
			shaderStageCreateInfo.flags = 0;
			shaderStageCreateInfo.stage = vkGetShaderStageFlagBits((shader_kind)indexShader);
			shaderStageCreateInfo.module = m_pShaders[indexShader]->GetShader();
			shaderStageCreateInfo.pName = "main";
			shaderStageCreateInfo.pSpecializationInfo = nullptr;
			shaders.emplace_back(shaderStageCreateInfo);
		}
	}

	return true;
}

bool CVKPipeline::CreateVertexInputState(eastl::vector<VkVertexInputBindingDescription>& inputBindingDescriptions, eastl::vector<VkVertexInputAttributeDescription>& inputAttributeDescriptions, int vertexBinding, int instanceBinding)
{
	ASSERT(m_pShaders[vertex_shader]);
	ASSERT(m_pShaders[vertex_shader]->IsValid());

	const eastl::vector<eastl::string>& vertexAttributes = m_pShaders[vertex_shader]->GetSprivCross().GetVertexAttributes();

	uint32_t vertexFormat = 0;
	uint32_t instanceFormat = 0;

	for (const auto& itVertexAttribute : vertexAttributes) {
		vertexFormat |= GetVertexAttribute(itVertexAttribute.c_str());
		instanceFormat |= GetInstanceAttribute(itVertexAttribute.c_str());
	}

	if (vertexFormat) {
		m_vertexFormats[vertexBinding] = vertexFormat;

		for (const auto& itVertexAttribute : vertexAttributes) {
			if (uint32_t attribute = GetVertexAttribute(itVertexAttribute.c_str())) {
				VkVertexInputAttributeDescription inputAttributeDescription = {};
				inputAttributeDescription.binding = vertexBinding;
				inputAttributeDescription.location = GetVertexAttributeLocation(attribute);
				inputAttributeDescription.format = (VkFormat)GetVertexAttributeFormat(attribute);
				inputAttributeDescription.offset = GetVertexAttributeOffset(vertexFormat, attribute);
				inputAttributeDescriptions.emplace_back(inputAttributeDescription);
			}
		}

		VkVertexInputBindingDescription inputBindingDescription;
		inputBindingDescription.binding = vertexBinding;
		inputBindingDescription.stride = GetVertexStride(vertexFormat);
		inputBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		inputBindingDescriptions.emplace_back(inputBindingDescription);
	}

	if (instanceFormat) {
		m_vertexFormats[instanceBinding] = instanceFormat;

		for (const auto& itVertexAttribute : vertexAttributes) {
			if (uint32_t attribute = GetInstanceAttribute(itVertexAttribute.c_str())) {
				VkVertexInputAttributeDescription inputAttributeDescription = {};
				inputAttributeDescription.binding = instanceBinding;
				inputAttributeDescription.location = GetInstanceAttributeLocation(attribute);
				inputAttributeDescription.format = (VkFormat)GetInstanceAttributeFormat(attribute);
				inputAttributeDescription.offset = GetInstanceAttributeOffset(instanceFormat, attribute);
				inputAttributeDescriptions.emplace_back(inputAttributeDescription);
			}
		}

		VkVertexInputBindingDescription inputBindingDescription;
		inputBindingDescription.binding = instanceBinding;
		inputBindingDescription.stride = GetInstanceStride(instanceFormat);
		inputBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_INSTANCE;
		inputBindingDescriptions.emplace_back(inputBindingDescription);
	}

	return true;
}

bool CVKPipeline::Create(const CGfxShader* pComputeShader)
{
	Destroy();
	{
		do {
			ASSERT(pComputeShader);
			ASSERT(pComputeShader->IsValid());
			ASSERT(pComputeShader->GetKind() == compute_shader);

			m_pShaders[compute_shader] = (CVKShader*)pComputeShader;

			CALL_BOOL_FUNCTION_BREAK(CreateLayouts());

			VkPipelineShaderStageCreateInfo shaderStageCreateInfo = {};
			shaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			shaderStageCreateInfo.pNext = nullptr;
			shaderStageCreateInfo.flags = 0;
			shaderStageCreateInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
			shaderStageCreateInfo.module = m_pShaders[compute_shader]->GetShader();
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
			CALL_VK_FUNCTION_BREAK(vkCreateComputePipelines(m_pDevice->GetDevice(), VK_NULL_HANDLE, 1, &pipelineCreateInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkPipeline));

			return true;
		} while (false);
	}
	Destroy();
	return false;
}

bool CVKPipeline::Create(const CGfxRenderPass* pRenderPass, const CGfxShader* pVertexShader, const CGfxShader* pFragmentShader, const PipelineState& state, int indexSubpass, int vertexBinding, int instanceBinding)
{
	Destroy();
	{
		do {
			ASSERT(pRenderPass);
			ASSERT(pVertexShader);
			ASSERT(pVertexShader->IsValid());
			ASSERT(pVertexShader->GetKind() == vertex_shader);
			ASSERT(pFragmentShader);
			ASSERT(pFragmentShader->IsValid());
			ASSERT(pFragmentShader->GetKind() == fragment_shader);

			m_pShaders[vertex_shader] = (CVKShader*)pVertexShader;
			m_pShaders[fragment_shader] = (CVKShader*)pFragmentShader;

			eastl::vector<VkPipelineShaderStageCreateInfo> shaders;
			eastl::vector<VkVertexInputBindingDescription> inputBindingDescriptions;
			eastl::vector<VkVertexInputAttributeDescription> inputAttributeDescriptions;

			CALL_BOOL_FUNCTION_BREAK(CreateLayouts());
			CALL_BOOL_FUNCTION_BREAK(CreateShaderStages(shaders));
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
			for (int indexAttachment = 0; indexAttachment < (int)pRenderPass->GetSubpassOutputAttachmentCount(indexSubpass); indexAttachment++) {
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
			pipelineCreateInfo.renderPass = ((CVKRenderPass*)pRenderPass)->GetRenderPass();
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

void CVKPipeline::Destroy(void)
{
	if (m_vkPipeline) {
		vkDestroyPipeline(m_pDevice->GetDevice(), m_vkPipeline, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
	}

	if (m_vkPipelineLayout) {
		vkDestroyPipelineLayout(m_pDevice->GetDevice(), m_vkPipelineLayout, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
	}

	m_vkPipeline = VK_NULL_HANDLE;
	m_vkPipelineLayout = VK_NULL_HANDLE;

	m_pShaders[vertex_shader] = nullptr;
	m_pShaders[fragment_shader] = nullptr;
	m_pShaders[compute_shader] = nullptr;

	m_pushConstantRanges.clear();
	m_inputAttachmentNames.clear();
	m_vertexFormats.clear();

	m_ptrDescriptorLayouts[DESCRIPTOR_SET_ENGINE]->Destroy(true);
	m_ptrDescriptorLayouts[DESCRIPTOR_SET_CAMERA]->Destroy(true);
	m_ptrDescriptorLayouts[DESCRIPTOR_SET_PASS]->Destroy(true);
	m_ptrDescriptorLayouts[DESCRIPTOR_SET_INPUTATTACHMENT]->Destroy(true);
}

uint32_t CVKPipeline::GetInputAttachmentName(uint32_t inputAttachmentIndex) const
{
	const auto& itInputAttachmentName = m_inputAttachmentNames.find(inputAttachmentIndex);

	if (itInputAttachmentName != m_inputAttachmentNames.end()) {
		return itInputAttachmentName->second;
	}
	else {
		return INVALID_HASHNAME;
	}
}

bool CVKPipeline::IsCompatibleVertexFormat(uint32_t binding, uint32_t format) const
{
	const auto& itFormat = m_vertexFormats.find(binding);

	if (itFormat != m_vertexFormats.end()) {
		return itFormat->second == format && itFormat->second != 0;
	}
	else {
		return false;
	}
}

void CVKPipeline::Bind(VkCommandBuffer vkCommandBuffer, VkPipelineBindPoint pipelineBindPoint)
{
	ASSERT(vkCommandBuffer);
	ASSERT(m_vkPipeline);
	ASSERT(m_vkPipelineLayout);

	vkCmdBindPipeline(vkCommandBuffer, pipelineBindPoint, m_vkPipeline);
}

void CVKPipeline::Uniform1i(VkCommandBuffer vkCommandBuffer, uint32_t name, int v0) const
{
	ASSERT(vkCommandBuffer);
	ASSERT(m_vkPipeline);
	ASSERT(m_vkPipelineLayout);

	const auto& itPushConstant = m_pushConstantRanges.find(name);

	if (itPushConstant != m_pushConstantRanges.end()) {
		glm::ivec4 vec(v0, 0, 0, 0);

		ASSERT(itPushConstant->second.size == sizeof(vec));
		vkCmdPushConstants(vkCommandBuffer, m_vkPipelineLayout, itPushConstant->second.stageFlags, itPushConstant->second.offset, itPushConstant->second.size, &vec);
	}
}

void CVKPipeline::Uniform2i(VkCommandBuffer vkCommandBuffer, uint32_t name, int v0, int v1) const
{
	ASSERT(vkCommandBuffer);
	ASSERT(m_vkPipeline);
	ASSERT(m_vkPipelineLayout);

	const auto& itPushConstant = m_pushConstantRanges.find(name);

	if (itPushConstant != m_pushConstantRanges.end()) {
		glm::ivec4 vec(v0, v1, 0, 0);

		ASSERT(itPushConstant->second.size == sizeof(vec));
		vkCmdPushConstants(vkCommandBuffer, m_vkPipelineLayout, itPushConstant->second.stageFlags, itPushConstant->second.offset, itPushConstant->second.size, &vec);
	}
}

void CVKPipeline::Uniform3i(VkCommandBuffer vkCommandBuffer, uint32_t name, int v0, int v1, int v2) const
{
	ASSERT(vkCommandBuffer);
	ASSERT(m_vkPipeline);
	ASSERT(m_vkPipelineLayout);

	const auto& itPushConstant = m_pushConstantRanges.find(name);

	if (itPushConstant != m_pushConstantRanges.end()) {
		glm::ivec4 vec(v0, v1, v2, 0);

		ASSERT(itPushConstant->second.size == sizeof(vec));
		vkCmdPushConstants(vkCommandBuffer, m_vkPipelineLayout, itPushConstant->second.stageFlags, itPushConstant->second.offset, itPushConstant->second.size, &vec);
	}
}

void CVKPipeline::Uniform4i(VkCommandBuffer vkCommandBuffer, uint32_t name, int v0, int v1, int v2, int v3) const
{
	ASSERT(vkCommandBuffer);
	ASSERT(m_vkPipeline);
	ASSERT(m_vkPipelineLayout);

	const auto& itPushConstant = m_pushConstantRanges.find(name);

	if (itPushConstant != m_pushConstantRanges.end()) {
		glm::ivec4 vec(v0, v1, v2, v3);

		ASSERT(itPushConstant->second.size == sizeof(vec));
		vkCmdPushConstants(vkCommandBuffer, m_vkPipelineLayout, itPushConstant->second.stageFlags, itPushConstant->second.offset, itPushConstant->second.size, &vec);
	}
}

void CVKPipeline::Uniform1f(VkCommandBuffer vkCommandBuffer, uint32_t name, float v0) const
{
	ASSERT(vkCommandBuffer);
	ASSERT(m_vkPipeline);
	ASSERT(m_vkPipelineLayout);

	const auto& itPushConstant = m_pushConstantRanges.find(name);

	if (itPushConstant != m_pushConstantRanges.end()) {
		glm::vec4 vec(v0, 0.0f, 0.0f, 0.0f);

		ASSERT(itPushConstant->second.size == sizeof(vec));
		vkCmdPushConstants(vkCommandBuffer, m_vkPipelineLayout, itPushConstant->second.stageFlags, itPushConstant->second.offset, itPushConstant->second.size, &vec);
	}
}

void CVKPipeline::Uniform2f(VkCommandBuffer vkCommandBuffer, uint32_t name, float v0, float v1) const
{
	ASSERT(vkCommandBuffer);
	ASSERT(m_vkPipeline);
	ASSERT(m_vkPipelineLayout);

	const auto& itPushConstant = m_pushConstantRanges.find(name);

	if (itPushConstant != m_pushConstantRanges.end()) {
		glm::vec4 vec(v0, v1, 0.0f, 0.0f);

		ASSERT(itPushConstant->second.size == sizeof(vec));
		vkCmdPushConstants(vkCommandBuffer, m_vkPipelineLayout, itPushConstant->second.stageFlags, itPushConstant->second.offset, itPushConstant->second.size, &vec);
	}
}

void CVKPipeline::Uniform3f(VkCommandBuffer vkCommandBuffer, uint32_t name, float v0, float v1, float v2) const
{
	ASSERT(vkCommandBuffer);
	ASSERT(m_vkPipeline);
	ASSERT(m_vkPipelineLayout);

	const auto& itPushConstant = m_pushConstantRanges.find(name);

	if (itPushConstant != m_pushConstantRanges.end()) {
		glm::vec4 vec(v0, v1, v2, 0.0f);

		ASSERT(itPushConstant->second.size == sizeof(vec));
		vkCmdPushConstants(vkCommandBuffer, m_vkPipelineLayout, itPushConstant->second.stageFlags, itPushConstant->second.offset, itPushConstant->second.size, &vec);
	}
}

void CVKPipeline::Uniform4f(VkCommandBuffer vkCommandBuffer, uint32_t name, float v0, float v1, float v2, float v3) const
{
	ASSERT(vkCommandBuffer);
	ASSERT(m_vkPipeline);
	ASSERT(m_vkPipelineLayout);

	const auto& itPushConstant = m_pushConstantRanges.find(name);

	if (itPushConstant != m_pushConstantRanges.end()) {
		glm::vec4 vec(v0, v1, v2, v3);

		ASSERT(itPushConstant->second.size == sizeof(vec));
		vkCmdPushConstants(vkCommandBuffer, m_vkPipelineLayout, itPushConstant->second.stageFlags, itPushConstant->second.offset, itPushConstant->second.size, &vec);
	}
}

void CVKPipeline::Uniform1iv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const int* value) const
{
	ASSERT(value);
	ASSERT(vkCommandBuffer);
	ASSERT(m_vkPipeline);
	ASSERT(m_vkPipelineLayout);

	const auto& itPushConstant = m_pushConstantRanges.find(name);

	if (itPushConstant != m_pushConstantRanges.end()) {
		ASSERT(itPushConstant->second.size == sizeof(*value) * count);
		vkCmdPushConstants(vkCommandBuffer, m_vkPipelineLayout, itPushConstant->second.stageFlags, itPushConstant->second.offset, itPushConstant->second.size, value);
	}
}

void CVKPipeline::Uniform2iv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const int* value) const
{
	ASSERT(value);
	ASSERT(vkCommandBuffer);
	ASSERT(m_vkPipeline);
	ASSERT(m_vkPipelineLayout);

	const auto& itPushConstant = m_pushConstantRanges.find(name);

	if (itPushConstant != m_pushConstantRanges.end()) {
		ASSERT(itPushConstant->second.size == sizeof(*value) * count);
		vkCmdPushConstants(vkCommandBuffer, m_vkPipelineLayout, itPushConstant->second.stageFlags, itPushConstant->second.offset, itPushConstant->second.size, value);
	}
}

void CVKPipeline::Uniform3iv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const int* value) const
{
	ASSERT(value);
	ASSERT(vkCommandBuffer);
	ASSERT(m_vkPipeline);
	ASSERT(m_vkPipelineLayout);

	const auto& itPushConstant = m_pushConstantRanges.find(name);

	if (itPushConstant != m_pushConstantRanges.end()) {
		ASSERT(itPushConstant->second.size == sizeof(*value) * count);
		vkCmdPushConstants(vkCommandBuffer, m_vkPipelineLayout, itPushConstant->second.stageFlags, itPushConstant->second.offset, itPushConstant->second.size, value);
	}
}

void CVKPipeline::Uniform4iv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const int* value) const
{
	ASSERT(value);
	ASSERT(vkCommandBuffer);
	ASSERT(m_vkPipeline);
	ASSERT(m_vkPipelineLayout);

	const auto& itPushConstant = m_pushConstantRanges.find(name);

	if (itPushConstant != m_pushConstantRanges.end()) {
		ASSERT(itPushConstant->second.size == sizeof(*value) * count);
		vkCmdPushConstants(vkCommandBuffer, m_vkPipelineLayout, itPushConstant->second.stageFlags, itPushConstant->second.offset, itPushConstant->second.size, value);
	}
}

void CVKPipeline::Uniform1fv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const float* value) const
{
	ASSERT(value);
	ASSERT(vkCommandBuffer);
	ASSERT(m_vkPipeline);
	ASSERT(m_vkPipelineLayout);

	const auto& itPushConstant = m_pushConstantRanges.find(name);

	if (itPushConstant != m_pushConstantRanges.end()) {
		ASSERT(itPushConstant->second.size == sizeof(*value) * count);
		vkCmdPushConstants(vkCommandBuffer, m_vkPipelineLayout, itPushConstant->second.stageFlags, itPushConstant->second.offset, itPushConstant->second.size, value);
	}
}

void CVKPipeline::Uniform2fv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const float* value) const
{
	ASSERT(value);
	ASSERT(vkCommandBuffer);
	ASSERT(m_vkPipeline);
	ASSERT(m_vkPipelineLayout);

	const auto& itPushConstant = m_pushConstantRanges.find(name);

	if (itPushConstant != m_pushConstantRanges.end()) {
		ASSERT(itPushConstant->second.size == sizeof(*value) * count);
		vkCmdPushConstants(vkCommandBuffer, m_vkPipelineLayout, itPushConstant->second.stageFlags, itPushConstant->second.offset, itPushConstant->second.size, value);
	}
}

void CVKPipeline::Uniform3fv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const float* value) const
{
	ASSERT(value);
	ASSERT(vkCommandBuffer);
	ASSERT(m_vkPipeline);
	ASSERT(m_vkPipelineLayout);

	const auto& itPushConstant = m_pushConstantRanges.find(name);

	if (itPushConstant != m_pushConstantRanges.end()) {
		ASSERT(itPushConstant->second.size == sizeof(*value) * count);
		vkCmdPushConstants(vkCommandBuffer, m_vkPipelineLayout, itPushConstant->second.stageFlags, itPushConstant->second.offset, itPushConstant->second.size, value);
	}
}

void CVKPipeline::Uniform4fv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const float* value) const
{
	ASSERT(value);
	ASSERT(vkCommandBuffer);
	ASSERT(m_vkPipeline);
	ASSERT(m_vkPipelineLayout);

	const auto& itPushConstant = m_pushConstantRanges.find(name);

	if (itPushConstant != m_pushConstantRanges.end()) {
		ASSERT(itPushConstant->second.size == sizeof(*value) * count);
		vkCmdPushConstants(vkCommandBuffer, m_vkPipelineLayout, itPushConstant->second.stageFlags, itPushConstant->second.offset, itPushConstant->second.size, value);
	}
}

void CVKPipeline::UniformMatrix2fv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const float* value) const
{
	ASSERT(value);
	ASSERT(vkCommandBuffer);
	ASSERT(m_vkPipeline);
	ASSERT(m_vkPipelineLayout);

	const auto& itPushConstant = m_pushConstantRanges.find(name);

	if (itPushConstant != m_pushConstantRanges.end()) {
		ASSERT(itPushConstant->second.size == sizeof(*value) * count);
		vkCmdPushConstants(vkCommandBuffer, m_vkPipelineLayout, itPushConstant->second.stageFlags, itPushConstant->second.offset, itPushConstant->second.size, value);
	}
}

void CVKPipeline::UniformMatrix3fv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const float* value) const
{
	ASSERT(value);
	ASSERT(vkCommandBuffer);
	ASSERT(m_vkPipeline);
	ASSERT(m_vkPipelineLayout);

	const auto& itPushConstant = m_pushConstantRanges.find(name);

	if (itPushConstant != m_pushConstantRanges.end()) {
		ASSERT(itPushConstant->second.size == sizeof(*value) * count);
		vkCmdPushConstants(vkCommandBuffer, m_vkPipelineLayout, itPushConstant->second.stageFlags, itPushConstant->second.offset, itPushConstant->second.size, value);
	}
}

void CVKPipeline::UniformMatrix4fv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const float* value) const
{
	ASSERT(value);
	ASSERT(vkCommandBuffer);
	ASSERT(m_vkPipeline);
	ASSERT(m_vkPipelineLayout);

	const auto& itPushConstant = m_pushConstantRanges.find(name);

	if (itPushConstant != m_pushConstantRanges.end()) {
		ASSERT(itPushConstant->second.size == sizeof(*value) * count);
		vkCmdPushConstants(vkCommandBuffer, m_vkPipelineLayout, itPushConstant->second.stageFlags, itPushConstant->second.offset, itPushConstant->second.size, value);
	}
}
