#pragma once
#include "PreHeader.h"


class CALL_API CCamera
{
	friend class CEngine;


private:
	CCamera(void);
	virtual ~CCamera(void);


public:
	CGfxCamera* GetCamera(void) const;
	CGfxRenderQueue* GetRenderQueue(void) const;
	CGfxUniformCamera* GetCameraUniform(void) const;

public:
	void SetScissor(float x, float y, float width, float height);
	void SetViewport(float x, float y, float width, float height);
	void SetPerspective(float fovy, float aspect, float zNear, float zFar);
	void SetOrtho(float left, float right, float bottom, float top, float zNear, float zFar);
	void SetLookat(float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz);


private:
	CGfxCamera* m_pCamera;
	CGfxRenderQueue* m_pRenderQueue;
	CGfxUniformCamera* m_pCameraUniform;
};
