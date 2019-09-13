#pragma once
#include "PreHeader.h"


class CALL_API CGfxUniformMat4
{
public:
	CGfxUniformMat4(void);
	virtual ~CGfxUniformMat4(void);


public:
	const CGfxUniformBufferPtr GetUniformBuffer(void) const;

public:
	void SetValue(const float* value);
	void Apply(void);


private:
	bool m_bDirty;
	glm::mat4 m_value;

private:
	CGfxUniformBufferPtr m_ptrUniformBuffer;
};
