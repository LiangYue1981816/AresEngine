#pragma once
#include "GfxRenderer.h"


class CALL_API CGfxUniformMat4
{
public:
	CGfxUniformMat4(bool bDynamic);
	virtual ~CGfxUniformMat4(void);


public:
	void SetValue(const float *value);
	void Apply(void);

public:
	const CGfxUniformBufferPtr& GetUniformBuffer(void) const;


private:
	bool m_bDirty;
	glm::mat4 m_value;

private:
	CGfxUniformBufferPtr m_ptrUniformBuffer;
};
