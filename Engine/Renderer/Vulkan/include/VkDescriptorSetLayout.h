#pragma once
#include "VKRenderer.h"


typedef enum DescriptorSetType {
	DESCRIPTOR_SET_FRAME = 0,
	DESCRIPTOR_SET_CAMERA,
	DESCRIPTOR_SET_PASS,
	DESCRIPTOR_SET_DRAW,
	DESCRIPTOR_SET_COUNT
} DescriptorSetType;


class CVKDescriptorSetLayout
{
	friend class CVKPipeline;


private:
	CVKDescriptorSetLayout(CVKDevice *pDevice, uint32_t set);
	virtual ~CVKDescriptorSetLayout(void);


public:
	uint32_t GetSet(void) const;
	VkDescriptorSetLayout GetLayout(void) const;

public:
	const uint32_t* GetNumDescriptors(void) const;

	bool IsTextureValid(uint32_t name) const;
	bool IsUniformBlockValid(uint32_t name) const;

	VkDescriptorSetLayoutBinding GetTextureBinding(uint32_t name) const;
	VkDescriptorSetLayoutBinding GetUniformBlockBinding(uint32_t name) const;

private:
	bool SetUniformBlockBinding(const char *szName, uint32_t binding, VkShaderStageFlags flags);
	bool SetSampledImageBinding(const char *szName, uint32_t binding, VkShaderStageFlags flags);
	bool SetInputAttachmentBinding(const char *szName, uint32_t binding, VkShaderStageFlags flags);

private:
	bool Create(void);
	void Destroy(void);


private:
	VkDescriptorSetLayout m_vkDescriptorSetLayout;

private:
	uint32_t m_set;
	uint32_t m_numDescriptors[VK_DESCRIPTOR_TYPE_RANGE_SIZE];
	eastl::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> m_uniformBlockBindings;    // [name, binding]
	eastl::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> m_sampledImageBindings;    // [name, binding]
	eastl::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> m_inputAttachmentBindings; // [name, binding]

private:
	CVKDevice *m_pDevice;
};
