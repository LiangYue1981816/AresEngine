#pragma once
#include "GfxRenderer.h"


class CGfxUniformMat4 : public CGfxResource
{
public:
	CGfxUniformMat4(void)
	{

	}
	virtual ~CGfxUniformMat4(void)
	{

	}


public:
	virtual void SetValue(const float *value) = 0;
	virtual void Apply(void) = 0;
};
