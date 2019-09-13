#pragma once
#include "PreHeader.h"


class CALL_API CGfxFrameBuffer : public CGfxResource
{
public:
	CGfxFrameBuffer(int width, int height, int numAttachments)
	{

	}
	virtual ~CGfxFrameBuffer(void)
	{

	}


public:
	virtual int GetWidth(void) const = 0;
	virtual int GetHeight(void) const = 0;

public:
	virtual bool Create(const CGfxRenderPassPtr ptrRenderPass) = 0;
	virtual void Destroy(bool bClear = true) = 0;

public:
	virtual bool SetAttachmentTexture(int indexAttachment, const CGfxRenderTexturePtr ptrAttachmentTexture) = 0;
	virtual const CGfxRenderTexturePtr GetAttachmentTexture(int indexAttachment) const = 0;
};
