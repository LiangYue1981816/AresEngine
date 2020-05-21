#pragma once
#include "PreHeader.h"


class CALL_API CCamera
{
	friend class CRenderQueue;
	friend class CRenderSystem;
	friend class CGPUScene;
	friend class CGPUCluster;
	friend class CPassBase;
	friend class CPassBlit;
	friend class CPassPreZ;
	friend class CPassShadow;
	friend class CPassUnlit;
	friend class CPassForwardShading;
	friend class CPassDeferredShading;
	friend class CPassTileDeferredShading;
	friend class CPassCopyColor;
	friend class CPassCopyDepthStencil;
	friend class CPassSSAO;
	friend class CPassBlurBox;
	friend class CPassBlurHorizontal;
	friend class CPassBlurVertical;
	friend class CPassBlendAdd;
	friend class CPassLuminanceThreshold;
	friend class CPassColorGrading;
	friend class CPassFinal;


public:
	CCamera(void);
	virtual ~CCamera(void);


private:
	CRenderQueue* GetRenderQueue(void) const;

private:
	CGfxCamera* GetCamera(void) const;
	CGfxUniformCamera* GetCameraUniform(void) const;

public:
	void SetDepthRange(float minz, float maxz);

	void SetScissor(float x, float y, float width, float height);
	void SetViewport(float x, float y, float width, float height);

	void SetPerspective(float fovy, float aspect, float zNear, float zFar);
	void SetOrtho(float left, float right, float bottom, float top, float zNear, float zFar);
	void SetLookat(float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz);

public:
	const float GetZNear(void) const;
	const float GetZFar(void) const;

	const float GetScissorX(void) const;
	const float GetScissorY(void) const;
	const float GetScissorWidth(void) const;
	const float GetScissorHeight(void) const;
	const glm::vec4& GetScissor(void) const;

	const float GetViewportX(void) const;
	const float GetViewportY(void) const;
	const float GetViewportWidth(void) const;
	const float GetViewportHeight(void) const;
	const glm::vec4& GetViewport(void) const;

	const glm::vec3& GetPosition(void) const;
	const glm::vec3& GetForwardDirection(void) const;
	const glm::vec3& GetUpDirection(void) const;

	const glm::mat4& GetProjectionMatrix(void) const;
	const glm::mat4& GetViewMatrix(void) const;
	const glm::mat4& GetViewInverseMatrix(void) const;
	const glm::mat4& GetViewInverseTransposeMatrix(void) const;

public:
	glm::vec3 WorldToScreen(const glm::vec3& world) const;
	glm::vec3 ScreenToWorld(const glm::vec3& screen) const;

public:
	bool IsVisible(const glm::vec3& vertex) const;
	bool IsVisible(const glm::aabb& aabb) const;
	bool IsVisible(const glm::sphere& sphere) const;

private:
	void Update(CTaskPool& taskPool, CTaskGraph& taskGraph, uint32_t mask, bool bComputeLOD) const;


private:
	CRenderQueue* m_pRenderQueue;

private:
	CGfxCamera* m_pCamera;
	CGfxUniformCamera* m_pCameraUniform;
};
