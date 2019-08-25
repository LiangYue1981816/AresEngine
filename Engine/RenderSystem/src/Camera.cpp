#include "EngineHeader.h"


CCamera::CCamera(void)
	: m_pCamera(nullptr)
	, m_pRenderQueue(nullptr)
	, m_pCameraUniform(nullptr)

	, m_pPassDefault(nullptr)
	, m_pPassForwardLighting(nullptr)
	, m_pPassShadow(nullptr)
	, m_pPassShadowBlur(nullptr)
{
	m_pCamera = new CGfxCamera;
	m_pRenderQueue = new CGfxRenderQueue;
	m_pCameraUniform = new CGfxUniformCamera;

	m_pPassDefault = new CPassDefault(this, RenderSystem());
	m_pPassForwardLighting = new CPassForwardLighting(this, RenderSystem());
	m_pPassShadow = new CPassShadow(this, RenderSystem());
	m_pPassShadowBlur = new CPassShadowBlur(RenderSystem());
}

CCamera::~CCamera(void)
{
	delete m_pPassDefault;
	delete m_pPassForwardLighting;
	delete m_pPassShadow;
	delete m_pPassShadowBlur;

	delete m_pCamera;
	delete m_pRenderQueue;
	delete m_pCameraUniform;
}

CGfxCamera* CCamera::GetCamera(void) const
{
	return m_pCamera;
}

CGfxRenderQueue* CCamera::GetRenderQueue(void) const
{
	return m_pRenderQueue;
}

CGfxUniformCamera* CCamera::GetCameraUniform(void) const
{
	return m_pCameraUniform;
}

CPassDefault* CCamera::GetPassDefault(void) const
{
	return m_pPassDefault;
}

CPassForwardLighting* CCamera::GetPassForwardLighting(void) const
{
	return m_pPassForwardLighting;
}

CPassShadow* CCamera::GetPassShadow(void) const
{
	return m_pPassShadow;
}

CPassShadowBlur* CCamera::GetPassShadowBlur(void) const
{
	return m_pPassShadowBlur;
}

void CCamera::SetScissor(float x, float y, float width, float height)
{
	m_pCamera->SetScissor(x, y, width, height);
}

void CCamera::SetViewport(float x, float y, float width, float height)
{
	m_pCamera->SetViewport(x, y, width, height);
	m_pCameraUniform->SetScreen(width, height);
}

void CCamera::SetPerspective(float fovy, float aspect, float zNear, float zFar)
{
	m_pCamera->SetPerspective(fovy, aspect, zNear, zFar);
	m_pCameraUniform->SetPerspective(fovy, aspect, zNear, zFar);
}

void CCamera::SetOrtho(float left, float right, float bottom, float top, float zNear, float zFar)
{
	m_pCamera->SetOrtho(left, right, bottom, top, zNear, zFar);
	m_pCameraUniform->SetOrtho(left, right, bottom, top, zNear, zFar);
}

void CCamera::SetLookat(float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz)
{
	m_pCamera->SetLookat(eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz);
	m_pCameraUniform->SetLookat(eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz);
}
