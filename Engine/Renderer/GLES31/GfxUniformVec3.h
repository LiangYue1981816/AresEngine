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
	GLuint GetBuffer(void) const;
	GLsizeiptr GetSize(void) const;


private:
	bool m_bDirty;
	glm::vec3 m_value;
	CGfxUniformBuffer *m_pUniformBuffer;
};
