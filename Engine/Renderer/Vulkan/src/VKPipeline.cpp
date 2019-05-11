#include "VKRenderer.h"


CVKPipeline::CVKPipeline(CVKDevice* pDevice)
	: m_pDevice(pDevice)

	, m_pShaders{ nullptr }
	, m_vkPipeline(VK_NULL_HANDLE)
	, m_vkPipelineLayout(VK_NULL_HANDLE)
{
	ASSERT(m_pDevice);

	m_ptrDescriptorLayouts[DESCRIPTOR_SET_ENGINE] = VKRenderer()->NewDescriptorLayout(DESCRIPTOR_SET_ENGINE);
	m_ptrDescriptorLayouts[DESCRIPTOR_SET_CAMERA] = VKRenderer()->NewDescriptorLayout(DESCRIPTOR_SET_CAMERA);
	m_ptrDescriptorLayouts[DESCRIPTOR_SET_PASS] = VKRenderer()->NewDescriptorLayout(DESCRIPTOR_SET_PASS);
	m_ptrDescriptorLayouts[DESCRIPTOR_SET_INPUTATTACHMENT] = VKRenderer()->NewDescriptorLayout(DESCRIPTOR_SET_INPUTATTACHMENT);
}

CVKPipeline::~CVKPipeline(void)
{
	Destroy();
}

bool CVKPipeline::CreateLayouts(eastl::vector<VkDescriptorSetLayout>& layouts, eastl::vector<VkPushConstantRange>& pushConstantRanges)
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
				m_ptrDescriptorLayouts[itInputAttachment.second.set]->SetInputAttachmentBinding(HashValue(itInputAttachment.first.c_str()), itInputAttachment.second.binding);
			}
		}
	}

	CALL_BOOL_FUNCTION_RETURN_BOOL(m_ptrDescriptorLayouts[DESCRIPTOR_SET_ENGINE]->Create());
	CALL_BOOL_FUNCTION_RETURN_BOOL(m_ptrDescriptorLayouts[DESCRIPTOR_SET_CAMERA]->Create());
	CALL_BOOL_FUNCTION_RETURN_BOOL(m_ptrDescriptorLayouts[DESCRIPTOR_SET_PASS]->Create());
	CALL_BOOL_FUNCTION_RETURN_BOOL(m_ptrDescriptorLayouts[DESCRIPTOR_SET_INPUTATTACHMENT]->Create());

	for (int index = 0; index < DESCRIPTOR_SET_COUNT; index++) {
		layouts.emplace_back(((CVKDescriptorLayout*)m_ptrDescriptorLayouts[index].GetPointer())->GetDescriptorLayout());
	}

	for (const auto& itPushConstantRange : m_pushConstantRanges) {
		pushConstantRanges.emplace_back(itPushConstantRange.second);
	}

	return true;
}

bool CVKPipeline::CreateShaderStages(eastl::vector<VkPipelineShaderStageCreateInfo>& shaders)
{
	for (int indexShader = 0; indexShader < compute_shader - vertex_shader + 1; indexShader++) {
		if (m_pShaders[indexShader] && m_pShaders[indexShader]->IsValid()) {
			VkPipelineShaderStageCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			createInfo.pNext = nullptr;
			createInfo.flags = 0;
			createInfo.stage = vkGetShaderStageFlagBits((shader_kind)indexShader);
			createInfo.module = m_pShaders[indexShader]->GetShader();
			createInfo.pName = "main";
			createInfo.pSpecializationInfo = nullptr;
			shaders.emplace_back(createInfo);
		}
	}

	return true;
}

bool CVKPipeline::CreateVertexInputState(eastl::vector<VkVertexInputBindingDescription>& inputBindingDescriptions, eastl::vector<VkVertexInputAttributeDescription>& inputAttributeDescriptions, int vertexBinding, int instanceBinding)
{
	ASSERT(m_pShaders[vertex_shader]);
	ASSERT(m_pShaders[vertex_shader]->IsValid());

	inputBindingDescriptions.clear();
	inputAttributeDescriptions.clear();

	uint32_t vertexFormat = 0;
	uint32_t instanceFormat = 0;

	const eastl::vector<eastl::string>& vertexAttributes = m_pShaders[vertex_shader]->GetSprivCross().GetVertexAttributes();

	for (const auto& itVertexAttribute : vertexAttributes) {
		vertexFormat |= GetVertexAttribute(itVertexAttribute.c_str());
		instanceFormat |= GetInstanceAttribute(itVertexAttribute.c_str());
	}

	if (vertexFormat || instanceFormat) {
		for (const auto& itVertexAttribute : vertexAttributes) {
			if (uint32_t attribute = GetVertexAttribute(itVertexAttribute.c_str())) {
				VkVertexInputAttributeDescription inputAttributeDescription = {};
				inputAttributeDescription.binding = vertexBinding;
				inputAttributeDescription.location = GetVertexAttributeLocation(attribute);
				inputAttributeDescription.format = (VkFormat)GetVertexAttributeFormat(attribute);
				inputAttributeDescription.offset = GetVertexAttributeOffset(vertexFormat, attribute);
				inputAttributeDescriptions.emplace_back(inputAttributeDescription);
			}

			if (uint32_t attribute = GetInstanceAttribute(itVertexAttribute.c_str())) {
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

void CVKPipeline::Destroy(void)
{

}

void CVKPipeline::Uniform1i(VkCommandBuffer vkCommandBuffer, uint32_t name, int v0) const
{
	const auto& itPushConstant = m_pushConstantRanges.find(name);

	if (itPushConstant != m_pushConstantRanges.end()) {
		glm::ivec4 vec(v0, 0, 0, 0);

		ASSERT(vkCommandBuffer);
		ASSERT(itPushConstant->second.size == sizeof(vec));
		vkCmdPushConstants(vkCommandBuffer, m_vkPipelineLayout, itPushConstant->second.stageFlags, itPushConstant->second.offset, itPushConstant->second.size, &vec);
	}
}

void CVKPipeline::Uniform2i(VkCommandBuffer vkCommandBuffer, uint32_t name, int v0, int v1) const
{
	const auto& itPushConstant = m_pushConstantRanges.find(name);

	if (itPushConstant != m_pushConstantRanges.end()) {
		glm::ivec4 vec(v0, v1, 0, 0);

		ASSERT(vkCommandBuffer);
		ASSERT(itPushConstant->second.size == sizeof(vec));
		vkCmdPushConstants(vkCommandBuffer, m_vkPipelineLayout, itPushConstant->second.stageFlags, itPushConstant->second.offset, itPushConstant->second.size, &vec);
	}
}

void CVKPipeline::Uniform3i(VkCommandBuffer vkCommandBuffer, uint32_t name, int v0, int v1, int v2) const
{
	const auto& itPushConstant = m_pushConstantRanges.find(name);

	if (itPushConstant != m_pushConstantRanges.end()) {
		glm::ivec4 vec(v0, v1, v2, 0);

		ASSERT(vkCommandBuffer);
		ASSERT(itPushConstant->second.size == sizeof(vec));
		vkCmdPushConstants(vkCommandBuffer, m_vkPipelineLayout, itPushConstant->second.stageFlags, itPushConstant->second.offset, itPushConstant->second.size, &vec);
	}
}

void CVKPipeline::Uniform4i(VkCommandBuffer vkCommandBuffer, uint32_t name, int v0, int v1, int v2, int v3) const
{
	const auto& itPushConstant = m_pushConstantRanges.find(name);

	if (itPushConstant != m_pushConstantRanges.end()) {
		glm::ivec4 vec(v0, v1, v2, v3);

		ASSERT(vkCommandBuffer);
		ASSERT(itPushConstant->second.size == sizeof(vec));
		vkCmdPushConstants(vkCommandBuffer, m_vkPipelineLayout, itPushConstant->second.stageFlags, itPushConstant->second.offset, itPushConstant->second.size, &vec);
	}
}

void CVKPipeline::Uniform1f(VkCommandBuffer vkCommandBuffer, uint32_t name, float v0) const
{
	const auto& itPushConstant = m_pushConstantRanges.find(name);

	if (itPushConstant != m_pushConstantRanges.end()) {
		glm::vec4 vec(v0, 0.0f, 0.0f, 0.0f);

		ASSERT(vkCommandBuffer);
		ASSERT(itPushConstant->second.size == sizeof(vec));
		vkCmdPushConstants(vkCommandBuffer, m_vkPipelineLayout, itPushConstant->second.stageFlags, itPushConstant->second.offset, itPushConstant->second.size, &vec);
	}
}

void CVKPipeline::Uniform2f(VkCommandBuffer vkCommandBuffer, uint32_t name, float v0, float v1) const
{
	const auto& itPushConstant = m_pushConstantRanges.find(name);

	if (itPushConstant != m_pushConstantRanges.end()) {
		glm::vec4 vec(v0, v1, 0.0f, 0.0f);

		ASSERT(vkCommandBuffer);
		ASSERT(itPushConstant->second.size == sizeof(vec));
		vkCmdPushConstants(vkCommandBuffer, m_vkPipelineLayout, itPushConstant->second.stageFlags, itPushConstant->second.offset, itPushConstant->second.size, &vec);
	}
}

void CVKPipeline::Uniform3f(VkCommandBuffer vkCommandBuffer, uint32_t name, float v0, float v1, float v2) const
{
	const auto& itPushConstant = m_pushConstantRanges.find(name);

	if (itPushConstant != m_pushConstantRanges.end()) {
		glm::vec4 vec(v0, v1, v2, 0.0f);

		ASSERT(vkCommandBuffer);
		ASSERT(itPushConstant->second.size == sizeof(vec));
		vkCmdPushConstants(vkCommandBuffer, m_vkPipelineLayout, itPushConstant->second.stageFlags, itPushConstant->second.offset, itPushConstant->second.size, &vec);
	}
}

void CVKPipeline::Uniform4f(VkCommandBuffer vkCommandBuffer, uint32_t name, float v0, float v1, float v2, float v3) const
{
	const auto& itPushConstant = m_pushConstantRanges.find(name);

	if (itPushConstant != m_pushConstantRanges.end()) {
		glm::vec4 vec(v0, v1, v2, v3);

		ASSERT(vkCommandBuffer);
		ASSERT(itPushConstant->second.size == sizeof(vec));
		vkCmdPushConstants(vkCommandBuffer, m_vkPipelineLayout, itPushConstant->second.stageFlags, itPushConstant->second.offset, itPushConstant->second.size, &vec);
	}
}

void CVKPipeline::Uniform1iv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const int* value) const
{
	const auto& itPushConstant = m_pushConstantRanges.find(name);

	if (itPushConstant != m_pushConstantRanges.end()) {
		ASSERT(value);
		ASSERT(vkCommandBuffer);
		ASSERT(itPushConstant->second.size == sizeof(*value) * count);
		vkCmdPushConstants(vkCommandBuffer, m_vkPipelineLayout, itPushConstant->second.stageFlags, itPushConstant->second.offset, itPushConstant->second.size, value);
	}
}

void CVKPipeline::Uniform2iv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const int* value) const
{
	const auto& itPushConstant = m_pushConstantRanges.find(name);

	if (itPushConstant != m_pushConstantRanges.end()) {
		ASSERT(value);
		ASSERT(vkCommandBuffer);
		ASSERT(itPushConstant->second.size == sizeof(*value) * count);
		vkCmdPushConstants(vkCommandBuffer, m_vkPipelineLayout, itPushConstant->second.stageFlags, itPushConstant->second.offset, itPushConstant->second.size, value);
	}
}

void CVKPipeline::Uniform3iv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const int* value) const
{
	const auto& itPushConstant = m_pushConstantRanges.find(name);

	if (itPushConstant != m_pushConstantRanges.end()) {
		ASSERT(value);
		ASSERT(vkCommandBuffer);
		ASSERT(itPushConstant->second.size == sizeof(*value) * count);
		vkCmdPushConstants(vkCommandBuffer, m_vkPipelineLayout, itPushConstant->second.stageFlags, itPushConstant->second.offset, itPushConstant->second.size, value);
	}
}

void CVKPipeline::Uniform4iv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const int* value) const
{
	const auto& itPushConstant = m_pushConstantRanges.find(name);

	if (itPushConstant != m_pushConstantRanges.end()) {
		ASSERT(value);
		ASSERT(vkCommandBuffer);
		ASSERT(itPushConstant->second.size == sizeof(*value) * count);
		vkCmdPushConstants(vkCommandBuffer, m_vkPipelineLayout, itPushConstant->second.stageFlags, itPushConstant->second.offset, itPushConstant->second.size, value);
	}
}

void CVKPipeline::Uniform1fv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const float* value) const
{
	const auto& itPushConstant = m_pushConstantRanges.find(name);

	if (itPushConstant != m_pushConstantRanges.end()) {
		ASSERT(value);
		ASSERT(vkCommandBuffer);
		ASSERT(itPushConstant->second.size == sizeof(*value) * count);
		vkCmdPushConstants(vkCommandBuffer, m_vkPipelineLayout, itPushConstant->second.stageFlags, itPushConstant->second.offset, itPushConstant->second.size, value);
	}
}

void CVKPipeline::Uniform2fv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const float* value) const
{
	const auto& itPushConstant = m_pushConstantRanges.find(name);

	if (itPushConstant != m_pushConstantRanges.end()) {
		ASSERT(value);
		ASSERT(vkCommandBuffer);
		ASSERT(itPushConstant->second.size == sizeof(*value) * count);
		vkCmdPushConstants(vkCommandBuffer, m_vkPipelineLayout, itPushConstant->second.stageFlags, itPushConstant->second.offset, itPushConstant->second.size, value);
	}
}

void CVKPipeline::Uniform3fv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const float* value) const
{
	const auto& itPushConstant = m_pushConstantRanges.find(name);

	if (itPushConstant != m_pushConstantRanges.end()) {
		ASSERT(value);
		ASSERT(vkCommandBuffer);
		ASSERT(itPushConstant->second.size == sizeof(*value) * count);
		vkCmdPushConstants(vkCommandBuffer, m_vkPipelineLayout, itPushConstant->second.stageFlags, itPushConstant->second.offset, itPushConstant->second.size, value);
	}
}

void CVKPipeline::Uniform4fv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const float* value) const
{
	const auto& itPushConstant = m_pushConstantRanges.find(name);

	if (itPushConstant != m_pushConstantRanges.end()) {
		ASSERT(value);
		ASSERT(vkCommandBuffer);
		ASSERT(itPushConstant->second.size == sizeof(*value) * count);
		vkCmdPushConstants(vkCommandBuffer, m_vkPipelineLayout, itPushConstant->second.stageFlags, itPushConstant->second.offset, itPushConstant->second.size, value);
	}
}

void CVKPipeline::UniformMatrix2fv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const float* value) const
{
	const auto& itPushConstant = m_pushConstantRanges.find(name);

	if (itPushConstant != m_pushConstantRanges.end()) {
		ASSERT(value);
		ASSERT(vkCommandBuffer);
		ASSERT(itPushConstant->second.size == sizeof(*value) * count);
		vkCmdPushConstants(vkCommandBuffer, m_vkPipelineLayout, itPushConstant->second.stageFlags, itPushConstant->second.offset, itPushConstant->second.size, value);
	}
}

void CVKPipeline::UniformMatrix3fv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const float* value) const
{
	const auto& itPushConstant = m_pushConstantRanges.find(name);

	if (itPushConstant != m_pushConstantRanges.end()) {
		ASSERT(value);
		ASSERT(vkCommandBuffer);
		ASSERT(itPushConstant->second.size == sizeof(*value) * count);
		vkCmdPushConstants(vkCommandBuffer, m_vkPipelineLayout, itPushConstant->second.stageFlags, itPushConstant->second.offset, itPushConstant->second.size, value);
	}
}

void CVKPipeline::UniformMatrix4fv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const float* value) const
{
	const auto& itPushConstant = m_pushConstantRanges.find(name);

	if (itPushConstant != m_pushConstantRanges.end()) {
		ASSERT(value);
		ASSERT(vkCommandBuffer);
		ASSERT(itPushConstant->second.size == sizeof(*value) * count);
		vkCmdPushConstants(vkCommandBuffer, m_vkPipelineLayout, itPushConstant->second.stageFlags, itPushConstant->second.offset, itPushConstant->second.size, value);
	}
}
