#pragma once
#include "GfxRenderer.h"


class CALL_API CGfxUniformCamera : public CGfxResource
{
public:
	typedef struct Params {
		glm::vec4 screen;
		glm::vec4 zbuffer;
		glm::vec4 projection;

		glm::mat4 projectionMatrix;
		glm::mat4 projectionViewMatrix;
		glm::mat4 viewMatrix;
		glm::mat4 viewInverseMatrix;
		glm::mat4 viewInverseTransposeMatrix;
	} Params;


public:
	CGfxUniformCamera(void)
	{

	}
	virtual ~CGfxUniformCamera(void)
	{

	}


public:
	virtual void SetScreen(float width, float height) = 0;

	virtual void SetPerspective(float fovy, float aspect, float zNear, float zFar) = 0;
	virtual void SetOrtho(float left, float right, float bottom, float top, float zNear, float zFar) = 0;
	virtual void SetLookat(float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz) = 0;

	virtual void Apply(void) = 0;
};
