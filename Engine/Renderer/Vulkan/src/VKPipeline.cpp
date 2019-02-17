#include "VKRenderer.h"


CVKPipeline::CVKPipeline(CVKDevice *pDevice)
	: m_pDevice(pDevice)

	, m_vkPipeline(VK_NULL_HANDLE)
	, m_vkPipelineLayout(VK_NULL_HANDLE)

	, m_pShaders{ NULL }
	, m_pDescriptorLayouts{ NULL }
{
	m_pDescriptorLayouts[DESCRIPTOR_SET_CAMERA] = new CVKDescriptorLayout(m_pDevice, DESCRIPTOR_SET_CAMERA);
	m_pDescriptorLayouts[DESCRIPTOR_SET_FRAME] = new CVKDescriptorLayout(m_pDevice, DESCRIPTOR_SET_FRAME);
	m_pDescriptorLayouts[DESCRIPTOR_SET_PASS] = new CVKDescriptorLayout(m_pDevice, DESCRIPTOR_SET_PASS);
	m_pDescriptorLayouts[DESCRIPTOR_SET_DRAW] = new CVKDescriptorLayout(m_pDevice, DESCRIPTOR_SET_DRAW);
}

CVKPipeline::~CVKPipeline(void)
{
	delete m_pDescriptorLayouts[DESCRIPTOR_SET_CAMERA];
	delete m_pDescriptorLayouts[DESCRIPTOR_SET_FRAME];
	delete m_pDescriptorLayouts[DESCRIPTOR_SET_PASS];
	delete m_pDescriptorLayouts[DESCRIPTOR_SET_DRAW];
}

bool CVKPipeline::CreateLayouts(eastl::vector<VkDescriptorSetLayout> &layouts, eastl::vector<VkPushConstantRange> &pushConstantRanges)
{
	for (int index = 0; index < compute_shader - vertex_shader + 1; index++) {
		if (m_pShaders[index]) {
			const eastl::unordered_map<eastl::string, PushConstantRange> &pushConstantRanges = m_pShaders[index]->GetSprivCross().GetPushConstantRanges();
			const eastl::unordered_map<eastl::string, DescriptorSetBinding> &uniformBlockBindings = m_pShaders[index]->GetSprivCross().GetUniformBlockBindings();
			const eastl::unordered_map<eastl::string, DescriptorSetBinding> &sampledImageBindings = m_pShaders[index]->GetSprivCross().GetSampledImageBindings();
			const eastl::unordered_map<eastl::string, DescriptorSetBinding> &inputAttachmentBindings = m_pShaders[index]->GetSprivCross().GetInputAttachmentBindings();

			for (const auto &itPushConstant : pushConstantRanges) {
				uint32_t name = HashValue(itPushConstant.first.c_str());
				m_pushConstantRanges[name].stageFlags = VK_SHADER_STAGE_ALL;
				m_pushConstantRanges[name].offset = itPushConstant.second.offset;
				m_pushConstantRanges[name].size = itPushConstant.second.range;
			}

			for (const auto &itUniform : uniformBlockBindings) {
				m_pDescriptorLayouts[itUniform.second.set]->SetUniformBlockBinding(itUniform.first.c_str(), itUniform.second.binding, VK_SHADER_STAGE_ALL);
			}

			for (const auto &itSampledImage : sampledImageBindings) {
				m_pDescriptorLayouts[itSampledImage.second.set]->SetSampledImageBinding(itSampledImage.first.c_str(), itSampledImage.second.binding, VK_SHADER_STAGE_ALL);
			}

			for (const auto &itInputAttachment : inputAttachmentBindings) {
				m_pDescriptorLayouts[itInputAttachment.second.set]->SetInputAttachmentBinding(itInputAttachment.first.c_str(), itInputAttachment.second.binding, VK_SHADER_STAGE_ALL);
			}
		}
	}

	for (int index = 0; index < DESCRIPTOR_SET_COUNT; index++) {
		if (m_pDescriptorLayouts[index]->Create()) {
			layouts.emplace_back(m_pDescriptorLayouts[index]->GetDescriptorSetLayout());
		}
	}

	for (const auto &itPushConstantRange : m_pushConstantRanges) {
		pushConstantRanges.emplace_back(itPushConstantRange.second);
	}

	return true;
}

bool CVKPipeline::CreateShaderStages(eastl::vector<VkPipelineShaderStageCreateInfo> &shaders)
{
	bool rcode = false;

	for (int index = 0; index < compute_shader - vertex_shader + 1; index++) {
		if (m_pShaders[index] && m_pShaders[index]->GetShader()) {
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

bool CVKPipeline::Uniform1i(VkCommandBuffer vkCommandBuffer, uint32_t name, int v0) const
{
	const auto &itPushConstant = m_pushConstantRanges.find(name);

	if (itPushConstant != m_pushConstantRanges.end()) {
		glm::ivec4 vec(v0, 0, 0, 0);
		vkCmdPushConstants(vkCommandBuffer, m_vkPipelineLayout, VK_SHADER_STAGE_ALL, itPushConstant->second.offset, itPushConstant->second.size, &vec);
		return true;
	}
	else {
		return false;
	}
}

bool CVKPipeline::Uniform2i(VkCommandBuffer vkCommandBuffer, uint32_t name, int v0, int v1) const
{
	const auto &itPushConstant = m_pushConstantRanges.find(name);

	if (itPushConstant != m_pushConstantRanges.end()) {
		glm::ivec4 vec(v0, v1, 0, 0);
		vkCmdPushConstants(vkCommandBuffer, m_vkPipelineLayout, VK_SHADER_STAGE_ALL, itPushConstant->second.offset, itPushConstant->second.size, &vec);
		return true;
	}
	else {
		return false;
	}
}

bool CVKPipeline::Uniform3i(VkCommandBuffer vkCommandBuffer, uint32_t name, int v0, int v1, int v2) const
{
	const auto &itPushConstant = m_pushConstantRanges.find(name);

	if (itPushConstant != m_pushConstantRanges.end()) {
		glm::ivec4 vec(v0, v1, v2, 0);
		vkCmdPushConstants(vkCommandBuffer, m_vkPipelineLayout, VK_SHADER_STAGE_ALL, itPushConstant->second.offset, itPushConstant->second.size, &vec);
		return true;
	}
	else {
		return false;
	}
}

bool CVKPipeline::Uniform4i(VkCommandBuffer vkCommandBuffer, uint32_t name, int v0, int v1, int v2, int v3) const
{
	const auto &itPushConstant = m_pushConstantRanges.find(name);

	if (itPushConstant != m_pushConstantRanges.end()) {
		glm::ivec4 vec(v0, v1, v2, v3);
		vkCmdPushConstants(vkCommandBuffer, m_vkPipelineLayout, VK_SHADER_STAGE_ALL, itPushConstant->second.offset, itPushConstant->second.size, &vec);
		return true;
	}
	else {
		return false;
	}
}

bool CVKPipeline::Uniform1f(VkCommandBuffer vkCommandBuffer, uint32_t name, float v0) const
{
	const auto &itPushConstant = m_pushConstantRanges.find(name);

	if (itPushConstant != m_pushConstantRanges.end()) {
		glm::vec4 vec(v0, 0.0f, 0.0f, 0.0f);
		vkCmdPushConstants(vkCommandBuffer, m_vkPipelineLayout, VK_SHADER_STAGE_ALL, itPushConstant->second.offset, itPushConstant->second.size, &vec);
		return true;
	}
	else {
		return false;
	}
}

bool CVKPipeline::Uniform2f(VkCommandBuffer vkCommandBuffer, uint32_t name, float v0, float v1) const
{
	const auto &itPushConstant = m_pushConstantRanges.find(name);

	if (itPushConstant != m_pushConstantRanges.end()) {
		glm::vec4 vec(v0, v1, 0.0f, 0.0f);
		vkCmdPushConstants(vkCommandBuffer, m_vkPipelineLayout, VK_SHADER_STAGE_ALL, itPushConstant->second.offset, itPushConstant->second.size, &vec);
		return true;
	}
	else {
		return false;
	}
}

bool CVKPipeline::Uniform3f(VkCommandBuffer vkCommandBuffer, uint32_t name, float v0, float v1, float v2) const
{
	const auto &itPushConstant = m_pushConstantRanges.find(name);

	if (itPushConstant != m_pushConstantRanges.end()) {
		glm::vec4 vec(v0, v1, v2, 0.0f);
		vkCmdPushConstants(vkCommandBuffer, m_vkPipelineLayout, VK_SHADER_STAGE_ALL, itPushConstant->second.offset, itPushConstant->second.size, &vec);
		return true;
	}
	else {
		return false;
	}
}

bool CVKPipeline::Uniform4f(VkCommandBuffer vkCommandBuffer, uint32_t name, float v0, float v1, float v2, float v3) const
{
	const auto &itPushConstant = m_pushConstantRanges.find(name);

	if (itPushConstant != m_pushConstantRanges.end()) {
		glm::vec4 vec(v0, v1, v2, v3);
		vkCmdPushConstants(vkCommandBuffer, m_vkPipelineLayout, VK_SHADER_STAGE_ALL, itPushConstant->second.offset, itPushConstant->second.size, &vec);
		return true;
	}
	else {
		return false;
	}
}

bool CVKPipeline::Uniform1iv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const int *value) const
{
	const auto &itPushConstant = m_pushConstantRanges.find(name);

	if (itPushConstant != m_pushConstantRanges.end()) {
		vkCmdPushConstants(vkCommandBuffer, m_vkPipelineLayout, VK_SHADER_STAGE_ALL, itPushConstant->second.offset, itPushConstant->second.size, value);
		return true;
	}
	else {
		return false;
	}
}

bool CVKPipeline::Uniform2iv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const int *value) const
{
	const auto &itPushConstant = m_pushConstantRanges.find(name);

	if (itPushConstant != m_pushConstantRanges.end()) {
		vkCmdPushConstants(vkCommandBuffer, m_vkPipelineLayout, VK_SHADER_STAGE_ALL, itPushConstant->second.offset, itPushConstant->second.size, value);
		return true;
	}
	else {
		return false;
	}
}

bool CVKPipeline::Uniform3iv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const int *value) const
{
	const auto &itPushConstant = m_pushConstantRanges.find(name);

	if (itPushConstant != m_pushConstantRanges.end()) {
		vkCmdPushConstants(vkCommandBuffer, m_vkPipelineLayout, VK_SHADER_STAGE_ALL, itPushConstant->second.offset, itPushConstant->second.size, value);
		return true;
	}
	else {
		return false;
	}
}

bool CVKPipeline::Uniform4iv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const int *value) const
{
	const auto &itPushConstant = m_pushConstantRanges.find(name);

	if (itPushConstant != m_pushConstantRanges.end()) {
		vkCmdPushConstants(vkCommandBuffer, m_vkPipelineLayout, VK_SHADER_STAGE_ALL, itPushConstant->second.offset, itPushConstant->second.size, value);
		return true;
	}
	else {
		return false;
	}
}

bool CVKPipeline::Uniform1fv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const float *value) const
{
	const auto &itPushConstant = m_pushConstantRanges.find(name);

	if (itPushConstant != m_pushConstantRanges.end()) {
		vkCmdPushConstants(vkCommandBuffer, m_vkPipelineLayout, VK_SHADER_STAGE_ALL, itPushConstant->second.offset, itPushConstant->second.size, value);
		return true;
	}
	else {
		return false;
	}
}

bool CVKPipeline::Uniform2fv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const float *value) const
{
	const auto &itPushConstant = m_pushConstantRanges.find(name);

	if (itPushConstant != m_pushConstantRanges.end()) {
		vkCmdPushConstants(vkCommandBuffer, m_vkPipelineLayout, VK_SHADER_STAGE_ALL, itPushConstant->second.offset, itPushConstant->second.size, value);
		return true;
	}
	else {
		return false;
	}
}

bool CVKPipeline::Uniform3fv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const float *value) const
{
	const auto &itPushConstant = m_pushConstantRanges.find(name);

	if (itPushConstant != m_pushConstantRanges.end()) {
		vkCmdPushConstants(vkCommandBuffer, m_vkPipelineLayout, VK_SHADER_STAGE_ALL, itPushConstant->second.offset, itPushConstant->second.size, value);
		return true;
	}
	else {
		return false;
	}
}

bool CVKPipeline::Uniform4fv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const float *value) const
{
	const auto &itPushConstant = m_pushConstantRanges.find(name);

	if (itPushConstant != m_pushConstantRanges.end()) {
		vkCmdPushConstants(vkCommandBuffer, m_vkPipelineLayout, VK_SHADER_STAGE_ALL, itPushConstant->second.offset, itPushConstant->second.size, value);
		return true;
	}
	else {
		return false;
	}
}

bool CVKPipeline::UniformMatrix2fv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const float *value) const
{
	const auto &itPushConstant = m_pushConstantRanges.find(name);

	if (itPushConstant != m_pushConstantRanges.end()) {
		vkCmdPushConstants(vkCommandBuffer, m_vkPipelineLayout, VK_SHADER_STAGE_ALL, itPushConstant->second.offset, itPushConstant->second.size, value);
		return true;
	}
	else {
		return false;
	}
}

bool CVKPipeline::UniformMatrix3fv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const float *value) const
{
	const auto &itPushConstant = m_pushConstantRanges.find(name);

	if (itPushConstant != m_pushConstantRanges.end()) {
		vkCmdPushConstants(vkCommandBuffer, m_vkPipelineLayout, VK_SHADER_STAGE_ALL, itPushConstant->second.offset, itPushConstant->second.size, value);
		return true;
	}
	else {
		return false;
	}
}

bool CVKPipeline::UniformMatrix4fv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const float *value) const
{
	const auto &itPushConstant = m_pushConstantRanges.find(name);

	if (itPushConstant != m_pushConstantRanges.end()) {
		vkCmdPushConstants(vkCommandBuffer, m_vkPipelineLayout, VK_SHADER_STAGE_ALL, itPushConstant->second.offset, itPushConstant->second.size, value);
		return true;
	}
	else {
		return false;
	}
}

bool CVKPipeline::IsTextureValid(uint32_t name) const
{
	for (int index = 0; index < DESCRIPTOR_SET_COUNT; index++) {
		if (m_pDescriptorLayouts[index]->IsTextureValid(name)) {
			return true;
		}
	}

	return false;
}

bool CVKPipeline::IsUniformBlockValid(uint32_t name) const
{
	for (int index = 0; index < DESCRIPTOR_SET_COUNT; index++) {
		if (m_pDescriptorLayouts[index]->IsUniformBlockValid(name)) {
			return true;
		}
	}

	return false;
}

bool CVKPipeline::IsUniformValid(uint32_t name) const
{
	return m_pushConstantRanges.find(name) != m_pushConstantRanges.end();
}

CVKDescriptorLayout* CVKPipeline::GetDescriptorLayout(uint32_t set) const
{
	if (set < DESCRIPTOR_SET_COUNT) {
		return m_pDescriptorLayouts[set];
	}
	else {
		return nullptr;
	}
}
