#pragma once
#include "GfxRenderer.h"


class CALL_API CGfxUniformVec2
{
public:
	CGfxUniformVec2(void);
	virtual ~CGfxUniformVec2(void);


public:
	const glm::vec2& GetValue(void) const;
	const CGfxUniformBufferPtr GetUniformBuffer(void) const;

public:
	void SetValue(float v0, float v1);
	void Apply(void);


private:
	bool m_bDirty;
	glm::vec2 m_value;

private:
	CGfxUniformBufferPtr m_ptrUniformBuffer;
};
