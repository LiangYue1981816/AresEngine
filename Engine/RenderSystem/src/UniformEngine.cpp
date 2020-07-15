#include "EngineHeader.h"
#include "RenderHeader.h"


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


CUniformEngine::CUniformEngine(void)
	: m_bDirty(false)
	, m_offset(0)
{
	m_ptrUniformBuffer = GfxRenderer()->NewUniformBuffer(CGfxSwapChain::SWAPCHAIN_FRAME_COUNT * sizeof(m_params));
}

CUniformEngine::~CUniformEngine(void)
{

}

uint32_t CUniformEngine::GetUniformBufferOffset(void) const
{
	return m_offset;
}

CGfxUniformBufferPtr CUniformEngine::GetUniformBuffer(void) const
{
	return m_ptrUniformBuffer;
}

void CUniformEngine::SetTime(float t, float dt)
{
	m_bDirty = true;
	m_params.SetTime(t, dt);
}

void CUniformEngine::SetEnvLightFactor(float factor)
{
	m_bDirty = true;
	m_params.SetEnvLightFactor(factor);
}

void CUniformEngine::SetAmbientLightFactor(float factor)
{
	m_bDirty = true;
	m_params.SetAmbientLightFactor(factor);
}

void CUniformEngine::SetMainPointLightFactor(float factor)
{
	m_bDirty = true;
	m_params.SetMainPointLightFactor(factor);
}

void CUniformEngine::SetMainDirectLightFactor(float factor)
{
	m_bDirty = true;
	m_params.SetMainDirectLightFactor(factor);
}

void CUniformEngine::SetAmbientLightSH(float shRed[9], float shGreen[9], float shBlue[9])
{
	m_bDirty = true;
	m_params.SetAmbientLightSH(shRed, shGreen, shBlue);
}

void CUniformEngine::SetAmbientLightRotation(float angle, float axisx, float axisy, float axisz)
{
	m_bDirty = true;
	m_params.SetAmbientLightRotation(angle, axisx, axisy, axisz);
}

void CUniformEngine::SetAmbientLightDirection(float dirx, float diry, float dirz)
{
	m_bDirty = true;
	m_params.SetAmbientLightDirection(dirx, diry, dirz);
}

void CUniformEngine::SetMainPointLightColor(float red, float green, float blue)
{
	m_bDirty = true;
	m_params.SetMainPointLightColor(red, green, blue);
}

void CUniformEngine::SetMainPointLightPosition(float posx, float posy, float posz, float radius)
{
	m_bDirty = true;
	m_params.SetMainPointLightPosition(posx, posy, posz, radius);
}

void CUniformEngine::SetMainPointLightAttenuation(float linear, float square, float constant)
{
	m_bDirty = true;
	m_params.SetMainPointLightAttenuation(linear, square, constant);
}

void CUniformEngine::SetMainDirectLightColor(float red, float green, float blue)
{
	m_bDirty = true;
	m_params.SetMainDirectLightColor(red, green, blue);
}

void CUniformEngine::SetMainDirectLightDirection(float dirx, float diry, float dirz)
{
	m_bDirty = true;
	m_params.SetMainDirectLightDirection(dirx, diry, dirz);
}

void CUniformEngine::SetMainFogColor(float red, float green, float blue)
{
	m_bDirty = true;
	m_params.SetMainFogColor(red, green, blue);
}

void CUniformEngine::SetMainFogHeightDensity(float startHeight, float endHeight, float density)
{
	m_bDirty = true;
	m_params.SetMainFogHeightDensity(startHeight, endHeight, density);
}

void CUniformEngine::SetMainFogDistanceDensity(float startDistance, float endDistance, float density)
{
	m_bDirty = true;
	m_params.SetMainFogDistanceDensity(startDistance, endDistance, density);
}

void CUniformEngine::SetMainShadowLevelFactor(float factor0, float factor1, float factor2, float factor3)
{
	m_bDirty = true;
	m_params.SetMainShadowLevelFactor(factor0, factor1, factor2, factor3);
}

void CUniformEngine::SetMainShadowOrtho(int indexLevel, float left, float right, float bottom, float top, float zNear, float zFar)
{
	if (indexLevel >= 0 && indexLevel < 4) {
		m_bDirty = true;
		m_params.SetMainShadowOrtho(GfxRenderer()->GetBaseMatrix(), indexLevel, left, right, bottom, top, zNear, zFar);
	}
}

void CUniformEngine::SetMainShadowLookat(int indexLevel, float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz)
{
	if (indexLevel >= 0 && indexLevel < 4) {
		m_bDirty = true;
		m_params.SetMainShadowLookat(indexLevel, eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz);
	}
}

void CUniformEngine::Apply(void)
{
	if (m_bDirty) {
		m_bDirty = false;
		m_offset = (GfxRenderer()->GetSwapChain()->GetFrameIndex() % CGfxSwapChain::SWAPCHAIN_FRAME_COUNT) * sizeof(m_params);
		m_ptrUniformBuffer->BufferData(m_offset, sizeof(m_params), &m_params);
	}
}
