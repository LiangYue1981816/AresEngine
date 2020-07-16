#pragma once
#include "PreHeader.h"


class CALL_API CUniformEngine
{
public:
	typedef struct Params {
		glm::vec4 time;
		glm::vec4 sinTime;
		glm::vec4 cosTime;
		glm::vec4 deltaTime;

		glm::vec4 lightFactor;

		glm::vec4 ambientSHLight0;
		glm::vec4 ambientSHLight1;
		glm::vec4 ambientSHLight2;
		glm::vec4 ambientSHLight3;
		glm::vec4 ambientSHLight4;
		glm::vec4 ambientSHLight5;
		glm::vec4 ambientSHLight6;
		glm::vec4 ambientSHLight7;
		glm::vec4 ambientSHLight8;
		glm::mat4 ambientSHLightRotationMatrix;

		glm::vec4 mainPointLightColor;
		glm::vec4 mainPointLightPosition;
		glm::vec4 mainPointLightAttenuation;

		glm::vec4 mainDirectLightColor;
		glm::vec4 mainDirectLightDirection;

		glm::vec4 mainFogColor;
		glm::vec4 mainFogHeightParams;
		glm::vec4 mainFogDistanceParams;

		glm::vec4 mainShadowLevelFactor;
		glm::vec4 mainShadowDepth[4];
		glm::mat4 mainShadowViewMatrix[4];
		glm::mat4 mainShadowViewInverseMatrix[4];
		glm::mat4 mainShadowProjectionMatrix[4];
		glm::mat4 mainShadowProjectionViewMatrix[4];
	} Params;


public:
	CUniformEngine(void);
	virtual ~CUniformEngine(void);


public:
	const Params& GetParams(void) const;

public:
	const uint32_t GetUniformBufferOffset(void) const;
	const CGfxUniformBufferPtr GetUniformBuffer(void) const;

public:
	void SetTime(float t, float dt);

	void SetEnvLightFactor(float factor);
	void SetAmbientLightFactor(float factor);
	void SetMainPointLightFactor(float factor);
	void SetMainDirectLightFactor(float factor);

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

	void SetMainShadowLevelFactor(float factor0, float factor1, float factor2, float factor3);
	void SetMainShadowOrtho(int indexLevel, float left, float right, float bottom, float top, float zNear, float zFar);
	void SetMainShadowLookat(int indexLevel, float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz);

	void Apply(void);


private:
	bool m_bDirty;
	Params m_params;
	uint32_t m_offset;

private:
	CGfxUniformBufferPtr m_ptrUniformBuffer;
};
