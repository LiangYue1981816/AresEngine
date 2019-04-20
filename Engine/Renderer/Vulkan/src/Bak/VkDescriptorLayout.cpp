#include "VKRenderer.h"


CVKDescriptorLayout::CVKDescriptorLayout(CVKDevice *pDevice, uint32_t set)
	: m_pDevice(pDevice)

	, m_set(set)
	, m_numDescriptors{ 0 }
	, m_vkDescriptorSetLayout(VK_NULL_HANDLE)
{

}

CVKDescriptorLayout::~CVKDescriptorLayout(void)
{
	Destroy();
}

bool CVKDescriptorLayout::IsCompatible(const CVKDescriptorLayout &layout) const
{
	if (m_set != layout.m_set) {
		return false;
	}

	if (m_uniformBlockBindings != layout.m_uniformBlockBindings) {
		return false;
	}

	if (m_sampledImageBindings != layout.m_sampledImageBindings) {
		return false;
	}

	if (m_inputAttachmentBindings != layout.m_inputAttachmentBindings) {
		return false;
	}

	return true;
}

uint32_t CVKDescriptorLayout::GetSetIndex(void) const
{
	return m_set;
}

VkDescriptorSetLayout CVKDescriptorLayout::GetDescriptorSetLayout(void) const
{
	return m_vkDescriptorSetLayout;
}

const uint32_t* CVKDescriptorLayout::GetNumDescriptors(void) const
{
	return m_numDescriptors;
}

bool CVKDescriptorLayout::IsTextureValid(uint32_t name) const
{
	return m_sampledImageBindings.find(name) != m_sampledImageBindings.end();
}

bool CVKDescriptorLayout::IsUniformBlockValid(uint32_t name) const
{
	return m_uniformBlockBindings.find(name) != m_uniformBlockBindings.end();
}

VkDescriptorSetLayoutBinding CVKDescriptorLayout::GetTextureBinding(uint32_t name) const
{
	const auto &itBinding = m_sampledImageBindings.find(name);

	if (itBinding != m_sampledImageBindings.end()) {
		return itBinding->second;
	}
	else {
		return VkDescriptorSetLayoutBinding{};
	}
}

VkDescriptorSetLayoutBinding CVKDescriptorLayout::GetUniformBlockBinding(uint32_t name) const
{
	const auto &itBinding = m_uniformBlockBindings.find(name);

	if (itBinding != m_uniformBlockBindings.end()) {
		return itBinding->second;
	}
	else {
		return VkDescriptorSetLayoutBinding{};
	}
}

bool CVKDescriptorLayout::SetUniformBlockBinding(const char *szName, uint32_t binding, VkShaderStageFlags flags)
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

bool CVKDescriptorLayout::SetSampledImageBinding(const char *szName, uint32_t binding, VkShaderStageFlags flags)
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

bool CVKDescriptorLayout::SetInputAttachmentBinding(const char *szName, uint32_t binding, VkShaderStageFlags flags)
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

bool CVKDescriptorLayout::Create(void)
{
	Destroy();
	{
		do {
			eastl::vector<VkDescriptorSetLayoutBinding> bindings;
			{
				for (const auto &itBinding : m_uniformBlockBindings) bindings.emplace_back(itBinding.second);
				for (const auto &itBinding : m_sampledImageBindings) bindings.emplace_back(itBinding.second);
				for (const auto &itBinding : m_inputAttachmentBindings) bindings.emplace_back(itBinding.second);
			}
			if (bindings.empty()) break;

			VkDescriptorSetLayoutCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
			createInfo.pNext = nullptr;
			createInfo.flags = 0;
			createInfo.bindingCount = bindings.size();
			createInfo.pBindings = bindings.data();
			CALL_VK_FUNCTION_BREAK(vkCreateDescriptorSetLayout(m_pDevice->GetDevice(), &createInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkDescriptorSetLayout));

			m_numDescriptors[VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC] = m_uniformBlockBindings.size();
			m_numDescriptors[VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER] = m_sampledImageBindings.size();
			m_numDescriptors[VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT] = m_inputAttachmentBindings.size();

			return true;
		} while (false);
	}
	Destroy();
	return false;
}

void CVKDescriptorLayout::Destroy(void)
{
	if (m_vkDescriptorSetLayout) {
		vkDestroyDescriptorSetLayout(m_pDevice->GetDevice(), m_vkDescriptorSetLayout, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
	}

	m_vkDescriptorSetLayout = VK_NULL_HANDLE;
	memset(m_numDescriptors, 0, sizeof(m_numDescriptors));
}
