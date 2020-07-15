#pragma once
#include "PreHeader.h"


class CALL_API CUniformCamera
{
public:
	typedef struct Params {
		void SetScreen(float width, float height)
		{
			screen = glm::vec4(width, height, 1.0f / width, 1.0f / height);
		}

		void SetPerspective(const glm::mat4& baseMatrix, float fovy, float aspect, float zNear, float zFar)
		{
			depth = glm::vec4(zNear, zFar, zFar - zNear, 1.0f / (zFar - zNear));
			projectionMatrix = baseMatrix * glm::perspective(glm::radians(fovy), aspect, zNear, zFar);
			projectionViewMatrix = projectionMatrix * viewMatrix;
			projectionInverseMatrix = glm::inverse(projectionMatrix);
			projectionViewInverseMatrix = glm::inverse(projectionViewMatrix);
		}

		void SetOrtho(const glm::mat4& baseMatrix, float left, float right, float bottom, float top, float zNear, float zFar)
		{
			depth = glm::vec4(zNear, zFar, zFar - zNear, 1.0f / (zFar - zNear));
			projectionMatrix = baseMatrix * glm::ortho(left, right, bottom, top, zNear, zFar);
			projectionViewMatrix = projectionMatrix * viewMatrix;
			projectionInverseMatrix = glm::inverse(projectionMatrix);
			projectionViewInverseMatrix = glm::inverse(projectionViewMatrix);
		}

		void SetLookat(float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz)
		{
			viewMatrix = glm::lookAt(glm::vec3(eyex, eyey, eyez), glm::vec3(centerx, centery, centerz), glm::vec3(upx, upy, upz));
			viewInverseMatrix = glm::inverse(viewMatrix);
			viewInverseTransposeMatrix = glm::transpose(viewInverseMatrix);
			projectionViewMatrix = projectionMatrix * viewMatrix;
			projectionInverseMatrix = glm::inverse(projectionMatrix);
			projectionViewInverseMatrix = glm::inverse(projectionViewMatrix);
		}

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
	CUniformCamera(void);
	virtual ~CUniformCamera(void);


public:
	const Params& GetParams(void) const;
	const CGfxUniformBufferPtr GetUniformBuffer(void) const;

public:
	void SetScreen(float width, float height);

	void SetPerspective(float fovy, float aspect, float zNear, float zFar);
	void SetOrtho(float left, float right, float bottom, float top, float zNear, float zFar);
	void SetLookat(float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz);

	void Apply(void);


private:
	bool m_bDirty;
	Params m_params;
	uint32_t m_offset;

private:
	CGfxUniformBufferPtr m_ptrUniformBuffer;
};
