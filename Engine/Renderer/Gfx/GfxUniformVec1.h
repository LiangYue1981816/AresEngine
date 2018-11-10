#pragma once
#include "GfxRenderer.h"


class CGfxUniformVec1 : public CGfxResource
{
public:
	CGfxUniformVec1(void)
	{

	}
	virtual ~CGfxUniformVec1(void)
	{

	}


public:
	virtual void SetValue(float v0) = 0;
	virtual void Apply(void) = 0;
};
