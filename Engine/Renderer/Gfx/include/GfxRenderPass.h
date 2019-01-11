#pragma once
#include "GfxRenderer.h"


typedef struct AttachmentInformation {
	GfxPixelFormat pixelFormat = GFX_PIXELFORMAT_UNDEFINED;
	int samples = 1;
	bool bInvalidation = false;
	bool bClear = true;
	int stencil = 0;
	float depth = 1.0f;
	float color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
} AttachmentInformation;

typedef struct SubPassInformation {
	int depthStencilAttachment = -1;
	eastl::unordered_map<int, eastl::string> inputAttachments;
	eastl::unordered_map<int, int> outputAttachments;
	eastl::unordered_map<int, int> resolveAttachments;
	eastl::unordered_map<int, int> preserveAttachments;
} SubPassInformation;


class CALL_API CGfxRenderPass : public CGfxResource
{
public:
	CGfxRenderPass(int numAttachments, int numSubpasses)
	{

	}
	virtual ~CGfxRenderPass(void)
	{

	}


public:
	virtual HANDLE GetRenderPass(void) const = 0;

public:
	virtual bool SetColorAttachment(int indexAttachment, GfxPixelFormat pixelFormat, int samples, bool bInvalidation, bool bClear, float red = 0.0f, float green = 0.0f, float blue = 0.0f, float alpha = 0.0f) = 0;
	virtual bool SetDepthStencilAttachment(int indexAttachment, GfxPixelFormat pixelFormat, int samples, bool bInvalidation, bool bClear, float depth = 1.0f, int stencil = 0) = 0;

	virtual bool SetSubpassInputColorReference(int indexSubPass, int indexAttachment, const char *szName) = 0;
	virtual bool SetSubpassOutputColorReference(int indexSubPass, int indexAttachment) = 0;
	virtual bool SetSubpassOutputDepthStencilReference(int indexSubPass, int indexAttachment) = 0;
	virtual bool SetSubpassResolveReference(int indexSubPass, int indexAttachment) = 0;
	virtual bool SetSubpassPreserveReference(int indexSubPass, int indexAttachment) = 0;

public:
	virtual uint32_t GetAttachmentCount(void) const = 0;
	virtual const AttachmentInformation* GetAttachments(void) const = 0;
	virtual const AttachmentInformation* GetAttachment(int indexAttachment) const = 0;

	virtual uint32_t GetSubPassCount(void) const = 0;
	virtual uint32_t GetSubpassInputAttachmentCount(int indexSubPass) const = 0;
	virtual uint32_t GetSubpassOutputAttachmentCount(int indexSubPass) const = 0;
	virtual const SubPassInformation* GetSubPass(int indexSubPass) const = 0;

public:
	virtual bool Create(void) = 0;
	virtual void Destroy(void) = 0;
};
