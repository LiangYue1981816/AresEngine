#pragma once
#include "VKRenderer.h"


class CVKDescriptorLayout : public CGfxDescriptorLayout
{
public:
	CVKDescriptorLayout(CVKDevice* pDevice, uint32_t set);
	virtual ~CVKDescriptorLayout(void);
	virtual void Release(void);


public:
	VkDescriptorSetLayout GetDescriptorLayout(void) const;
	const uint32_t* GetNumDescriptors(void) const;

public:
	bool Create(void);
	void Destroy(bool bClear = true);

public:
	void SetUniformBlockBinding(uint32_t name, uint32_t binding);
	void SetStorageBlockBinding(uint32_t name, uint32_t binding);
	void SetStorageImageBinding(uint32_t name, uint32_t binding);
	void SetSampledImageBinding(uint32_t name, uint32_t binding);
	void SetInputAttachmentBinding(uint32_t name, uint32_t binding);

public:
	uint32_t GetSetIndex(void) const;
	uint32_t GetUniformBlockBinding(uint32_t name) const;
	uint32_t GetStorageBlockBinding(uint32_t name) const;
	uint32_t GetStorageImageBinding(uint32_t name) const;
	uint32_t GetSampledImageBinding(uint32_t name) const;
	uint32_t GetInputAttachmentBinding(uint32_t name) const;

public:
	bool IsUniformBlockValid(uint32_t name) const;
	bool IsStorageBlockValid(uint32_t name) const;
	bool IsStorageImageValid(uint32_t name) const;
	bool IsSampledImageValid(uint32_t name) const;
	bool IsInputAttachmentValid(uint32_t name) const;
	bool IsCompatible(const CGfxDescriptorLayoutPtr ptrLayout) const;


private:
	VkDescriptorSetLayout m_vkDescriptorLayout;

private:
	uint32_t m_set;
	uint32_t m_numDescriptors[VK_DESCRIPTOR_TYPE_RANGE_SIZE];
	eastl::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> m_uniformBlockBindings;    // [name, binding]
	eastl::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> m_storageBlockBindings;    // [name, binding]
	eastl::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> m_storageImageBindings;    // [name, binding]
	eastl::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> m_sampledImageBindings;    // [name, binding]
	eastl::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> m_inputAttachmentBindings; // [name, binding]

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
