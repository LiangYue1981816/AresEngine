#include "EngineHeader.h"


CCamera::CCamera(void)
	: m_pCamera(nullptr)
	, m_pRenderQueue(nullptr)
	, m_pCameraUniform(nullptr)
{
	m_pCamera = new CGfxCamera;
	m_pRenderQueue = new CGfxRenderQueue;
	m_pCameraUniform = new CGfxUniformCamera;
}

CCamera::~CCamera(void)
{
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

void CCamera::SetCameraScissor(float x, float y, float width, float height)
{
	m_pCamera->SetScissor(x, y, width, height);
}

void CCamera::SetCameraViewport(float x, float y, float width, float height)
{
	m_pCamera->SetViewport(x, y, width, height);
	m_pCameraUniform->SetScreen(width, height);
}

void CCamera::SetCameraPerspective(float fovy, float aspect, float zNear, float zFar)
{
	m_pCamera->SetPerspective(fovy, aspect, zNear, zFar);
	m_pCameraUniform->SetPerspective(fovy, aspect, zNear, zFar);
}

void CCamera::SetCameraOrtho(float left, float right, float bottom, float top, float zNear, float zFar)
{
	m_pCamera->SetOrtho(left, right, bottom, top, zNear, zFar);
	m_pCameraUniform->SetOrtho(left, right, bottom, top, zNear, zFar);
}

void CCamera::SetCameraLookat(float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz)
{
	m_pCamera->SetLookat(eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz);
	m_pCameraUniform->SetLookat(eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz);
}

void CCamera::Update(void)
{
	SceneManager()->UpdateCamera(m_pCamera, m_pRenderQueue);
}

void CCamera::Render(void)
{

}
