#pragma once
#include "PreHeader.h"


typedef struct PushConstantRange {
	PushConstantRange(void)
	{
		offset = 0;
		range = 0;
	}

	void Set(uint32_t _offset, uint32_t _range)
	{
		offset = _offset;
		range = _range;
	}

	uint32_t offset;
	uint32_t range;
} PushConstantRange;

typedef struct DescriptorSetBinding {
	DescriptorSetBinding(void)
	{
		set = 0;
		binding = 0;
	}

	void Set(uint32_t _set, uint32_t _binding)
	{
		set = _set;
		binding = _binding;
	}

	uint32_t set;
	uint32_t binding;
} DescriptorSetBinding;

typedef struct InputAttachmentBinding {
	InputAttachmentBinding(void)
	{
		set = 0;
		binding = 0;
		indexInputAttachment = 0;
	}

	void Set(uint32_t _set, uint32_t _binding, uint32_t _indexInputAttachment)
	{
		set = _set;
		binding = _binding;
		indexInputAttachment = _indexInputAttachment;
	}

	uint32_t set = 0;
	uint32_t binding = 0;
	uint32_t indexInputAttachment = 0;
} InputAttachmentBinding;

class CGfxSprivCross
{
public:
	CGfxSprivCross(void);
	virtual ~CGfxSprivCross(void);


public:
	const eastl::string& Create(const uint32_t* words, size_t numWords, uint32_t version = 450);

public:
	const eastl::string& GetSource(void) const;
	const eastl::vector<eastl::string>& GetVertexAttributes(void) const;
	const eastl::unordered_map<eastl::string, PushConstantRange>& GetPushConstantRanges(void) const;
	const eastl::unordered_map<eastl::string, DescriptorSetBinding>& GetUniformBlockBindings(void) const;
	const eastl::unordered_map<eastl::string, DescriptorSetBinding>& GetStorageBlockBindings(void) const;
	const eastl::unordered_map<eastl::string, DescriptorSetBinding>& GetStorageImageBindings(void) const;
	const eastl::unordered_map<eastl::string, DescriptorSetBinding>& GetSampledImageBindings(void) const;
	const eastl::unordered_map<eastl::string, InputAttachmentBinding>& GetInputAttachmentBindings(void) const;


private:
	eastl::string m_source;
	eastl::vector<eastl::string> m_vertexAttributes;
	eastl::unordered_map<eastl::string, PushConstantRange> m_pushConstantRanges;
	eastl::unordered_map<eastl::string, DescriptorSetBinding> m_uniformBlockBindings;
	eastl::unordered_map<eastl::string, DescriptorSetBinding> m_storageBlockBindings;
	eastl::unordered_map<eastl::string, DescriptorSetBinding> m_storageImageBindings;
	eastl::unordered_map<eastl::string, DescriptorSetBinding> m_sampledImageBindings;
	eastl::unordered_map<eastl::string, InputAttachmentBinding> m_inputAttachmentBindings;
};
