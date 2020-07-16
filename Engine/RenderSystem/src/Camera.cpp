#include "EngineHeader.h"
#include "RenderHeader.h"


CCamera::CCamera(void)
	: m_pRenderQueue(nullptr)
	, m_pCamera(nullptr)
	, m_bDirty(false)
	, m_offset(0)
{
	m_pRenderQueue = new CRenderQueue;
	m_pCamera = new CGfxCamera;
	m_ptrUniformBuffer = GfxRenderer()->NewUniformBuffer(CGfxSwapChain::SWAPCHAIN_FRAME_COUNT * sizeof(m_params));
}

CCamera::~CCamera(void)
{
	delete m_pRenderQueue;
	delete m_pCamera;
}

const CCamera::Params& CCamera::GetParams(void) const
{
	return m_params;
}

const CGfxCamera* CCamera::GetCamera(void) const
{
	return m_pCamera;
}

CRenderQueue* CCamera::GetRenderQueue(void) const
{
	return m_pRenderQueue;
}

uint32_t CCamera::GetUniformBufferOffset(void) const
{
	return m_offset;
}

CGfxUniformBufferPtr CCamera::GetUniformBuffer(void) const
{
	return m_ptrUniformBuffer;
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

	m_bDirty = true;
	m_params.screen = glm::vec4(width, height, 1.0f / width, 1.0f / height);
}

void CCamera::SetPerspective(float fovy, float aspect, float zNear, float zFar)
{
	m_pCamera->SetPerspective(fovy, aspect, zNear, zFar);

	m_bDirty = true;
	m_params.depth = glm::vec4(zNear, zFar, zFar - zNear, 1.0f / (zFar - zNear));
	m_params.projectionMatrix = GfxRenderer()->GetBaseMatrix() * glm::perspective(glm::radians(fovy), aspect, zNear, zFar);
	m_params.projectionViewMatrix = m_params.projectionMatrix * m_params.viewMatrix;
	m_params.projectionInverseMatrix = glm::inverse(m_params.projectionMatrix);
	m_params.projectionViewInverseMatrix = glm::inverse(m_params.projectionViewMatrix);
}

void CCamera::SetOrtho(float left, float right, float bottom, float top, float zNear, float zFar)
{
	m_pCamera->SetOrtho(left, right, bottom, top, zNear, zFar);

	m_bDirty = true;
	m_params.depth = glm::vec4(zNear, zFar, zFar - zNear, 1.0f / (zFar - zNear));
	m_params.projectionMatrix = GfxRenderer()->GetBaseMatrix() * glm::ortho(left, right, bottom, top, zNear, zFar);
	m_params.projectionViewMatrix = m_params.projectionMatrix * m_params.viewMatrix;
	m_params.projectionInverseMatrix = glm::inverse(m_params.projectionMatrix);
	m_params.projectionViewInverseMatrix = glm::inverse(m_params.projectionViewMatrix);
}

void CCamera::SetLookat(float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz)
{
	m_pCamera->SetLookat(eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz);

	m_bDirty = true;
	m_params.viewMatrix = glm::lookAt(glm::vec3(eyex, eyey, eyez), glm::vec3(centerx, centery, centerz), glm::vec3(upx, upy, upz));
	m_params.viewInverseMatrix = glm::inverse(m_params.viewMatrix);
	m_params.viewInverseTransposeMatrix = glm::transpose(m_params.viewInverseMatrix);
	m_params.projectionViewMatrix = m_params.projectionMatrix * m_params.viewMatrix;
	m_params.projectionInverseMatrix = glm::inverse(m_params.projectionMatrix);
	m_params.projectionViewInverseMatrix = glm::inverse(m_params.projectionViewMatrix);
}

void CCamera::Apply(void)
{
	if (m_bDirty) {
		m_bDirty = false;
		m_offset = (GfxRenderer()->GetSwapChain()->GetFrameIndex() % CGfxSwapChain::SWAPCHAIN_FRAME_COUNT) * sizeof(m_params);
		m_ptrUniformBuffer->BufferData(m_offset, sizeof(m_params), &m_params);
	}
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

const glm::vec4& CCamera::GetScissor(void) const
{
	return m_pCamera->GetScissor();
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

const glm::vec4& CCamera::GetViewport(void) const
{
	return m_pCamera->GetViewport();
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
