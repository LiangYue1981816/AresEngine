#pragma once
#include "PreHeader.h"


class CALL_API CRenderSolutionBase
{
	friend class CRenderSolution;


protected:
	CRenderSolutionBase(void);
	virtual ~CRenderSolutionBase(void);


protected:
	virtual void CreateFrameBuffer(void) = 0;
	virtual void DestroyFrameBuffer(void) = 0;

	virtual void CreateFrameBufferMSAA(int samples) = 0;
	virtual void DestroyFrameBufferMSAA(void) = 0;

public:
	virtual void Create(int samples = 1) = 0;
	virtual void Destroy(void) = 0;

public:
	virtual void UpdateCamera(int indexQueue) = 0;
	virtual void Render(int indexQueue) = 0;


public:
	static void CreateInstance(void);
	static void DestroyInstance(void);

public:
	static void SetTime(float t, float dt);

	static void SetCameraScissor(float x, float y, float width, float height);
	static void SetCameraViewport(float x, float y, float width, float height);
	static void SetCameraPerspective(float fovy, float aspect, float zNear, float zFar);
	static void SetCameraOrtho(float left, float right, float bottom, float top, float zNear, float zFar);
	static void SetCameraLookat(float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz);

	static void SetShadowOrtho(float left, float right, float bottom, float top, float zNear, float zFar);
	static void SetShadowLookat(float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz);
	static void SetShadowRange(float range);
	static void SetShadowResolution(float resolution);

	static void SetLightFactor(float ambientLightFactor, float pointLightFactor, float directLightFactor, float envLightFactor);

	static void SetAmbientLightSH(float shRed[9], float shGreen[9], float shBlue[9]);
	static void SetAmbientLightRotation(float angle, float axisx, float axisy, float axisz);
	static void SetAmbientLightDirection(float dirx, float diry, float dirz);

	static void SetMainPointLightColor(float red, float green, float blue);
	static void SetMainPointLightPosition(float posx, float posy, float posz, float radius);
	static void SetMainPointLightAttenuation(float linear, float square, float constant);

	static void SetMainDirectLightColor(float red, float green, float blue);
	static void SetMainDirectLightDirection(float dirx, float diry, float dirz);

	static void SetFogColor(float red, float green, float blue);
	static void SetFogHeightDensity(float startHeight, float endHeight, float density);
	static void SetFogDistanceDensity(float startDistance, float endDistance, float density);


protected:
	CGfxRenderQueue* m_pMainQueue;
	CGfxRenderQueue* m_pLightQueue;
	CGfxRenderQueue* m_pShadowQueue;

protected:
	static CGfxEngine* m_pEngine;
	static CGfxCamera* m_pMainCamera;
	static CGfxCamera* m_pShadowCamera;
};
