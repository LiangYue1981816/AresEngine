#pragma once
#include "PreHeader.h"


typedef enum RenderSolution {
	RENDER_SOLUTION_DEFAULT = 0,
	RENDER_SOLUTION_DEFERRED,
	RENDER_SOLUTION_FORWARD,
	RENDER_SOLUTION_FORWARD_PLUS,
	RENDER_SOLUTION_COUNT
} RenderSolution;


class CALL_API CRenderSolutionBase
{
	friend class CEngine;


protected:
	CRenderSolutionBase(void);
	virtual ~CRenderSolutionBase(void);


protected:
	virtual void CreateFrameBuffer(void) = 0;
	virtual void DestroyFrameBuffer(void) = 0;

	virtual void CreateFrameBufferMSAA(int samples) = 0;
	virtual void DestroyFrameBufferMSAA(void) = 0;

public:
	virtual void SetEnableMSAA(bool bEnable, int samples = 4) = 0;

public:
	virtual void Render(int indexQueue, float deltaTime) = 0;
	virtual void Present(int indexQueue) = 0;
	virtual void Clearup(int indexQueue) = 0;

public:
	CGfxCamera* GetMainCamera(void) const;
	CGfxCamera* GetShadowCamera(void) const;

public:
	void SetTime(float t, float dt);

	void SetShadowOrtho(float left, float right, float bottom, float top, float zNear, float zFar);
	void SetShadowLookat(float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz);
	void SetShadowRange(float range);
	void SetShadowResolution(float resolution);

	void SetLightFactor(float ambientLightFactor, float pointLightFactor, float directLightFactor, float envLightFactor);

	void SetAmbientLightSH(float shRed[9], float shGreen[9], float shBlue[9]);
	void SetAmbientLightRotation(float angle, float axisx, float axisy, float axisz);

	void SetMainPointLightColor(float red, float green, float blue);
	void SetMainPointLightPosition(float posx, float posy, float posz, float radius);
	void SetMainPointLightAttenuation(float linear, float square, float constant);

	void SetMainDirectLightColor(float red, float green, float blue);
	void SetMainDirectLightDirection(float dirx, float diry, float dirz);

	void SetFogColor(float red, float green, float blue);
	void SetFogHeightDensity(float startHeight, float endHeight, float density);
	void SetFogDistanceDensity(float startDistance, float endDistance, float density);


protected:
	CGfxCamera *m_pMainCamera;
	CGfxCamera *m_pShadowCamera;
	CGfxUniformEngine *m_pUniformEngine;
};
