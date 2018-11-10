#pragma once
#include "GLES3Renderer.h"


class CGLES3UniformVec3 : public CGLES3UniformBase, public CGfxUniformVec3
{
	friend class CGLES3MaterialPass;


private:
	CGLES3UniformVec3(bool bDynamic);
	virtual ~CGLES3UniformVec3(void);
	virtual void Release(void);


public:
	void SetValue(float v0, float v1, float v2);
	void Apply(void);


private:
	bool m_bDirty;
	glm::vec3 m_value;
};
