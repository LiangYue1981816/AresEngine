#pragma once
#include "VKRenderer.h"


class CVKDescriptorLayout
{
	friend class CVKPipeline;


public:
	CVKDescriptorLayout(CVKDevice* pDevice, uint32_t set);
	virtual ~CVKDescriptorLayout(void);


public:
	bool IsCompatible(const CVKDescriptorLayout& layout) const;

public:
	uint32_t GetSetIndex(void) const;
	VkDescriptorSetLayout GetDescriptorSetLayout(void) const;

public:
	const uint32_t* GetNumDescriptors(void) const;

	bool IsTextureValid(uint32_t name) const;
	bool IsUniformBlockValid(uint32_t name) const;

	VkDescriptorSetLayoutBinding GetTextureBinding(uint32_t name) const;
	VkDescriptorSetLayoutBinding GetUniformBlockBinding(uint32_t name) const;

public:
	bool SetUniformBlockBinding(const char* szName, uint32_t binding, VkShaderStageFlags flags);
	bool SetSampledImageBinding(const char* szName, uint32_t binding, VkShaderStageFlags flags);
	bool SetInputAttachmentBinding(const char* szName, uint32_t binding, VkShaderStageFlags flags);

public:
	bool Create(void);
	void Destroy(void);


private:
	uint32_t m_set;
	uint32_t m_numDescriptors[VK_DESCRIPTOR_TYPE_RANGE_SIZE];
	eastl::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> m_uniformBlockBindings;    // [name, binding]
	eastl::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> m_sampledImageBindings;    // [name, binding]
	eastl::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> m_inputAttachmentBindings; // [name, binding]

private:
	VkDescriptorSetLayout m_vkDescriptorSetLayout;

private:
	CVKDevice* m_pDevice;
};

inline bool operator == (const VkDescriptorSetLayoutBinding& left, const VkDescriptorSetLayoutBinding& right)
{
	if (left.binding != right.binding) {
		return false;
	}

	if (left.descriptorType != right.descriptorType) {
		return false;
	}

	if (left.descriptorCount != right.descriptorCount) {
		return false;
	}

	if (left.stageFlags != right.stageFlags) {
		return false;
	}

	return true;
}
