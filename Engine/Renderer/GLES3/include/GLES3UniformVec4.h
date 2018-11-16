#pragma once
#include "GLES3Renderer.h"


class CGLES3UniformVec4 : public CGLES3UniformBase, public CGfxUniformVec4
{
	friend class CGLES3MaterialPass;


private:
	CGLES3UniformVec4(bool bDynamic);
	virtual ~CGLES3UniformVec4(void);
	virtual void Release(void);


public:
	void SetValue(float v0, float v1, float v2, float v3);
	void Apply(void);


private:
	bool m_bDirty;
	glm::vec4 m_value;
};
