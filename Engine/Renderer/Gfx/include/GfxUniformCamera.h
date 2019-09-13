#pragma once
#include "PreHeader.h"


class CALL_API CGfxUniformCamera
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
	CGfxUniformCamera(void);
	virtual ~CGfxUniformCamera(void);


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

private:
	CGfxUniformBufferPtr m_ptrUniformBuffer;
};
