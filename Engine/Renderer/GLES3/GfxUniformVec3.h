#pragma once
#include "GfxRenderer.h"


class CGfxUniformVec3
{
public:
	CGfxUniformVec3(void);
	virtual ~CGfxUniformVec3(void);


public:
	void SetValue(float x, float y, float z);
	void Apply(void);

public:
	uint32_t GetBuffer(void) const;
	uint32_t GetSize(void) const;


private:
	bool m_bDirty;
	glm::vec3 m_value;
	CGfxUniformBuffer *m_pUniformBuffer;
};
