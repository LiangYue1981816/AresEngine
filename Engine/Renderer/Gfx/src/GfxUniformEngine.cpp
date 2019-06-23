#include "GfxHeader.h"


/*
static void SHRotationMatrix0(float rotation0[1][1])
{
	rotation0[0][0] = 1.0f;
}

static void SHRotationMatrix1(float rotation1[3][3], float angle, const glm::vec3 &axis)
{
	glm::mat4 rotation = glm::rotate(glm::mat4(), angle, axis);

	rotation1[0][0] =  rotation[2][2];
	rotation1[0][1] = -rotation[2][0];
	rotation1[0][2] = -rotation[2][1];

	rotation1[1][0] = -rotation[0][2];
	rotation1[1][1] =  rotation[0][0];
	rotation1[1][2] = -rotation[0][1];

	rotation1[2][0] = -rotation[1][2];
	rotation1[2][1] = -rotation[1][0];
	rotation1[2][2] =  rotation[1][1];
}

static void SHRotationMatrix2(float rotation2[5][5], const float rotation1[3][3])
{
	const int l = 2;
	float P[3][5][5] = { 0.0f };

	for (int i = -1; i <= 1; i++) {
		for (int m = -1; m <= 1; m++) {
			for (int n = -l; n <= l; n++) {
				if (n == l) {
					P[i + 1][m + l][n + l] = rotation1[i + 1][2] * rotation1[m + 1][l - 1 + 1] - rotation1[i + 1][0] * rotation1[m + 1][-l + 1 + 1];
				}
				else if (n == -l) {
					P[i + 1][m + l][n + l] = rotation1[i + 1][2] * rotation1[m + 1][-l + 1 + 1] + rotation1[i + 1][0] * rotation1[m + 1][l - 1 + 1];
				}
				else {
					P[i + 1][m + l][n + l] = rotation1[i + 1][1] * rotation1[m + 1][n + 1];
				}
			}
		}
	}

	for (int m = -l; m <= l; m++) {
		for (int n = -l; n <= l; n++) {
			float d = (m == 0) ? 1.0f : 0.0f;
			float denom = (abs(n) == l) ? 2.0f * l * (2.0f * l - 1.0f) : (l + n) * (l - n);
			float U = 0.0f, u = 1.0f * sqrtf((l + m) * (l - m) / denom);
			float V = 0.0f, v = 0.5f * sqrtf((1 + d) * (l + abs(m) - 1) * (l + abs(m)) / denom) * (1 - 2 * d);
			float W = 0.0f, w = -0.5f * sqrtf((l - abs(m) - 1) * (l - abs(m)) / denom) * (1 - d);

			if (m == 0) {
				U = P[0 + 1][m + l][n + l];
				V = P[1 + 1][1 + l][n + l] + P[-1 + 1][-1 + l][n + l];
				W = P[1 + 1][m + 1 + l][n + l] + P[-1 + 1][-m - 1 + l][n + l];
			}

			if (m > 0) {
				float d = (m == 1) ? 1.0f : 0.0f;
				U = P[0 + 1][m + l][n + l];
				V = P[1 + 1][m - 1 + l][n + l] * sqrtf(1 + d) - P[-1 + 1][-m + 1 + l][n + l] * (1 - d);
				W = w != 0.0f ? P[1 + 1][m + 1 + l][n + l] + P[-1 + 1][-m - 1 + l][n + l] : 0.0f;
			}

			if (m < 0) {
				float d = (m == -1) ? 1.0f : 0.0f;
				U = P[0 + 1][m + l][n + l];
				V = P[1 + 1][m + 1 + l][n + l] * (1 - d) + P[-1 + 1][-m - 1 + l][n + l] * sqrtf(1 + d);
				W = w != 0.0f ? P[1 + 1][m - 1 + l][n + l] - P[-1 + 1][-m + 1 + l][n + l] : 0.0f;
			}

			rotation2[m + l][n + l] = u * U + v * V + w * W;
		}
	}
}

static void SHRotationMatrix(float rotation[9][9], float angle, const glm::vec3 &axis)
{
	float rotation0[1][1] = { 0.0f };
	float rotation1[3][3] = { 0.0f };
	float rotation2[5][5] = { 0.0f };

	{
		SHRotationMatrix0(rotation0);
		SHRotationMatrix1(rotation1, angle, axis);
		SHRotationMatrix2(rotation2, rotation1);
	}

	{
		rotation[0][0] = rotation0[0][0];

		for (int m = 0; m < 3; m++) {
			for (int n = 0; n < 3; n++) {
				rotation[m + 1][n + 1] = rotation1[m][n];
			}
		}

		for (int m = 0; m < 5; m++) {
			for (int n = 0; n < 5; n++) {
				rotation[m + 4][n + 4] = rotation2[m][n];
			}
		}
	}
}

static void SHRotate(float shRedRotate[9], float shGreenRotate[9], float shBlueRotate[9], float shRedSrc[9], float shGreenSrc[9], float shBlueSrc[9], float angle, const glm::vec3 &axis)
{
	float rotation[9][9] = { 0.0f };
	SHRotationMatrix(rotation, angle, axis);

	for (int m = 0; m < 9; m++) {
		shRedRotate[m] = 0.0f;
		shGreenRotate[m] = 0.0f;
		shBlueRotate[m] = 0.0f;

		for (int n = 0; n < 9; n++) {
			shRedRotate[m] += shRedSrc[n] * rotation[m][n];
			shGreenRotate[m] += shGreenSrc[n] * rotation[m][n];
			shBlueRotate[m] += shBlueSrc[n] * rotation[m][n];
		}
	}
}
*/


CGfxUniformEngine::CGfxUniformEngine(void)
	: m_bDirty(false)
{
	m_ptrUniformBuffer = GfxRenderer()->NewUniformBuffer(sizeof(m_params));
}

CGfxUniformEngine::~CGfxUniformEngine(void)
{

}

const CGfxUniformEngine::Params& CGfxUniformEngine::GetParams(void) const
{
	return m_params;
}

const CGfxUniformBufferPtr CGfxUniformEngine::GetUniformBuffer(void) const
{
	return m_ptrUniformBuffer;
}

void CGfxUniformEngine::SetTime(float t, float dt)
{
	m_bDirty = true;
	m_params.time = glm::vec4(t / 20.0f, t * 1.0f, t * 2.0f, t * 3.0f);
	m_params.sinTime = glm::vec4(sinf(t / 8.0f), sinf(t / 4.0f), sinf(t / 2.0f), t);
	m_params.cosTime = glm::vec4(cosf(t / 8.0f), cosf(t / 4.0f), cosf(t / 2.0f), t);
	m_params.deltaTime = glm::vec4(dt, 1.0f / dt, 1.0f, 1.0f);
}

void CGfxUniformEngine::SetShadowOrtho(int indexLevel, float left, float right, float bottom, float top, float zNear, float zFar)
{
	if (indexLevel >= 0 && indexLevel < 4) {
		m_bDirty = true;
		m_params.shadowParams[indexLevel].x = zFar - zNear;
		m_params.shadowParams[indexLevel].y = 1.0f / (zFar - zNear);
		m_params.shadowProjectionMatrix[indexLevel] = GfxRenderer()->GetBaseMatrix() * glm::ortho(left, right, bottom, top, zNear, zFar);
		m_params.shadowProjectionViewMatrix[indexLevel] = m_params.shadowProjectionMatrix[indexLevel] * m_params.shadowViewMatrix[indexLevel];
	}
}

void CGfxUniformEngine::SetShadowLookat(int indexLevel, float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz)
{
	if (indexLevel >= 0 && indexLevel < 4) {
		m_bDirty = true;
		m_params.shadowViewMatrix[indexLevel] = glm::lookAt(glm::vec3(eyex, eyey, eyez), glm::vec3(centerx, centery, centerz), glm::vec3(upx, upy, upz));
		m_params.shadowProjectionViewMatrix[indexLevel] = m_params.shadowProjectionMatrix[indexLevel] * m_params.shadowViewMatrix[indexLevel];
	}
}

void CGfxUniformEngine::SetShadowRange(int indexLevel, float range)
{
	if (indexLevel >= 0 && indexLevel < 4) {
		m_bDirty = true;
		m_params.shadowParams[indexLevel].z = range;
	}
}

void CGfxUniformEngine::SetShadowResolution(int indexLevel, float resolution)
{
	if (indexLevel >= 0 && indexLevel < 4) {
		m_bDirty = true;
		m_params.shadowParams[indexLevel].w = resolution;
	}
}

void CGfxUniformEngine::SetLightFactor(float ambientLightFactor, float pointLightFactor, float directLightFactor, float envLightFactor)
{
	m_bDirty = true;
	m_params.lightFactor = glm::vec4(ambientLightFactor, pointLightFactor, directLightFactor, envLightFactor);
}

void CGfxUniformEngine::SetAmbientLightSH(float shRed[9], float shGreen[9], float shBlue[9])
{
	m_bDirty = true;
	m_params.ambientLightSH0 = glm::vec4(shRed[0], shRed[1], shRed[2], 0.0);
	m_params.ambientLightSH1 = glm::vec4(shRed[3], shRed[4], shRed[5], 0.0);
	m_params.ambientLightSH2 = glm::vec4(shRed[6], shRed[7], shRed[8], 0.0);
	m_params.ambientLightSH3 = glm::vec4(shGreen[0], shGreen[1], shGreen[2], 0.0);
	m_params.ambientLightSH4 = glm::vec4(shGreen[3], shGreen[4], shGreen[5], 0.0);
	m_params.ambientLightSH5 = glm::vec4(shGreen[6], shGreen[7], shGreen[8], 0.0);
	m_params.ambientLightSH6 = glm::vec4(shBlue[0], shBlue[1], shBlue[2], 0.0);
	m_params.ambientLightSH7 = glm::vec4(shBlue[3], shBlue[4], shBlue[5], 0.0);
	m_params.ambientLightSH8 = glm::vec4(shBlue[6], shBlue[7], shBlue[8], 0.0);
}

void CGfxUniformEngine::SetAmbientLightRotation(float angle, float axisx, float axisy, float axisz)
{
	m_bDirty = true;
	m_params.ambientLightRotationMatrix = glm::rotate(glm::mat4(), -angle, glm::vec3(axisx, axisy, axisz));
}

void CGfxUniformEngine::SetAmbientLightDirection(float dirx, float diry, float dirz)
{
	m_bDirty = true;
	m_params.ambientLightRotationMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::normalize(glm::vec3(dirx, diry, dirz)), glm::vec3(0.0f, 1.0f, 0.0f));
}

void CGfxUniformEngine::SetMainPointLightColor(float red, float green, float blue)
{
	m_bDirty = true;
	m_params.mainPointLightColor = glm::vec4(red, green, blue, 0.0f);
}

void CGfxUniformEngine::SetMainPointLightPosition(float posx, float posy, float posz, float radius)
{
	m_bDirty = true;
	m_params.mainPointLightPosition = glm::vec4(posx, posy, posz, radius);
}

void CGfxUniformEngine::SetMainPointLightAttenuation(float linear, float square, float constant)
{
	m_bDirty = true;
	m_params.mainPointLightAttenuation = glm::vec4(linear, square, constant, 0.0f);
}

void CGfxUniformEngine::SetMainDirectLightColor(float red, float green, float blue)
{
	m_bDirty = true;
	m_params.mainDirectLightColor = glm::vec4(red, green, blue, 0.0);
}

void CGfxUniformEngine::SetMainDirectLightDirection(float dirx, float diry, float dirz)
{
	m_bDirty = true;
	m_params.mainDirectLightDirection = glm::vec4(glm::normalize(glm::vec3(-dirx, -diry, -dirz)), 0.0f);
}

void CGfxUniformEngine::SetMainFogColor(float red, float green, float blue)
{
	m_bDirty = true;
	m_params.mainFogColor = glm::vec4(red, green, blue, 0.0f);
}

void CGfxUniformEngine::SetMainFogHeightDensity(float startHeight, float endHeight, float density)
{
	m_bDirty = true;
	m_params.mainFogHeightParams = glm::vec4(startHeight, endHeight, density, 0.0f);
}

void CGfxUniformEngine::SetMainFogDistanceDensity(float startDistance, float endDistance, float density)
{
	m_bDirty = true;
	m_params.mainFogDistanceParams = glm::vec4(startDistance, endDistance, density, 0.0f);
}

void CGfxUniformEngine::Apply(void)
{
	if (m_bDirty) {
		m_bDirty = false;
		m_ptrUniformBuffer->BufferData(0, sizeof(m_params), &m_params);
	}
}
