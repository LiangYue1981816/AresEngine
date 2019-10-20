#include "VKRenderer.h"


CVKDescriptorLayout::CVKDescriptorLayout(CVKDevice* pDevice, CVKDescriptorLayoutManager* pManager, uint32_t set)
	: CGfxDescriptorLayout(set)
	, m_pDevice(pDevice)
	, m_pManager(pManager)

	, m_set(set)
	, m_numDescriptors{ 0 }
	, m_vkDescriptorLayout(VK_NULL_HANDLE)
{

}

CVKDescriptorLayout::~CVKDescriptorLayout(void)
{
	Destroy(true);
}

void CVKDescriptorLayout::Release(void)
{
	m_pManager->Destroy(this);
}

VkDescriptorSetLayout CVKDescriptorLayout::GetDescriptorLayout(void) const
{
	ASSERT(m_vkDescriptorLayout);
	return m_vkDescriptorLayout;
}

const uint32_t* CVKDescriptorLayout::GetNumDescriptors(void) const
{
	return m_numDescriptors;
}

bool CVKDescriptorLayout::Create(void)
{
	Destroy(false);
	{
		do {
			eastl::vector<VkDescriptorSetLayoutBinding> bindings;
			{
				for (const auto& itBinding : m_uniformBlockBindings) {
					bindings.emplace_back(itBinding.second);
				}

				for (const auto& itBinding : m_storageBlockBindings) {
					bindings.emplace_back(itBinding.second);
				}

				for (const auto& itBinding : m_sampledImageBindings) {
					bindings.emplace_back(itBinding.second);
				}

				for (const auto& itBinding : m_inputAttachmentBindings) {
					bindings.emplace_back(itBinding.second);
				}
			}

			VkDescriptorSetLayoutCreateInfo layoutCreateInfo = {};
			layoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
			layoutCreateInfo.pNext = nullptr;
			layoutCreateInfo.flags = 0;
			layoutCreateInfo.bindingCount = bindings.size();
			layoutCreateInfo.pBindings = bindings.data();
			CALL_VK_FUNCTION_BREAK(vkCreateDescriptorSetLayout(m_pDevice->GetDevice(), &layoutCreateInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkDescriptorLayout));

			m_numDescriptors[VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC] = m_uniformBlockBindings.size();
			m_numDescriptors[VK_DESCRIPTOR_TYPE_STORAGE_BUFFER] = m_storageBlockBindings.size();
			m_numDescriptors[VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER] = m_sampledImageBindings.size();
			m_numDescriptors[VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT] = m_inputAttachmentBindings.size();

			return true;
		} while (false);
	}
	Destroy(true);
	return false;
}

void CVKDescriptorLayout::Destroy(bool bClear)
{
	if (bClear) {
		m_uniformBlockBindings.clear();
		m_storageBlockBindings.clear();
		m_sampledImageBindings.clear();
		m_inputAttachmentBindings.clear();
	}

	if (m_vkDescriptorLayout) {
		vkDestroyDescriptorSetLayout(m_pDevice->GetDevice(), m_vkDescriptorLayout, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
	}

	m_vkDescriptorLayout = VK_NULL_HANDLE;
	memset(m_numDescriptors, 0, sizeof(m_numDescriptors));
}

void CVKDescriptorLayout::SetUniformBlockBinding(uint32_t name, uint32_t binding)
{
	m_uniformBlockBindings[name] = {};
	m_uniformBlockBindings[name].binding = binding;
	m_uniformBlockBindings[name].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
	m_uniformBlockBindings[name].descriptorCount = 1;
	m_uniformBlockBindings[name].stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_COMPUTE_BIT;
	m_uniformBlockBindings[name].pImmutableSamplers = nullptr;
}

void CVKDescriptorLayout::SetStorageBlockBinding(uint32_t name, uint32_t binding)
{
	m_storageBlockBindings[name] = {};
	m_storageBlockBindings[name].binding = binding;
	m_storageBlockBindings[name].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	m_storageBlockBindings[name].descriptorCount = 1;
	m_storageBlockBindings[name].stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_COMPUTE_BIT;
	m_storageBlockBindings[name].pImmutableSamplers = nullptr;
}

void CVKDescriptorLayout::SetSampledImageBinding(uint32_t name, uint32_t binding)
{
	m_sampledImageBindings[name] = {};
	m_sampledImageBindings[name].binding = binding;
	m_sampledImageBindings[name].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	m_sampledImageBindings[name].descriptorCount = 1;
	m_sampledImageBindings[name].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	m_sampledImageBindings[name].pImmutableSamplers = nullptr;
}

void CVKDescriptorLayout::SetInputAttachmentBinding(uint32_t name, uint32_t binding)
{
	m_inputAttachmentBindings[name].binding = binding;
	m_inputAttachmentBindings[name].descriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
	m_inputAttachmentBindings[name].descriptorCount = 1;
	m_inputAttachmentBindings[name].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	m_inputAttachmentBindings[name].pImmutableSamplers = nullptr;
}

uint32_t CVKDescriptorLayout::GetSetIndex(void) const
{
	return m_set;
}

uint32_t CVKDescriptorLayout::GetUniformBlockBinding(uint32_t name) const
{
	const auto& itBinding = m_uniformBlockBindings.find(name);

	if (itBinding != m_uniformBlockBindings.end()) {
		return itBinding->second.binding;
	}
	else {
		return -1;
	}
}

uint32_t CVKDescriptorLayout::GetStorageBlockBinding(uint32_t name) const
{
	const auto& itBinding = m_storageBlockBindings.find(name);

	if (itBinding != m_storageBlockBindings.end()) {
		return itBinding->second.binding;
	}
	else {
		return -1;
	}
}

uint32_t CVKDescriptorLayout::GetSampledImageBinding(uint32_t name) const
{
	const auto& itBinding = m_sampledImageBindings.find(name);

	if (itBinding != m_sampledImageBindings.end()) {
		return itBinding->second.binding;
	}
	else {
		return -1;
	}
}

uint32_t CVKDescriptorLayout::GetInputAttachmentBinding(uint32_t name) const
{
	const auto& itBinding = m_inputAttachmentBindings.find(name);

	if (itBinding != m_inputAttachmentBindings.end()) {
		return itBinding->second.binding;
	}
	else {
		return -1;
	}
}

bool CVKDescriptorLayout::IsUniformBlockValid(uint32_t name) const
{
	return GetUniformBlockBinding(name) != -1;
}

bool CVKDescriptorLayout::IsStorageBlockValid(uint32_t name) const
{
	return GetStorageBlockBinding(name) != -1;
}

bool CVKDescriptorLayout::IsSampledImageValid(uint32_t name) const
{
	return GetSampledImageBinding(name) != -1;
}

bool CVKDescriptorLayout::IsInputAttachmentValid(uint32_t name) const
{
	return GetInputAttachmentBinding(name) != -1;
}

bool CVKDescriptorLayout::IsCompatible(const CGfxDescriptorLayoutPtr ptrLayout) const
{
	ASSERT(ptrLayout);

	if (m_set != ((CVKDescriptorLayout*)ptrLayout.GetPointer())->m_set) {
		return false;
	}

	if (m_uniformBlockBindings != ((CVKDescriptorLayout*)ptrLayout.GetPointer())->m_uniformBlockBindings) {
		return false;
	}

	if (m_storageBlockBindings != ((CVKDescriptorLayout*)ptrLayout.GetPointer())->m_storageBlockBindings) {
		return false;
	}

	if (m_sampledImageBindings != ((CVKDescriptorLayout*)ptrLayout.GetPointer())->m_sampledImageBindings) {
		return false;
	}

	if (m_inputAttachmentBindings != ((CVKDescriptorLayout*)ptrLayout.GetPointer())->m_inputAttachmentBindings) {
		return false;
	}

	return true;
}
