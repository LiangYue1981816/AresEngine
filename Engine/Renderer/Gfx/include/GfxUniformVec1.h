#pragma once
#include "PreHeader.h"


class CALL_API CGfxUniformVec1
{
public:
	CGfxUniformVec1(void);
	virtual ~CGfxUniformVec1(void);


public:
	const float GetValue(void) const;
	const CGfxUniformBufferPtr GetUniformBuffer(void) const;

public:
	void SetValue(float v0);
	void Apply(void);


private:
	bool m_bDirty;
	float m_value;

private:
	CGfxUniformBufferPtr m_ptrUniformBuffer;
};
