#pragma once
#include "VKRenderer.h"


class CVKDescriptorLayout : public CGfxDescriptorLayout
{
	friend class CVKDescriptorLayoutManager;


private:
	CVKDescriptorLayout(CVKDevice* pDevice, uint32_t set);
	virtual ~CVKDescriptorLayout(void);
	virtual void Release(void);


public:
	VkDescriptorSetLayout GetDescriptorLayout(void) const;
	const uint32_t* GetNumDescriptors(void) const;

public:
	bool Create(void);
	void Destroy(void);

public:
	bool SetUniformBlockBinding(uint32_t name, uint32_t binding);
	bool SetSampledImageBinding(uint32_t name, uint32_t binding);
	bool SetInputAttachmentBinding(uint32_t name, uint32_t binding);

public:
	uint32_t GetSetIndex(void) const;
	uint32_t GetUniformBlockBinding(uint32_t name) const;
	uint32_t GetSampledImageBinding(uint32_t name) const;
	uint32_t GetInputAttachmentBinding(uint32_t name) const;

public:
	bool IsUniformBlockValid(uint32_t name) const;
	bool IsSampledImageValid(uint32_t name) const;
	bool IsInputAttachmentValid(uint32_t name) const;
	bool IsCompatible(const CGfxDescriptorLayoutPtr ptrLayout) const;


private:
	uint32_t m_set;
	uint32_t m_numDescriptors[VK_DESCRIPTOR_TYPE_RANGE_SIZE];
	eastl::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> m_uniformBlockBindings; // [name, binding]
	eastl::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> m_sampledImageBindings; // [name, binding]
	eastl::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> m_inputAttachmentBindings; // [name, binding]

private:
	VkDescriptorSetLayout m_vkDescriptorLayout;

private:
	CVKDevice* m_pDevice;
};
