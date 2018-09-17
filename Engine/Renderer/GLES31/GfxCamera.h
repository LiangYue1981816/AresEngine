#pragma once
#include "GfxRenderer.h"


class CGfxCamera
{
	friend class CGfxRenderer;
	friend class CGfxCameraManager;


private:
	CGfxCamera(void);
	virtual ~CGfxCamera(void);


public:
	void SetFrameBuffer(CGfxFrameBuffer *pFrameBuffer);

public:
	void SetEnableClearDepth(bool bEnable);
	void SetEnableClearColor(bool bEnable);

	void SetClearDepth(float depth);
	void SetClearColor(float red, float green, float blue, float alpha);

public:
	void SetScissor(float x, float y, float width, float height);
	void SetViewport(float x, float y, float width, float height);
	void SetPerspective(float fovy, float aspect, float zNear, float zFar);
	void SetOrtho(float left, float right, float bottom, float top, float zNear, float zFar);
	void SetLookat(float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz);

	const glm::vec3& GetPosition(void) const;
	const glm::vec3& GetForwardDirection(void) const;
	const glm::vec3& GetUpDirection(void) const;

	const glm::mat4& GetProjectionMatrix(void) const;
	const glm::mat4& GetViewMatrix(void) const;
	const glm::mat4& GetViewInverseMatrix(void) const;
	const glm::mat4& GetViewInverseTransposeMatrix(void) const;

public:
	glm::vec3 WorldToScreen(const glm::vec3 &world);
	glm::vec3 ScreenToWorld(const glm::vec3 &screen);

public:
	bool IsVisible(const glm::vec3 &vertex);
	bool IsVisible(const glm::aabb &aabb);
	bool IsVisible(const glm::sphere &sphere);

public:
	void AddQueue(CGfxMaterial *pMaterial, CGfxMesh *pMesh, const glm::mat4 &mtxTransform, int indexThread);
	void ClearQueue(void);

public:
	void CmdDraw(void);
	void Submit(void);

private:
	const CGfxUniformCamera* GetUniformCamera(void) const;


private:
	bool m_bEnableClearDepth;
	bool m_bEnableClearColor;

	float m_clearDepth;
	float m_clearColorRed;
	float m_clearColorGreen;
	float m_clearColorBlue;
	float m_clearColorAlpha;

private:
	glm::camera m_camera;

	CGfxFrameBuffer *m_pFrameBuffer;
	CGfxUniformCamera *m_pUniformCamera;

private:
	GLuint m_index;
	eastl::unordered_map<CGfxMaterial*, eastl::unordered_map<CGfxMesh*, eastl::vector<glm::mat4>>> m_queueOpaque[THREAD_COUNT][2];
	eastl::unordered_map<CGfxMaterial*, eastl::unordered_map<CGfxMesh*, eastl::vector<glm::mat4>>> m_queueTransparent[THREAD_COUNT][2];
	CGfxCommandBuffer *m_pCommandBuffer[2];
};
