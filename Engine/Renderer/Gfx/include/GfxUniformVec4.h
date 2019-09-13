#pragma once
#include "PreHeader.h"


class CALL_API CGfxUniformVec4
{
public:
	CGfxUniformVec4(void);
	virtual ~CGfxUniformVec4(void);


public:
	const glm::vec4& GetValue(void) const;
	const CGfxUniformBufferPtr GetUniformBuffer(void) const;

public:
	void SetValue(float v0, float v1, float v2, float v3);
	void Apply(void);


private:
	bool m_bDirty;
	glm::vec4 m_value;

private:
	CGfxUniformBufferPtr m_ptrUniformBuffer;
};
