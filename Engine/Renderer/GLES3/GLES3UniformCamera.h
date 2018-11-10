#pragma once
#include "GLES3Renderer.h"


class CGLES3UniformCamera : public CGLES3UniformBase, public CGfxUniformCamera
{
	friend class CGLES3UniformManager;


private:
	CGLES3UniformCamera(CGLES3UniformManager *pManager, bool bDynamic);
	virtual ~CGLES3UniformCamera(void);
	virtual void Release(void);


public:
	void SetScreen(float width, float height);

	void SetPerspective(float fovy, float aspect, float zNear, float zFar);
	void SetOrtho(float left, float right, float bottom, float top, float zNear, float zFar);
	void SetLookat(float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz);

	void Apply(void);


private:
	bool m_bDirty;
	uint32_t m_hash;
	Params m_params;
};
