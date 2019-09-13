#pragma once
#include "PreHeader.h"


typedef struct ClearValue {
	int stencil = 0;
	float depth = 1.0f;
	float color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
} ClearValue;

typedef struct AttachmentInformation {
	GfxPixelFormat format = GFX_PIXELFORMAT_UNDEFINED;
	int samples = 1;
	bool bInvalidation = false;
	bool bClear = true;
	ClearValue clearValue;
} AttachmentInformation;

typedef struct SubpassInformation {
	int depthStencilAttachment = -1;
	eastl::unordered_map<int, int> inputAttachments;
	eastl::unordered_map<int, int> outputAttachments;
	eastl::unordered_map<int, int> resolveAttachments;
	eastl::unordered_map<int, int> preserveAttachments;
} SubpassInformation;

class CALL_API CGfxRenderPass : public CGfxResource
{
public:
	CGfxRenderPass(uint32_t name, int numAttachments, int numSubpasses)
		: m_name(name)
	{

	}
	virtual ~CGfxRenderPass(void)
	{

	}

	uint32_t GetName(void) const
	{
		return m_name;
	}


public:
	virtual bool Create(void) = 0;
	virtual void Destroy(bool bClear = true) = 0;

public:
	virtual bool SetColorAttachment(int indexAttachment, GfxPixelFormat format, int samples, bool bInvalidation, bool bClear, float red = 0.0f, float green = 0.0f, float blue = 0.0f, float alpha = 0.0f) = 0;
	virtual bool SetDepthStencilAttachment(int indexAttachment, GfxPixelFormat format, int samples, bool bInvalidation, bool bClear, float depth = 1.0f, int stencil = 0) = 0;

	virtual bool SetSubpassInputColorReference(int indexSubpass, int indexAttachment) = 0;
	virtual bool SetSubpassOutputColorReference(int indexSubpass, int indexAttachment) = 0;
	virtual bool SetSubpassOutputDepthStencilReference(int indexSubpass, int indexAttachment) = 0;
	virtual bool SetSubpassResolveReference(int indexSubpass, int indexAttachment) = 0;
	virtual bool SetSubpassPreserveReference(int indexSubpass, int indexAttachment) = 0;

public:
	virtual uint32_t GetAttachmentCount(void) const = 0;
	virtual const AttachmentInformation* GetAttachments(void) const = 0;
	virtual const AttachmentInformation* GetAttachment(int indexAttachment) const = 0;

	virtual uint32_t GetSubpassCount(void) const = 0;
	virtual uint32_t GetSubpassInputAttachmentCount(int indexSubpass) const = 0;
	virtual uint32_t GetSubpassOutputAttachmentCount(int indexSubpass) const = 0;
	virtual const SubpassInformation* GetSubpass(int indexSubpass) const = 0;


private:
	uint32_t m_name;
};
