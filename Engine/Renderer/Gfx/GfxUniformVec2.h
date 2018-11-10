#pragma once
#include "GfxRenderer.h"


class CGfxUniformVec2 : public CGfxResource
{
public:
	CGfxUniformVec2(void)
	{

	}
	virtual ~CGfxUniformVec2(void)
	{

	}


public:
	virtual void SetValue(float v0, float v1) = 0;
	virtual void Apply(void) = 0;
};
