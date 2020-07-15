#pragma once
#include "PreHeader.h"


class CALL_API CUniformEngine
{
public:
	typedef struct Params {
		void SetTime(float t, float dt)
		{
			time = glm::vec4(t / 20.0f, t * 1.0f, t * 2.0f, t * 3.0f);
			sinTime = glm::vec4(sinf(t / 8.0f), sinf(t / 4.0f), sinf(t / 2.0f), t);
			cosTime = glm::vec4(cosf(t / 8.0f), cosf(t / 4.0f), cosf(t / 2.0f), t);
			deltaTime = glm::vec4(dt, 1.0f / dt, 1.0f, 1.0f);
		}

		void SetEnvLightFactor(float factor)
		{
			lightFactor.w = factor;
		}

		void SetAmbientLightFactor(float factor)
		{
			lightFactor.x = factor;
		}

		void SetMainPointLightFactor(float factor)
		{
			lightFactor.y = factor;
		}

		void SetMainDirectLightFactor(float factor)
		{
			lightFactor.z = factor;
		}

		void SetAmbientLightSH(float shRed[9], float shGreen[9], float shBlue[9])
		{
			ambientSHLight0 = glm::vec4(shRed[0], shRed[1], shRed[2], 0.0);
			ambientSHLight1 = glm::vec4(shRed[3], shRed[4], shRed[5], 0.0);
			ambientSHLight2 = glm::vec4(shRed[6], shRed[7], shRed[8], 0.0);
			ambientSHLight3 = glm::vec4(shGreen[0], shGreen[1], shGreen[2], 0.0);
			ambientSHLight4 = glm::vec4(shGreen[3], shGreen[4], shGreen[5], 0.0);
			ambientSHLight5 = glm::vec4(shGreen[6], shGreen[7], shGreen[8], 0.0);
			ambientSHLight6 = glm::vec4(shBlue[0], shBlue[1], shBlue[2], 0.0);
			ambientSHLight7 = glm::vec4(shBlue[3], shBlue[4], shBlue[5], 0.0);
			ambientSHLight8 = glm::vec4(shBlue[6], shBlue[7], shBlue[8], 0.0);
		}

		void SetAmbientLightRotation(float angle, float axisx, float axisy, float axisz)
		{
			ambientSHLightRotationMatrix = glm::rotate(glm::mat4(), -angle, glm::vec3(axisx, axisy, axisz));
		}

		void SetAmbientLightDirection(float dirx, float diry, float dirz)
		{
			ambientSHLightRotationMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::normalize(glm::vec3(dirx, diry, dirz)), glm::vec3(0.0f, 1.0f, 0.0f));
		}

		void SetMainPointLightColor(float red, float green, float blue)
		{
			mainPointLightColor = glm::vec4(red, green, blue, 0.0f);
		}

		void SetMainPointLightPosition(float posx, float posy, float posz, float radius)
		{
			mainPointLightPosition = glm::vec4(posx, posy, posz, radius);
		}

		void SetMainPointLightAttenuation(float linear, float square, float constant)
		{
			mainPointLightAttenuation = glm::vec4(linear, square, constant, 0.0f);
		}

		void SetMainDirectLightColor(float red, float green, float blue)
		{
			mainDirectLightColor = glm::vec4(red, green, blue, 0.0);
		}

		void SetMainDirectLightDirection(float dirx, float diry, float dirz)
		{
			mainDirectLightDirection = glm::vec4(glm::normalize(glm::vec3(-dirx, -diry, -dirz)), 0.0f);
		}

		void SetMainFogColor(float red, float green, float blue)
		{
			mainFogColor = glm::vec4(red, green, blue, 0.0f);
		}

		void SetMainFogHeightDensity(float startHeight, float endHeight, float density)
		{
			mainFogHeightParams = glm::vec4(startHeight, endHeight, density, 0.0f);
		}

		void SetMainFogDistanceDensity(float startDistance, float endDistance, float density)
		{
			mainFogDistanceParams = glm::vec4(startDistance, endDistance, density, 0.0f);
		}

		void SetMainShadowLevelFactor(float factor0, float factor1, float factor2, float factor3)
		{
			mainShadowLevelFactor = glm::vec4(factor0, factor1, factor2, factor3);
		}

		void SetMainShadowOrtho(const glm::mat4& baseMatrix, int indexLevel, float left, float right, float bottom, float top, float zNear, float zFar)
		{
			mainShadowDepth[indexLevel] = glm::vec4(zNear, zFar, zFar - zNear, 1.0f / (zFar - zNear));
			mainShadowProjectionMatrix[indexLevel] = baseMatrix * glm::ortho(left, right, bottom, top, zNear, zFar);
			mainShadowProjectionViewMatrix[indexLevel] = mainShadowProjectionMatrix[indexLevel] * mainShadowViewMatrix[indexLevel];
		}

		void SetMainShadowLookat(int indexLevel, float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz)
		{
			mainShadowViewMatrix[indexLevel] = glm::lookAt(glm::vec3(eyex, eyey, eyez), glm::vec3(centerx, centery, centerz), glm::vec3(upx, upy, upz));
			mainShadowViewInverseMatrix[indexLevel] = glm::inverse(mainShadowViewMatrix[indexLevel]);
			mainShadowProjectionViewMatrix[indexLevel] = mainShadowProjectionMatrix[indexLevel] * mainShadowViewMatrix[indexLevel];
		}

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

	void Apply(uint32_t indexFrame);


private:
	bool m_bDirty;
	Params m_params;

private:
	CGfxUniformBufferPtr m_ptrUniformBuffer;
};
