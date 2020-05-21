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

void CCamera::SetDepthRange(float minz, float maxz)
{
	m_pCamera->SetDepthRange(minz, maxz);
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

const float CCamera::GetZNear(void) const
{
	return m_pCamera->GetZNear();
}

const float CCamera::GetZFar(void) const
{
	return m_pCamera->GetZFar();
}

const float CCamera::GetScissorX(void) const
{
	return m_pCamera->GetScissor().x;
}

const float CCamera::GetScissorY(void) const
{
	return m_pCamera->GetScissor().y;
}

const float CCamera::GetScissorWidth(void) const
{
	return m_pCamera->GetScissor().z;
}

const float CCamera::GetScissorHeight(void) const
{
	return m_pCamera->GetScissor().w;
}

const float CCamera::GetViewportX(void) const
{
	return m_pCamera->GetViewport().x;
}

const float CCamera::GetViewportY(void) const
{
	return m_pCamera->GetViewport().y;
}

const float CCamera::GetViewportWidth(void) const
{
	return m_pCamera->GetViewport().z;
}

const float CCamera::GetViewportHeight(void) const
{
	return m_pCamera->GetViewport().w;
}

const glm::vec3& CCamera::GetPosition(void) const
{
	return m_pCamera->GetPosition();
}

const glm::vec3& CCamera::GetForwardDirection(void) const
{
	return m_pCamera->GetForwardDirection();
}

const glm::vec3& CCamera::GetUpDirection(void) const
{
	return m_pCamera->GetUpDirection();
}

const glm::mat4& CCamera::GetProjectionMatrix(void) const
{
	return m_pCamera->GetProjectionMatrix();
}

const glm::mat4& CCamera::GetViewMatrix(void) const
{
	return m_pCamera->GetViewMatrix();
}

const glm::mat4& CCamera::GetViewInverseMatrix(void) const
{
	return m_pCamera->GetViewInverseMatrix();
}

const glm::mat4& CCamera::GetViewInverseTransposeMatrix(void) const
{
	return m_pCamera->GetViewInverseTransposeMatrix();
}

glm::vec3 CCamera::WorldToScreen(const glm::vec3& world) const
{
	return m_pCamera->WorldToScreen(world);
}

glm::vec3 CCamera::ScreenToWorld(const glm::vec3& screen) const
{
	return m_pCamera->ScreenToWorld(screen);
}

bool CCamera::IsVisible(const glm::vec3& vertex) const
{
	return m_pCamera->IsVisible(vertex);
}

bool CCamera::IsVisible(const glm::aabb& aabb) const
{
	return m_pCamera->IsVisible(aabb);
}

bool CCamera::IsVisible(const glm::sphere& sphere) const
{
	return m_pCamera->IsVisible(sphere);
}

void CCamera::Update(CTaskPool& taskPool, CTaskGraph& taskGraph, uint32_t mask, bool bComputeLOD) const
{
	SceneManager()->UpdateCamera(taskPool, taskGraph, m_pCamera, m_pRenderQueue, mask, bComputeLOD);
}
