#pragma once
#include "PreHeader.h"


typedef struct PushConstantRange {
	uint32_t offset = 0;
	uint32_t range = 0;
} PushConstantRange;

typedef struct DescriptorSetBinding {
	uint32_t set = 0;
	uint32_t binding = 0;
} DescriptorSetBinding;

typedef struct InputAttachmentBinding {
	uint32_t set = 0;
	uint32_t binding = 0;
	uint32_t indexInputAttachment = 0;
} InputAttachmentBinding;

class CALL_API CGfxSprivCross
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
	const eastl::unordered_map<eastl::string, DescriptorSetBinding>& GetSampledImageBindings(void) const;
	const eastl::unordered_map<eastl::string, InputAttachmentBinding>& GetInputAttachmentBindings(void) const;


private:
	eastl::string m_source;
	eastl::vector<eastl::string> m_vertexAttributes;
	eastl::unordered_map<eastl::string, PushConstantRange> m_pushConstantRanges;
	eastl::unordered_map<eastl::string, DescriptorSetBinding> m_uniformBlockBindings;
	eastl::unordered_map<eastl::string, DescriptorSetBinding> m_storageBlockBindings;
	eastl::unordered_map<eastl::string, DescriptorSetBinding> m_sampledImageBindings;
	eastl::unordered_map<eastl::string, InputAttachmentBinding> m_inputAttachmentBindings;
};
