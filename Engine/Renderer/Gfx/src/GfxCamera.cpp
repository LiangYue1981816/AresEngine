#include "GfxHeader.h"


CGfxCamera::CGfxCamera(void)
	: m_pRenderQueue(nullptr)
	, m_pUniformCamera(nullptr)
{
	m_pRenderQueue = new CGfxRenderQueue;
	m_pUniformCamera = new CGfxUniformCamera;

	m_ptrDescriptorLayout = GfxRenderer()->NewDescriptorLayout(DESCRIPTOR_SET_CAMERA);
	m_ptrDescriptorLayout->SetUniformBlockBinding(UNIFORM_CAMERA_NAME, DESCRIPTOR_BIND_CAMERA);
	m_ptrDescriptorLayout->Create();

	m_ptrDescriptorSet = GfxRenderer()->NewDescriptorSet(m_ptrDescriptorLayout);
	m_ptrDescriptorSet->SetUniformBuffer(UNIFORM_CAMERA_NAME, m_pUniformCamera->GetUniformBuffer());
}

CGfxCamera::~CGfxCamera(void)
{
	delete m_pRenderQueue;
	delete m_pUniformCamera;
}

void CGfxCamera::SetScissor(float x, float y, float width, float height)
{
	m_camera.setScissor(x, y, width, height);
}

void CGfxCamera::SetViewport(float x, float y, float width, float height)
{
	m_camera.setViewport(x, y, width, height);
	m_pUniformCamera->SetScreen(width, height);
}

void CGfxCamera::SetPerspective(float fovy, float aspect, float zNear, float zFar)
{
	m_camera.setPerspective(fovy, aspect, zNear, zFar);
	m_pUniformCamera->SetPerspective(fovy, aspect, zNear, zFar);
}

void CGfxCamera::SetOrtho(float left, float right, float bottom, float top, float zNear, float zFar)
{
	m_camera.setOrtho(left, right, bottom, top, zNear, zFar);
	m_pUniformCamera->SetOrtho(left, right, bottom, top, zNear, zFar);
}

void CGfxCamera::SetLookat(float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz)
{
	m_camera.setLookat(glm::vec3(eyex, eyey, eyez), glm::vec3(centerx, centery, centerz), glm::vec3(upx, upy, upz));
	m_pUniformCamera->SetLookat(eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz);
}

void CGfxCamera::Apply(void)
{
	m_pUniformCamera->Apply();
}

const glm::vec4& CGfxCamera::GetScissor(void) const
{
	return m_camera.scissor;
}

const glm::vec4& CGfxCamera::GetViewport(void) const
{
	return m_camera.viewport;
}

const glm::vec3& CGfxCamera::GetPosition(void) const
{
	return m_camera.position;
}

const glm::vec3& CGfxCamera::GetForwardDirection(void) const
{
	return m_camera.forward;
}

const glm::vec3& CGfxCamera::GetUpDirection(void) const
{
	return m_camera.up;
}

const glm::mat4& CGfxCamera::GetProjectionMatrix(void) const
{
	return m_camera.projectionMatrix;
}

const glm::mat4& CGfxCamera::GetViewMatrix(void) const
{
	return m_camera.viewMatrix;
}

const glm::mat4& CGfxCamera::GetViewInverseMatrix(void) const
{
	return m_camera.viewInverseMatrix;
}

const glm::mat4& CGfxCamera::GetViewInverseTransposeMatrix(void) const
{
	return m_camera.viewInverseTransposeMatrix;
}

glm::vec3 CGfxCamera::WorldToScreen(const glm::vec3 &world) const
{
	return m_camera.worldToScreen(world);
}

glm::vec3 CGfxCamera::ScreenToWorld(const glm::vec3 &screen) const
{
	return m_camera.screenToWorld(screen);
}

bool CGfxCamera::IsVisible(const glm::vec3 &vertex) const
{
	return m_camera.visible(vertex);
}

bool CGfxCamera::IsVisible(const glm::aabb &aabb) const
{
	return m_camera.visible(aabb);
}

bool CGfxCamera::IsVisible(const glm::sphere &sphere) const
{
	return m_camera.visible(sphere);
}

void CGfxCamera::Clear(int indexQueue)
{
	m_pRenderQueue->Clear(indexQueue);
}

void CGfxCamera::Begin(int indexQueue)
{
	m_pRenderQueue->Begin(indexQueue);
}

void CGfxCamera::Add(int indexQueue, int indexThread, const CGfxMaterialPtr ptrMaterial, const CGfxMeshDrawPtr ptrMeshDraw, const uint8_t *pInstanceData, uint32_t size)
{
	m_pRenderQueue->Add(indexQueue, indexThread, ptrMaterial, ptrMeshDraw, pInstanceData, size);
}

void CGfxCamera::End(int indexQueue)
{
	m_pRenderQueue->End(indexQueue);
}

void CGfxCamera::CmdDraw(int indexQueue, CGfxCommandBufferPtr ptrCommandBuffer, const CGfxDescriptorSetPtr ptrDescriptorSetEngine, uint32_t namePass)
{
	m_pRenderQueue->CmdDraw(indexQueue, ptrCommandBuffer, ptrDescriptorSetEngine, m_ptrDescriptorSet, namePass);
}
