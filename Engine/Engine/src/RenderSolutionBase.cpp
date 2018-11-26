#include "EngineHeader.h"


CRenderSolutionBase::CRenderSolutionBase(void)
	: m_pMainCamera(nullptr)
	, m_pShadowCamera(nullptr)
	, m_pUniformEngine(nullptr)
{
	m_pMainCamera = new CGfxCamera;
	m_pShadowCamera = new CGfxCamera;
	m_pUniformEngine = new CGfxUniformEngine(true);
}

CRenderSolutionBase::~CRenderSolutionBase(void)
{
	delete m_pMainCamera;
	delete m_pShadowCamera;
	delete m_pUniformEngine;
}

CGfxCamera* CRenderSolutionBase::GetMainCamera(void) const
{
	return m_pMainCamera;
}

CGfxCamera* CRenderSolutionBase::GetShadowCamera(void) const
{
	return m_pShadowCamera;
}

void CRenderSolutionBase::SetTime(float t, float dt)
{
	m_pUniformEngine->SetTime(t, dt);
}

void CRenderSolutionBase::SetShadowOrtho(float left, float right, float bottom, float top, float zNear, float zFar)
{
	m_pUniformEngine->SetShadowOrtho(left, right, bottom, top, zNear, zFar);
}

void CRenderSolutionBase::SetShadowLookat(float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz)
{
	m_pUniformEngine->SetShadowLookat(eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz);
}

void CRenderSolutionBase::SetShadowRange(float range)
{
	m_pUniformEngine->SetShadowRange(range);
}

void CRenderSolutionBase::SetShadowResolution(float resolution)
{
	m_pUniformEngine->SetShadowResolution(resolution);
}

void CRenderSolutionBase::SetLightFactor(float ambientLightFactor, float pointLightFactor, float directLightFactor, float envLightFactor)
{
	m_pUniformEngine->SetLightFactor(ambientLightFactor, pointLightFactor, directLightFactor, envLightFactor);
}

void CRenderSolutionBase::SetAmbientLightSH(float shRed[9], float shGreen[9], float shBlue[9])
{
	m_pUniformEngine->SetAmbientLightSH(shRed, shGreen, shBlue);
}

void CRenderSolutionBase::SetAmbientLightRotation(float angle, float axisx, float axisy, float axisz)
{
	m_pUniformEngine->SetAmbientLightRotation(angle, axisx, axisy, axisz);
}

void CRenderSolutionBase::SetMainPointLightColor(float red, float green, float blue)
{
	m_pUniformEngine->SetPointLightColor(red, green, blue);
}

void CRenderSolutionBase::SetMainPointLightPosition(float posx, float posy, float posz, float radius)
{
	m_pUniformEngine->SetPointLightPosition(posx, posy, posz, radius);
}

void CRenderSolutionBase::SetMainPointLightAttenuation(float linear, float square, float constant)
{
	m_pUniformEngine->SetPointLightAttenuation(linear, square, constant);
}

void CRenderSolutionBase::SetMainDirectLightColor(float red, float green, float blue)
{
	m_pUniformEngine->SetDirectLightColor(red, green, blue);
}

void CRenderSolutionBase::SetMainDirectLightDirection(float dirx, float diry, float dirz)
{
	m_pUniformEngine->SetDirectLightDirection(dirx, diry, dirz);
}

void CRenderSolutionBase::SetFogColor(float red, float green, float blue)
{
	m_pUniformEngine->SetFogColor(red, green, blue);
}

void CRenderSolutionBase::SetFogHeightDensity(float startHeight, float endHeight, float density)
{
	m_pUniformEngine->SetFogHeightDensity(startHeight, endHeight, density);
}

void CRenderSolutionBase::SetFogDistanceDensity(float startDistance, float endDistance, float density)
{
	m_pUniformEngine->SetFogDistanceDensity(startDistance, endDistance, density);
}
