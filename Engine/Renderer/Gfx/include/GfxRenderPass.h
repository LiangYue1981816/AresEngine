#pragma once
#include "PreHeader.h"


typedef struct ClearValue {
	ClearValue(void)
	{
		stencil = 0;
		depth = 1.0f;
		color[0] = 0.0f;
		color[1] = 0.0f;
		color[2] = 0.0f;
		color[3] = 0.0f;
	}

	int stencil;
	float depth;
	float color[4];
} ClearValue;

typedef struct AttachmentInformation {
	AttachmentInformation(void)
	{
		format = GFX_PIXELFORMAT_UNDEFINED;
		samples = 1;
		bInvalidation = false;
		bClear = true;
	}

	void Set(GfxPixelFormat _format, int _samples, bool _bInvalidation, bool _bClear, float _clearDepth, int _clearStencil)
	{
		format = _format;
		samples = _samples;
		bInvalidation = _bInvalidation;
		bClear = _bClear;
		clearValue.depth = _clearDepth;
		clearValue.stencil = _clearStencil;
	}

	void Set(GfxPixelFormat _format, int _samples, bool _bInvalidation, bool _bClear, float _clearRed, float _clearGreen, float _clearBlue, float _clearAlpha)
	{
		format = _format;
		samples = _samples;
		bInvalidation = _bInvalidation;
		bClear = _bClear;
		clearValue.color[0] = _clearRed;
		clearValue.color[1] = _clearGreen;
		clearValue.color[2] = _clearBlue;
		clearValue.color[3] = _clearAlpha;
	}

	GfxPixelFormat format;
	int samples;
	bool bInvalidation;
	bool bClear;
	ClearValue clearValue;
} AttachmentInformation;

typedef struct SubpassInformation {
	SubpassInformation(void)
	{
		depthStencilAttachment = -1;
	}

	void SetInputAttachment(int indexAttachment)
	{
		inputAttachments[indexAttachment] = indexAttachment;
	}

	void SetOutputAttachment(int indexAttachment)
	{
		outputAttachments[indexAttachment] = indexAttachment;
	}

	void SetResolveAttachment(int indexAttachment)
	{
		resolveAttachments[indexAttachment] = indexAttachment;
	}

	void SetPreserveAttachment(int indexAttachment)
	{
		preserveAttachments[indexAttachment] = indexAttachment;
	}

	void SetDepthStencilAttachment(int indexAttachment)
	{
		depthStencilAttachment = indexAttachment;
	}

	eastl::unordered_map<int, int> inputAttachments;
	eastl::unordered_map<int, int> outputAttachments;
	eastl::unordered_map<int, int> resolveAttachments;
	eastl::unordered_map<int, int> preserveAttachments;
	int depthStencilAttachment;
} SubpassInformation;

class CGfxRenderPass : public CGfxResource
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
	virtual bool SetSubpassInputDepthStencilReference(int indexSubpass, int indexAttachment) = 0;
	virtual bool SetSubpassOutputColorReference(int indexSubpass, int indexAttachment) = 0;
	virtual bool SetSubpassOutputDepthStencilReference(int indexSubpass, int indexAttachment) = 0;
	virtual bool SetSubpassDepthStencilReference(int indexSubpass, int indexAttachment) = 0;
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
