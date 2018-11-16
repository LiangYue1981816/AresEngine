#pragma once
#include "GLES3Renderer.h"


class CGLES3UniformMat4 : public CGLES3UniformBase, public CGfxUniformMat4
{
	friend class CGLES3MaterialPass;


private:
	CGLES3UniformMat4(bool bDynamic);
	virtual ~CGLES3UniformMat4(void);
	virtual void Release(void);


public:
	void SetValue(const float *value);
	void Apply(void);


private:
	bool m_bDirty;
	glm::mat4 m_value;
};
