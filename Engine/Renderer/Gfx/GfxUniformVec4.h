#pragma once
#include "GfxRenderer.h"


class CALL_API CGfxUniformVec4 : public CGfxResource
{
public:
	CGfxUniformVec4(void)
	{

	}
	virtual ~CGfxUniformVec4(void)
	{

	}


public:
	virtual void SetValue(float v0, float v1, float v2, float v3) = 0;
	virtual void Apply(void) = 0;
};
