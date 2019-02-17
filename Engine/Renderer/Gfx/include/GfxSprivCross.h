#pragma once
#include "GfxRenderer.h"


typedef struct PushConstantRange {
	uint32_t offset;
	uint32_t range;
} PushConstantRange;

typedef struct DescriptorSetBinding {
	uint32_t set;
	uint32_t binding;
} DescriptorSetBinding;

class CALL_API CGfxSprivCross
{
public:
	CGfxSprivCross(void);
	virtual ~CGfxSprivCross(void);


public:
	const eastl::string& Create(const uint32_t *words, size_t numWords, uint32_t version = 450);

public:
	const eastl::string& GetSource(void) const;
	const eastl::unordered_map<eastl::string, PushConstantRange>& GetPushConstantRanges(void) const;
	const eastl::unordered_map<eastl::string, DescriptorSetBinding>& GetUniformBlockBindings(void) const;
	const eastl::unordered_map<eastl::string, DescriptorSetBinding>& GetSampledImageBindings(void) const;
	const eastl::unordered_map<eastl::string, DescriptorSetBinding>& GetInputAttachmentBindings(void) const;


private:
	eastl::string m_source;
	eastl::unordered_map<eastl::string, PushConstantRange> m_pushConstantRanges;
	eastl::unordered_map<eastl::string, DescriptorSetBinding> m_uniformBlockBindings;
	eastl::unordered_map<eastl::string, DescriptorSetBinding> m_sampledImageBindings;
	eastl::unordered_map<eastl::string, DescriptorSetBinding> m_inputAttachmentBindings;
};
