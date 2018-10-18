#pragma once
#include "GfxRenderer.h"


class CGfxCamera
{
public:
	CGfxCamera(void);
	virtual ~CGfxCamera(void);


public:
	void SetScissor(float x, float y, float width, float height);
	void SetViewport(float x, float y, float width, float height);

	void SetPerspective(float fovy, float aspect, float zNear, float zFar);
	void SetOrtho(float left, float right, float bottom, float top, float zNear, float zFar);
	void SetLookat(float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz);

	void Apply(void);

	const glm::vec4& GetScissor(void) const;
	const glm::vec4& GetViewport(void) const;

	const glm::vec3& GetPosition(void) const;
	const glm::vec3& GetForwardDirection(void) const;
	const glm::vec3& GetUpDirection(void) const;

	const glm::mat4& GetProjectionMatrix(void) const;
	const glm::mat4& GetViewMatrix(void) const;
	const glm::mat4& GetViewInverseMatrix(void) const;
	const glm::mat4& GetViewInverseTransposeMatrix(void) const;

	const CGfxUniformCamera* GetUniformCamera(void) const;

public:
	glm::vec3 WorldToScreen(const glm::vec3 &world) const;
	glm::vec3 ScreenToWorld(const glm::vec3 &screen) const;

public:
	bool IsVisible(const glm::vec3 &vertex) const;
	bool IsVisible(const glm::aabb &aabb) const;
	bool IsVisible(const glm::sphere &sphere) const;

public:
	void AddQueue(int indexThread, int indexQueue, const CGfxMaterialPtr &ptrMaterial, const CGfxMeshPtr &ptrMesh, const glm::mat4 &mtxTransform);
	void ClearQueue(int indexQueue);
	void ClearQueueAll(void);

public:
	void CmdDraw(int indexThread, int indexQueue, uint32_t namePass);
	void CmdExecute(CGfxCommandBuffer *pMainCommandBuffer, int indexQueue);


private:
	glm::camera m_camera;

private:
	CGfxRenderQueue *m_pRenderQueue;
	CGfxUniformCamera *m_pUniformCamera;
};
