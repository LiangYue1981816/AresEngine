#include "VKRenderer.h"


CVKPipeline::CVKPipeline(CVKDevice *pDevice)
	: m_pDevice(pDevice)

	, m_vkPipeline(VK_NULL_HANDLE)
	, m_vkPipelineLayout(VK_NULL_HANDLE)

	, m_pShaders{ NULL }
	, m_pLayouts{ NULL }
{
	m_pLayouts[DESCRIPTOR_SET_CAMERA] = new CVKDescriptorSetLayout(m_pDevice, DESCRIPTOR_SET_CAMERA);
	m_pLayouts[DESCRIPTOR_SET_FRAME] = new CVKDescriptorSetLayout(m_pDevice, DESCRIPTOR_SET_FRAME);
	m_pLayouts[DESCRIPTOR_SET_PASS] = new CVKDescriptorSetLayout(m_pDevice, DESCRIPTOR_SET_PASS);
	m_pLayouts[DESCRIPTOR_SET_DRAW] = new CVKDescriptorSetLayout(m_pDevice, DESCRIPTOR_SET_DRAW);
}

CVKPipeline::~CVKPipeline(void)
{
	delete m_pLayouts[DESCRIPTOR_SET_CAMERA];
	delete m_pLayouts[DESCRIPTOR_SET_FRAME];
	delete m_pLayouts[DESCRIPTOR_SET_PASS];
	delete m_pLayouts[DESCRIPTOR_SET_DRAW];
}

bool CVKPipeline::CreateLayouts(eastl::vector<VkDescriptorSetLayout> &layouts)
{
	for (int index = 0; index < compute_shader - vertex_shader + 1; index++) {
		if (m_pShaders[index] && m_pShaders[index]->GetShader() && m_pShaders[index]->GetShaderCompiler()) {
			if (const spirv_cross::CompilerGLSL *pShaderCompiler = m_pShaders[index]->GetShaderCompiler()) {
				const spirv_cross::ShaderResources shaderResources = pShaderCompiler->get_shader_resources();

				for (const auto &itPushConstant : shaderResources.push_constant_buffers) {
					if (pShaderCompiler->get_type(itPushConstant.base_type_id).basetype == spirv_cross::SPIRType::Struct) {
						for (uint32_t index = 0; index < pShaderCompiler->get_member_count(itPushConstant.base_type_id); index++) {
							const std::string member = pShaderCompiler->get_member_name(itPushConstant.base_type_id, index);
							const std::string name = itPushConstant.name + "." + member;

							const spirv_cross::SPIRType type = pShaderCompiler->get_type(itPushConstant.base_type_id);
							const uint32_t offset = pShaderCompiler->type_struct_member_offset(type, index);
							const uint32_t size = pShaderCompiler->get_declared_struct_member_size(type, index);
						}
					}
				}

				for (const auto &itUniform : shaderResources.uniform_buffers) {
					if (pShaderCompiler->get_type(itUniform.base_type_id).basetype == spirv_cross::SPIRType::Struct) {
						const uint32_t set = pShaderCompiler->get_decoration(itUniform.id, spv::DecorationDescriptorSet);
						const uint32_t binding = pShaderCompiler->get_decoration(itUniform.id, spv::DecorationBinding);
						if (set >= DESCRIPTOR_SET_COUNT) return false;

						m_pLayouts[set]->SetUniformBlockBinding(itUniform.name.c_str(), binding, VK_SHADER_STAGE_ALL);
					}
				}

				for (const auto &itSampledImage : shaderResources.sampled_images) {
					if (pShaderCompiler->get_type(itSampledImage.base_type_id).basetype == spirv_cross::SPIRType::SampledImage) {
						const uint32_t set = pShaderCompiler->get_decoration(itSampledImage.id, spv::DecorationDescriptorSet);
						const uint32_t binding = pShaderCompiler->get_decoration(itSampledImage.id, spv::DecorationBinding);
						if (set >= DESCRIPTOR_SET_COUNT) return false;

						m_pLayouts[set]->SetSampledImageBinding(itSampledImage.name.c_str(), binding, VK_SHADER_STAGE_ALL);
					}
				}

				for (const auto &itSubpassInput : shaderResources.subpass_inputs) {
					if (pShaderCompiler->get_type(itSubpassInput.base_type_id).basetype == spirv_cross::SPIRType::Image) {
						const uint32_t set = pShaderCompiler->get_decoration(itSubpassInput.id, spv::DecorationDescriptorSet);
						const uint32_t binding = pShaderCompiler->get_decoration(itSubpassInput.id, spv::DecorationBinding);
						if (set >= DESCRIPTOR_SET_COUNT) return false;

						m_pLayouts[set]->SetInputAttachmentBinding(itSubpassInput.name.c_str(), binding, VK_SHADER_STAGE_ALL);
					}
				}
			}
		}
	}

	for (int indexDescriptorSet = 0; indexDescriptorSet < DESCRIPTOR_SET_COUNT; indexDescriptorSet++) {
		if (m_pLayouts[indexDescriptorSet]->Create()) {
			layouts.emplace_back(m_pLayouts[indexDescriptorSet]->GetLayout());
		}
	}

	return true;
}

bool CVKPipeline::CreateShaderStages(eastl::vector<VkPipelineShaderStageCreateInfo> &shaders)
{
	bool rcode = false;

	for (int index = 0; index < compute_shader - vertex_shader + 1; index++) {
		if (m_pShaders[index] && m_pShaders[index]->GetShader() && m_pShaders[index]->GetShaderCompiler()) {
			VkPipelineShaderStageCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			createInfo.pNext = nullptr;
			createInfo.flags = 0;
			createInfo.stage = vkGetShaderStageFlagBits((shader_kind)index);
			createInfo.module = (VkShaderModule)m_pShaders[index]->GetShader();
			createInfo.pName = "main";
			createInfo.pSpecializationInfo = nullptr;
			shaders.emplace_back(createInfo);
			rcode = true;
		}
	}

	return rcode;
}

bool CVKPipeline::CreateVertexInputState(eastl::vector<VkVertexInputBindingDescription> &inputBindingDescriptions, eastl::vector<VkVertexInputAttributeDescription> &inputAttributeDescriptions, uint32_t vertexBinding, uint32_t instanceBinding)
{
	inputBindingDescriptions.clear();
	inputAttributeDescriptions.clear();

	uint32_t vertexFormat = 0;
	uint32_t instanceFormat = 0;

	const spirv_cross::CompilerGLSL *pShaderCompiler = m_pShaders[vertex_shader]->GetShaderCompiler();
	const spirv_cross::ShaderResources shaderResources = pShaderCompiler->get_shader_resources();

	for (const auto &itInput : shaderResources.stage_inputs) {
		vertexFormat |= GetVertexAttribute(itInput.name.c_str());
		instanceFormat |= GetInstanceAttribute(itInput.name.c_str());
	}

	if (vertexFormat || instanceFormat) {
		for (const auto &itInput : shaderResources.stage_inputs) {
			if (uint32_t attribute = GetVertexAttribute(itInput.name.c_str())) {
				VkVertexInputAttributeDescription inputAttributeDescription = {};
				inputAttributeDescription.binding = vertexBinding;
				inputAttributeDescription.location = GetVertexAttributeLocation(attribute);
				inputAttributeDescription.format = (VkFormat)GetVertexAttributeFormat(attribute);
				inputAttributeDescription.offset = GetVertexAttributeOffset(vertexFormat, attribute);
				inputAttributeDescriptions.emplace_back(inputAttributeDescription);
			}

			if (uint32_t attribute = GetInstanceAttribute(itInput.name.c_str())) {
				VkVertexInputAttributeDescription inputAttributeDescription = {};
				inputAttributeDescription.binding = instanceBinding;
				inputAttributeDescription.location = GetInstanceAttributeLocation(attribute);
				inputAttributeDescription.format = (VkFormat)GetInstanceAttributeFormat(attribute);
				inputAttributeDescription.offset = GetInstanceAttributeOffset(instanceFormat, attribute);
				inputAttributeDescriptions.emplace_back(inputAttributeDescription);
			}
		}

		if (vertexFormat) {
			VkVertexInputBindingDescription inputBindingDescription;
			inputBindingDescription.binding = vertexBinding;
			inputBindingDescription.stride = GetVertexStride(vertexFormat);
			inputBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
			inputBindingDescriptions.emplace_back(inputBindingDescription);
		}

		if (instanceFormat) {
			VkVertexInputBindingDescription inputBindingDescription;
			inputBindingDescription.binding = instanceBinding;
			inputBindingDescription.stride = GetInstanceStride(instanceFormat);
			inputBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_INSTANCE;
			inputBindingDescriptions.emplace_back(inputBindingDescription);
		}
	}

	return true;
}
