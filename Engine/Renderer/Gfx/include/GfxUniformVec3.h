#pragma once
#include "GfxRenderer.h"


class CALL_API CGfxUniformVec3
{
public:
	CGfxUniformVec3(bool bDynamic);
	virtual ~CGfxUniformVec3(void);


public:
	void SetValue(float v0, float v1, float v2);
	void Apply(void);

public:
	const CGfxUniformBufferPtr& GetUniformBuffer(void) const;


private:
	bool m_bDirty;
	glm::vec3 m_value;

private:
	CGfxUniformBufferPtr m_ptrUniformBuffer;
};
