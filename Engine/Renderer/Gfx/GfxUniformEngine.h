#pragma once
#include "GfxRenderer.h"


class CGfxUniformEngine : public CGfxResource
{
public:
	typedef struct Params {
		glm::vec4 time;
		glm::vec4 sinTime;
		glm::vec4 cosTime;
		glm::vec4 deltaTime;

		glm::mat4 shadowProjectionMatrix;
		glm::mat4 shadowProjectionViewMatrix;
		glm::mat4 shadowViewMatrix;
		glm::vec4 shadowParams;

		glm::vec4 lightFactor;

		glm::vec4 ambientLightSH0;
		glm::vec4 ambientLightSH1;
		glm::vec4 ambientLightSH2;
		glm::vec4 ambientLightSH3;
		glm::vec4 ambientLightSH4;
		glm::vec4 ambientLightSH5;
		glm::vec4 ambientLightSH6;
		glm::vec4 ambientLightSH7;
		glm::vec4 ambientLightSH8;
		glm::mat4 ambientLightRotationMatrix;

		glm::vec4 pointLightColor;
		glm::vec4 pointLightPosition;
		glm::vec4 pointLightAttenuation;

		glm::vec4 directLightColor;
		glm::vec4 directLightDirection;

		glm::vec4 fogColor;
		glm::vec4 fogHeightDensity;
		glm::vec4 fogDistanceDensity;
	} Params;


public:
	CGfxUniformEngine(void)
	{

	}
	virtual ~CGfxUniformEngine(void)
	{

	}


public:
	virtual void SetTime(float t, float dt) = 0;

	virtual void SetShadowOrtho(float left, float right, float bottom, float top, float zNear, float zFar) = 0;
	virtual void SetShadowLookat(float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz) = 0;
	virtual void SetShadowRange(float range) = 0;
	virtual void SetShadowResolution(float resolution) = 0;

	virtual void SetLightFactor(float ambientLightFactor, float pointLightFactor, float directLightFactor, float envLightFactor) = 0;

	virtual void SetAmbientLightSH(float shRed[9], float shGreen[9], float shBlue[9]) = 0;
	virtual void SetAmbientLightRotation(float angle, float axisx, float axisy, float axisz) = 0;

	virtual void SetPointLightColor(float red, float green, float blue) = 0;
	virtual void SetPointLightPosition(float posx, float posy, float posz, float radius) = 0;
	virtual void SetPointLightAttenuation(float linear, float square, float constant) = 0;

	virtual void SetDirectLightColor(float red, float green, float blue) = 0;
	virtual void SetDirectLightDirection(float dirx, float diry, float dirz) = 0;

	virtual void SetFogColor(float red, float green, float blue) = 0;
	virtual void SetFogHeightDensity(float startHeight, float endHeight, float density) = 0;
	virtual void SetFogDistanceDensity(float startDistance, float endDistance, float density) = 0;

	virtual void Apply(void) = 0;
};
