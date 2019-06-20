#pragma once
#include "GfxRenderer.h"


class CALL_API CGfxUniformEngine
{
private:
	typedef struct Params {
		glm::vec4 time;
		glm::vec4 sinTime;
		glm::vec4 cosTime;
		glm::vec4 deltaTime;

		glm::mat4 shadowProjectionMatrix[4];
		glm::mat4 shadowProjectionViewMatrix[4];
		glm::mat4 shadowViewMatrix[4];
		glm::vec4 shadowParams[4];

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

		glm::vec4 mainPointLightColor;
		glm::vec4 mainPointLightPosition;
		glm::vec4 mainPointLightAttenuation;

		glm::vec4 mainDirectLightColor;
		glm::vec4 mainDirectLightDirection;

		glm::vec4 mainFogColor;
		glm::vec4 mainFogHeightParams;
		glm::vec4 mainFogDistanceParams;
	} Params;


public:
	CGfxUniformEngine(void);
	virtual ~CGfxUniformEngine(void);


public:
	const CGfxUniformBufferPtr GetUniformBuffer(void) const;

public:
	void SetTime(float t, float dt);

	void SetShadowOrtho(int indexLevel, float left, float right, float bottom, float top, float zNear, float zFar);
	void SetShadowLookat(int indexLevel, float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz);
	void SetShadowRange(int indexLevel, float range);
	void SetShadowResolution(int indexLevel, float resolution);

	void SetLightFactor(float ambientLightFactor, float pointLightFactor, float directLightFactor, float envLightFactor);

	void SetAmbientLightSH(float shRed[9], float shGreen[9], float shBlue[9]);
	void SetAmbientLightRotation(float angle, float axisx, float axisy, float axisz);
	void SetAmbientLightDirection(float dirx, float diry, float dirz);

	void SetMainPointLightColor(float red, float green, float blue);
	void SetMainPointLightPosition(float posx, float posy, float posz, float radius);
	void SetMainPointLightAttenuation(float linear, float square, float constant);

	void SetMainDirectLightColor(float red, float green, float blue);
	void SetMainDirectLightDirection(float dirx, float diry, float dirz);

	void SetMainFogColor(float red, float green, float blue);
	void SetMainFogHeightDensity(float startHeight, float endHeight, float density);
	void SetMainFogDistanceDensity(float startDistance, float endDistance, float density);

	void Apply(void);


private:
	bool m_bDirty;
	Params m_params;

private:
	CGfxUniformBufferPtr m_ptrUniformBuffer;
};
