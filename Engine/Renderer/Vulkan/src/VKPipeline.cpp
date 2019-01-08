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

				/*
				for (const auto &itPushConstant : shaderResources.push_constant_buffers) {
					if (pShaderCompiler->get_type(itPushConstant.base_type_id).basetype == spirv_cross::SPIRType::Struct) {
						const uint32_t set = pShaderCompiler->get_decoration(itUniform.id, spv::DecorationDescriptorSet);
						if (set >= DESCRIPTOR_SET_COUNT) return false;

						for (uint32_t index = 0; index < pShaderCompiler->get_member_count(itPushConstant.base_type_id); index++) {
							const std::string member = pShaderCompiler->get_member_name(itPushConstant.base_type_id, index);
							const std::string name = itPushConstant.name + "." + member;
						}
					}
				}
				*/

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

	for (int indexLayout = 0; indexLayout < DESCRIPTOR_SET_COUNT; indexLayout++) {
		if (m_pLayouts[indexLayout]->Create()) {
			layouts.emplace_back(m_pLayouts[indexLayout]->GetLayout());
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
			createInfo.module = m_pShaders[index]->GetShader();
			createInfo.pName = "main";
			createInfo.pSpecializationInfo = nullptr;
			shaders.emplace_back(createInfo);
			rcode = true;
		}
	}

	return rcode;
}
