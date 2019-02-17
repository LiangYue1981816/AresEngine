#pragma once
#include "GfxRenderer.h"


typedef struct BufferRange {
	uint32_t offset;
	uint32_t range;
} BufferRange;

class CALL_API CGfxSprivCross
{
public:
	CGfxSprivCross(void);
	virtual ~CGfxSprivCross(void);


public:
	bool Load(const uint32_t *words, size_t numWords, uint32_t version = 450);

	const eastl::string& GetSource(void) const;
	const eastl::unordered_map<eastl::string, BufferRange>& GetPushConstantRanges(void) const;
	const eastl::unordered_map<eastl::string, eastl::unordered_map<uint32_t, uint32_t>>& GetUniformBlockBindings(void) const;
	const eastl::unordered_map<eastl::string, eastl::unordered_map<uint32_t, uint32_t>>& GetSampledImageBindings(void) const;
	const eastl::unordered_map<eastl::string, eastl::unordered_map<uint32_t, uint32_t>>& GetInputAttachmentBindings(void) const;


private:
	eastl::string m_source;
	eastl::unordered_map<eastl::string, BufferRange> m_pushConstantRanges;
	eastl::unordered_map<eastl::string, eastl::unordered_map<uint32_t, uint32_t>> m_uniformBlockBindings;
	eastl::unordered_map<eastl::string, eastl::unordered_map<uint32_t, uint32_t>> m_sampledImageBindings;
	eastl::unordered_map<eastl::string, eastl::unordered_map<uint32_t, uint32_t>> m_inputAttachmentBindings;
};
