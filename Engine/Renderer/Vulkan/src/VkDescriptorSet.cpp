#include "VKRenderer.h"


CVKDescriptorSetLayout::CVKDescriptorSetLayout(CVKDevice *pDevice, uint32_t set)
	: m_pDevice(pDevice)

	, m_set(set)
	, m_vkDescriptorSetLayout(VK_NULL_HANDLE)
{

}

CVKDescriptorSetLayout::~CVKDescriptorSetLayout(void)
{
	Destroy();
}

uint32_t CVKDescriptorSetLayout::GetSet(void) const
{
	return m_set;
}

VkDescriptorSetLayout CVKDescriptorSetLayout::GetLayout(void) const
{
	return m_vkDescriptorSetLayout;
}

bool CVKDescriptorSetLayout::SetUniformBlockBinding(const char *szName, uint32_t binding, VkShaderStageFlags flags)
{
	uint32_t name = HashValue(szName);

	if (m_uniformBlockBindings.find(name) == m_uniformBlockBindings.end()) {
		m_uniformBlockBindings[name] = {};
		m_uniformBlockBindings[name].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
		m_uniformBlockBindings[name].descriptorCount = 1;
		m_uniformBlockBindings[name].pImmutableSamplers = nullptr;
	}

	m_uniformBlockBindings[name].binding = binding;
	m_uniformBlockBindings[name].stageFlags = flags;

	return true;
}

bool CVKDescriptorSetLayout::SetSampledImageBinding(const char *szName, uint32_t binding, VkShaderStageFlags flags)
{
	uint32_t name = HashValue(szName);

	if (m_sampledImageBindings.find(name) == m_sampledImageBindings.end()) {
		m_sampledImageBindings[name] = {};
		m_sampledImageBindings[name].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		m_sampledImageBindings[name].descriptorCount = 1;
		m_sampledImageBindings[name].pImmutableSamplers = nullptr;
	}

	m_sampledImageBindings[name].binding = binding;
	m_sampledImageBindings[name].stageFlags = flags;

	return true;
}

bool CVKDescriptorSetLayout::SetInputAttachmentBinding(const char *szName, uint32_t binding, VkShaderStageFlags flags)
{
	uint32_t name = HashValue(szName);

	if (m_inputAttachmentBindings.find(name) == m_inputAttachmentBindings.end()) {
		m_inputAttachmentBindings[name] = {};
		m_inputAttachmentBindings[name].descriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
		m_inputAttachmentBindings[name].descriptorCount = 1;
		m_inputAttachmentBindings[name].pImmutableSamplers = nullptr;
	}

	m_inputAttachmentBindings[name].binding = binding;
	m_inputAttachmentBindings[name].stageFlags = flags;

	return true;
}

bool CVKDescriptorSetLayout::Create(void)
{
	eastl::vector<VkDescriptorSetLayoutBinding> bindings;
	{
		for (const auto &itBinding : m_uniformBlockBindings) bindings.emplace_back(itBinding.second);
		for (const auto &itBinding : m_sampledImageBindings) bindings.emplace_back(itBinding.second);
		for (const auto &itBinding : m_inputAttachmentBindings) bindings.emplace_back(itBinding.second);
	}
	if (bindings.empty()) return false;

	VkDescriptorSetLayoutCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.flags = 0;
	createInfo.bindingCount = bindings.size();
	createInfo.pBindings = bindings.data();
	CALL_VK_FUNCTION_RETURN_BOOL(vkCreateDescriptorSetLayout(m_pDevice->GetDevice(), &createInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkDescriptorSetLayout));

	return true;
}

void CVKDescriptorSetLayout::Destroy(void)
{
	if (m_vkDescriptorSetLayout) {
		vkDestroyDescriptorSetLayout(m_pDevice->GetDevice(), m_vkDescriptorSetLayout, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
	}

	m_vkDescriptorSetLayout = VK_NULL_HANDLE;

	m_uniformBlockBindings.clear();
	m_sampledImageBindings.clear();
	m_inputAttachmentBindings.clear();
}
