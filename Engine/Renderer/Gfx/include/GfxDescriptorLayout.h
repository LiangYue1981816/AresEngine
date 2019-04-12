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
	virtual bool Create(void) = 0;
	virtual void Destroy(void) = 0;

public:
	virtual bool SetUniformBlockBinding(uint32_t name, uint32_t binding) = 0;
	virtual bool SetSampledImageBinding(uint32_t name, uint32_t binding) = 0;
	virtual bool SetInputAttachmentBinding(uint32_t name, uint32_t binding) = 0;

public:
	virtual uint32_t GetSetIndex(void) const = 0;
	virtual uint32_t GetUniformBlockBinding(uint32_t name) const = 0;
	virtual uint32_t GetSampledImageBinding(uint32_t name) const = 0;

public:
	virtual bool IsUniformBlockValid(uint32_t name) const = 0;
	virtual bool IsSampledImageValid(uint32_t name) const = 0;
	virtual bool IsCompatible(const CGfxDescriptorLayoutPtr ptrLayout) const = 0;
};
