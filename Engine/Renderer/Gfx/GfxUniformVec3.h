#pragma once
#include "GfxRenderer.h"


class CGfxUniformVec3 : public CGfxResource
{
public:
	CGfxUniformVec3(void)
	{

	}
	virtual ~CGfxUniformVec3(void)
	{

	}


public:
	virtual void SetValue(float v0, float v1, float v2) = 0;
	virtual void Apply(void) = 0;
};
