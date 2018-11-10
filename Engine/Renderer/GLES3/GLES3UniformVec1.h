#pragma once
#include "GLES3Renderer.h"


class CGLES3UniformVec1 : public CGLES3UniformBase, public CGfxUniformVec1
{
	friend class CGLES3MaterialPass;


private:
	CGLES3UniformVec1(bool bDynamic);
	virtual ~CGLES3UniformVec1(void);
	virtual void Release(void);


public:
	void SetValue(float v0);
	void Apply(void);


private:
	bool m_bDirty;
	float m_value;
};
