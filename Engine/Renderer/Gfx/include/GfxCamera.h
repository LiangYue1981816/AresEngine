#pragma once
#include "PreHeader.h"


class CALL_API CGfxCamera
{
public:
	CGfxCamera(void);
	virtual ~CGfxCamera(void);


public:
	void SetDepthRange(float minz, float maxz);

	void SetScissor(float x, float y, float width, float height);
	void SetViewport(float x, float y, float width, float height);

	void SetPerspective(float fovy, float aspect, float zNear, float zFar);
	void SetOrtho(float left, float right, float bottom, float top, float zNear, float zFar);
	void SetLookat(float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz);

public:
	const glm::camera& GetCamera(void) const;

	const float GetZNear(void) const;
	const float GetZFar(void) const;

	const glm::vec4& GetScissor(void) const;
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
	glm::camera m_camera;
};
