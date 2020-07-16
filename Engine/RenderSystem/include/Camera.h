#pragma once
#include "PreHeader.h"


class CALL_API CRenderQueue;
class CALL_API CCamera
{
public:
	typedef struct Params {
		glm::vec4 screen;
		glm::vec4 depth;

		glm::mat4 viewMatrix;
		glm::mat4 viewInverseMatrix;
		glm::mat4 viewInverseTransposeMatrix;
		glm::mat4 projectionMatrix;
		glm::mat4 projectionViewMatrix;
		glm::mat4 projectionInverseMatrix;
		glm::mat4 projectionViewInverseMatrix;
	} Params;


public:
	CCamera(void);
	virtual ~CCamera(void);


public:
	Params GetParams(void) const;
	CRenderQueue* GetRenderQueue(void) const;

public:
	uint32_t GetUniformBufferOffset(void) const;
	CGfxUniformBufferPtr GetUniformBuffer(void) const;

public:
	void SetDepthRange(float minz, float maxz);

	void SetScissor(float x, float y, float width, float height);
	void SetViewport(float x, float y, float width, float height);

	void SetPerspective(float fovy, float aspect, float zNear, float zFar);
	void SetOrtho(float left, float right, float bottom, float top, float zNear, float zFar);
	void SetLookat(float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz);

	void Apply(void);

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

public:
	void Update(CTaskPool& taskPool, CTaskGraph& taskGraph, uint32_t mask, bool bComputeLOD) const;


private:
	bool m_bDirty;
	Params m_params;
	uint32_t m_offset;

private:
	CRenderQueue* m_pRenderQueue;

private:
	CGfxCamera* m_pCamera;
	CGfxUniformBufferPtr m_ptrUniformBuffer;
};
