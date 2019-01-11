#pragma once
#include "GfxRenderer.h"


class CALL_API CGfxMaterial : public CGfxResource
{
public:
	CGfxMaterial(uint32_t name)
	{

	}
	virtual ~CGfxMaterial(void)
	{

	}


public:
	virtual uint32_t GetName(void) const = 0;

public:
	virtual CGfxMaterialPass* GetPass(uint32_t name) = 0;

public:
	virtual bool CreatePass(uint32_t name) = 0;
	virtual void Destroy(void) = 0;
};
