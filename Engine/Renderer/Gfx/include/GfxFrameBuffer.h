#pragma once
#include "GfxRenderer.h"


class CALL_API CGfxFrameBuffer : public CGfxResource
{
public:
	CGfxFrameBuffer(int width, int height)
		: m_width(width)
		, m_height(height)
	{

	}
	virtual ~CGfxFrameBuffer(void)
	{

	}


public:
	virtual bool SetAttachmentTexture(int indexAttachment, CGfxRenderTexturePtr &ptrAttachmentTexture) = 0;
	virtual CGfxRenderTexturePtr GetAttachmentTexture(int indexAttachment) const = 0;

public:
	int GetWidth(void) const
	{
		return m_width;
	}

	int GetHeight(void) const
	{
		return m_height;
	}


protected:
	int m_width;
	int m_height;
};
