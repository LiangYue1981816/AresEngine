#pragma once
#include "GfxRenderer.h"


class CGfxUniformMat4
{
public:
	CGfxUniformMat4(void);
	virtual ~CGfxUniformMat4(void);


public:
	void SetValue(const float *matrix);
	void Apply(void);

public:
	uint32_t GetBuffer(void) const;
	uint32_t GetSize(void) const;


private:
	bool m_bDirty;
	glm::mat4 m_value;
	CGfxUniformBuffer *m_pUniformBuffer;
};
