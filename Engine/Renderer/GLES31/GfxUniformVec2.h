#pragma once
#include "GfxRenderer.h"


class CGfxUniformVec2
{
public:
	CGfxUniformVec2(void);
	virtual ~CGfxUniformVec2(void);


public:
	void SetValue(float x, float y);
	void Apply(void);

public:
	GLuint GetBuffer(void) const;
	GLsizeiptr GetSize(void) const;


private:
	bool m_bDirty;
	glm::vec2 m_value;
	CGfxUniformBuffer *m_pUniformBuffer;
};
