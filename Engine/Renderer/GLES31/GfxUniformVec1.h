#pragma once
#include "GfxRenderer.h"


class CGfxUniformVec1
{
public:
	CGfxUniformVec1(void);
	virtual ~CGfxUniformVec1(void);


public:
	void SetValue(float value);
	void Apply(void);

public:
	GLuint GetSize(void) const;
	GLuint GetBuffer(void) const;


private:
	bool m_bDirty;
	float m_value;
	CGfxUniformBuffer *m_pUniformBuffer;
};
