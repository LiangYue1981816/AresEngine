#pragma once
#include "GLES3Renderer.h"


class CGLES3UniformVec2 : public CGLES3UniformBase, public CGfxUniformVec2
{
	friend class CGLES3MaterialPass;


private:
	CGLES3UniformVec2(bool bDynamic);
	virtual ~CGLES3UniformVec2(void);
	virtual void Release(void);


public:
	void SetValue(float v0, float v1);
	void Apply(void);


private:
	bool m_bDirty;
	glm::vec2 m_value;
};
