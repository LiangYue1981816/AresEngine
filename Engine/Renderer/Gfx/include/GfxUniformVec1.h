#pragma once
#include "GfxRenderer.h"


class CALL_API CGfxUniformVec1
{
public:
	CGfxUniformVec1(void);
	virtual ~CGfxUniformVec1(void);


public:
	void SetValue(float v0);
	void Apply(void);

public:
	const CGfxUniformBufferPtr& GetUniformBuffer(void) const;


private:
	bool m_bDirty;
	float m_value;

private:
	CGfxUniformBufferPtr m_ptrUniformBuffer;
};
