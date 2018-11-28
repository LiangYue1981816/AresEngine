#pragma once
#include "GfxRenderer.h"


class CALL_API CGfxUniformCamera
{
private:
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
	CGfxUniformCamera(bool bDynamic);
	virtual ~CGfxUniformCamera(void);


public:
	void SetScreen(float width, float height);

	void SetPerspective(float fovy, float aspect, float zNear, float zFar);
	void SetOrtho(float left, float right, float bottom, float top, float zNear, float zFar);
	void SetLookat(float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz);

	void Apply(void);

public:
	const CGfxUniformBufferPtr& GetUniformBuffer(void) const;


private:
	bool m_bDirty;
	uint32_t m_hash;
	Params m_params;

private:
	CGfxUniformBufferPtr m_ptrUniformBuffer;
};
