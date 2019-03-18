#pragma once
#include "GfxRenderer.h"


class CALL_API CGfxDescriptorLayout : public CGfxResource
{
public:
	CGfxDescriptorLayout(uint32_t set)
	{

	}
	virtual ~CGfxDescriptorLayout(void)
	{

	}


public:
	virtual uint32_t GetSetIndex(void) const = 0;
	virtual HANDLE GetDescriptorLayout(void) const = 0;

public:
	virtual bool Create(void) = 0;
	virtual void Destroy(void) = 0;

public:
	virtual bool SetUniformLocation(uint32_t name, uint32_t location, uint32_t offset, uint32_t size) = 0;
	virtual bool SetUniformBlockBinding(uint32_t name, uint32_t binding) = 0;
	virtual bool SetSampledImageBinding(uint32_t name, uint32_t binding) = 0;
	virtual bool SetInputAttachmentBinding(uint32_t name, uint32_t binding) = 0;

public:
	virtual uint32_t GetTextureBinding(uint32_t name) const = 0;
	virtual uint32_t GetUniformBlockBinding(uint32_t name) const = 0;
	virtual uint32_t GetUniformLocation(uint32_t name) const = 0;

public:
	virtual bool IsCompatible(const CGfxDescriptorLayout *pLayout) const = 0;
};
