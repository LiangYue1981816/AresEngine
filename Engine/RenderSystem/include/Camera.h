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
	void SetCameraScissor(float x, float y, float width, float height);
	void SetCameraViewport(float x, float y, float width, float height);
	void SetCameraPerspective(float fovy, float aspect, float zNear, float zFar);
	void SetCameraOrtho(float left, float right, float bottom, float top, float zNear, float zFar);
	void SetCameraLookat(float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz);

public:
	void Render(CTaskGraph& taskGraph);


private:
	CGfxCamera* m_pCamera;
	CGfxRenderQueue* m_pRenderQueue;
	CGfxUniformCamera* m_pCameraUniform;
};
