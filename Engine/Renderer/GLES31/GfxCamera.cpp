#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxCamera.h"


CGfxCamera::CGfxCamera(void)
	: m_pUniformCamera(NULL)
{
	m_pUniformCamera = new CGfxUniformCamera;
}

CGfxCamera::~CGfxCamera(void)
{
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

glm::vec3 CGfxCamera::WorldToScreen(const glm::vec3 &world)
{
	return m_camera.worldToScreen(world);
}

glm::vec3 CGfxCamera::ScreenToWorld(const glm::vec3 &screen)
{
	return m_camera.screenToWorld(screen);
}

bool CGfxCamera::IsVisible(const glm::vec3 &vertex)
{
	return m_camera.visible(vertex);
}

bool CGfxCamera::IsVisible(const glm::aabb &aabb)
{
	return m_camera.visible(aabb);
}

bool CGfxCamera::IsVisible(const glm::sphere &sphere)
{
	return m_camera.visible(sphere);
}

void CGfxCamera::AddQueue(int indexThread, int indexQueue, const CGfxMaterialPtr &ptrMaterial, const CGfxMeshPtr &ptrMesh, const glm::mat4 &mtxTransform)
{
	if (indexThread >= 0 && indexThread < THREAD_COUNT) {
		m_materialQueue[indexThread][indexQueue][ptrMaterial][ptrMesh].push_back(mtxTransform);
	}
}

void CGfxCamera::ClearQueueAll(void)
{
	ClearQueue(0);
	ClearQueue(1);
}

void CGfxCamera::ClearQueue(int indexQueue)
{
	for (int indexThread = 0; indexThread < THREAD_COUNT; indexThread++) {
		m_materialQueue[indexThread][indexQueue].clear();
		m_secondaryCommandBuffer[indexThread][indexQueue].clear();
	}
}

void CGfxCamera::CmdDraw(int indexThread, int indexQueue, uint32_t namePass)
{
	eastl::unordered_map<const CGfxPipelineBase*, eastl::vector<CGfxMaterialPtr>> pipelineQueue;
	{
		for (const auto &itMaterialQueue : m_materialQueue[indexThread][indexQueue]) {
			if (const CGfxMaterialPass *pPass = itMaterialQueue.first->GetPass(namePass)) {
				if (const CGfxPipelineBase *pPipeline = pPass->GetPipeline()) {
					pipelineQueue[pPipeline].push_back(itMaterialQueue.first);
				}
			}
		}

		if (pipelineQueue.empty()) {
			return;
		}
	}

	m_secondaryCommandBuffer[indexThread][indexQueue].emplace_back(false);
	CGfxCommandBuffer *pCommandBuffer = &m_secondaryCommandBuffer[indexThread][indexQueue][m_secondaryCommandBuffer[indexThread][indexQueue].size() - 1];
	{
		for (const auto &itPipelineQueue : pipelineQueue) {
			Renderer()->CmdBindPipeline(pCommandBuffer, (CGfxPipelineBase *)itPipelineQueue.first);
			Renderer()->CmdBindCamera(pCommandBuffer, this);

			for (const auto &itMaterial : itPipelineQueue.second) {
				Renderer()->CmdBindMaterialPass(pCommandBuffer, itMaterial, namePass);

				for (const auto &itMeshQueue : m_materialQueue[indexThread][indexQueue][itMaterial]) {
					Renderer()->CmdDrawInstance(pCommandBuffer, itMeshQueue.first, itMeshQueue.first->GetIndexCount(), 0, itMeshQueue.second);
				}
			}
		}
	}
}

void CGfxCamera::CmdExecute(int indexQueue, CGfxCommandBuffer *pMainCommandBuffer)
{
	Renderer()->CmdSetScissor(pMainCommandBuffer, (int)m_camera.scissor.x, (int)m_camera.scissor.y, (int)m_camera.scissor.z, (int)m_camera.scissor.w);
	Renderer()->CmdSetViewport(pMainCommandBuffer, (int)m_camera.viewport.x, (int)m_camera.viewport.y, (int)m_camera.viewport.z, (int)m_camera.viewport.w);

	for (int indexThread = 0; indexThread < THREAD_COUNT; indexThread++) {
		for (const auto &itCommandBuffer : m_secondaryCommandBuffer[indexThread][indexQueue]) {
			Renderer()->CmdExecute(pMainCommandBuffer, (CGfxCommandBuffer *)&itCommandBuffer);
		}
	}
}

void CGfxCamera::Apply(void)
{
	m_pUniformCamera->Apply();
}

const CGfxUniformCamera* CGfxCamera::GetUniformCamera(void) const
{
	return m_pUniformCamera;
}
