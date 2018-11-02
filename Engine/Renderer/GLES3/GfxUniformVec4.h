#pragma once
#include "GfxRenderer.h"


class CGfxUniformVec4
{
public:
	CGfxUniformVec4(void);
	virtual ~CGfxUniformVec4(void);


public:
	void SetValue(float x, float y, float z, float w);
	void Apply(void);

public:
	uint32_t GetBuffer(void) const;
	uint32_t GetSize(void) const;


private:
	bool m_bDirty;
	glm::vec4 m_value;
	CGfxUniformBuffer *m_pUniformBuffer;
};
