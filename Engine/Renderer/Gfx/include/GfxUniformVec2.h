#pragma once
#include "GfxRenderer.h"


class CALL_API CGfxUniformVec2
{
public:
	CGfxUniformVec2(bool bDynamic);
	virtual ~CGfxUniformVec2(void);


public:
	void SetValue(float v0, float v1);
	void Apply(void);

public:
	const CGfxUniformBufferPtr& GetUniformBuffer(void) const;


private:
	bool m_bDirty;
	glm::vec2 m_value;

private:
	CGfxUniformBufferPtr m_ptrUniformBuffer;
};
