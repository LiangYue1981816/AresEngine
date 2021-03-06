#pragma once
#include "PreHeader.h"


class CALL_API CGfxUniformVec3
{
public:
	CGfxUniformVec3(void);
	virtual ~CGfxUniformVec3(void);


public:
	const glm::vec3& GetValue(void) const;
	const CGfxUniformBufferPtr GetUniformBuffer(void) const;

public:
	void SetValue(float v0, float v1, float v2);
	void Apply(void);


private:
	bool m_bDirty;
	glm::vec3 m_value;

private:
	CGfxUniformBufferPtr m_ptrUniformBuffer;
};
