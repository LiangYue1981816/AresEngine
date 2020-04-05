#include "EngineHeader.h"
#include "RenderHeader.h"


CCamera::CCamera(void)
	: m_pRenderQueue(nullptr)
	, m_pCamera(nullptr)
	, m_pCameraUniform(nullptr)
{
	m_pRenderQueue = new CRenderQueue;
	m_pCamera = new CGfxCamera;
	m_pCameraUniform = new CGfxUniformCamera;
}

CCamera::~CCamera(void)
{
	delete m_pRenderQueue;
	delete m_pCamera;
	delete m_pCameraUniform;
}

CRenderQueue* CCamera::GetRenderQueue(void) const
{
	return m_pRenderQueue;
}

CGfxCamera* CCamera::GetCamera(void) const
{
	return m_pCamera;
}

CGfxUniformCamera* CCamera::GetCameraUniform(void) const
{
	return m_pCameraUniform;
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

void CCamera::Update(CTaskPool& taskPool, CTaskGraph& taskGraph, uint32_t mask, bool bComputeLOD) const
{
	SceneManager()->UpdateCamera(taskPool, taskGraph, m_pCamera, m_pRenderQueue, mask, bComputeLOD);
}
