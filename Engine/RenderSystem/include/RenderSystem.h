#pragma once
#include "PreHeader.h"


#define VERTEX_ATTRIBUTE_POSITION                          0x00000001
#define VERTEX_ATTRIBUTE_NORMAL                            0x00000002
#define VERTEX_ATTRIBUTE_BINORMAL                          0x00000004
#define VERTEX_ATTRIBUTE_COLOR                             0x00000008
#define VERTEX_ATTRIBUTE_TEXCOORD0                         0x00000010
#define VERTEX_ATTRIBUTE_TEXCOORD1                         0x00000020
#define VERTEX_ATTRIBUTE_COUNT                             6

#define INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL0           0x00000001
#define INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL1           0x00000002
#define INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL2           0x00000004
#define INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL3           0x00000008
#define INSTANCE_ATTRIBUTE_POINTLIGHT_COLOR                0x00000010
#define INSTANCE_ATTRIBUTE_POINTLIGHT_ATTENUATION          0x00000020
#define INSTANCE_ATTRIBUTE_COUNT                           6

#define INSTANCE_ATTRIBUTE_TRANSFORM (INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL0 | INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL1 | INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL2 | INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL3)
#define INSTANCE_FORMAT_TRANSFORM INSTANCE_ATTRIBUTE_TRANSFORM


typedef enum RenderTextureType {
	RENDER_TEXTURE_NONE = -1,

	RENDER_TEXTURE_SWAPCHAIN_DEPTH,
	RENDER_TEXTURE_SWAPCHAIN_COLOR0,
	RENDER_TEXTURE_SWAPCHAIN_COLOR1,
	RENDER_TEXTURE_SWAPCHAIN_COLOR2,
	
	RENDER_TEXTURE_SHADOWMAP_DEPTH,
	RENDER_TEXTURE_SHADOWMAP_COLOR,
	RENDER_TEXTURE_SHADOWMAP_COLOR_BLUR,

	RENDER_TEXTURE_COUNT
} RenderTextureType;


extern const uint32_t PASS_DEFAULT_NAME;
extern const uint32_t PASS_FORWARD_LIGHTING_NAME;
extern const uint32_t PASS_SHADOWMAP_NAME;
extern const uint32_t PASS_SHADOWMAP_BLUR_NAME;
extern const uint32_t PASS_COLOR_GRADING_NAME;


class CALL_API CRenderSystem
{
	friend class CEngine;
	friend class CPassDefault;
	friend class CPassForwardLighting;
	friend class CPassShadowMap;
	friend class CPassShadowMapBlur;
	friend class CPassColorGrading;


private:
	CRenderSystem(GfxApi api, void* hInstance, void* hWnd, void* hDC, int width, int height, GfxPixelFormat format);
	virtual ~CRenderSystem(void);


private:
	CGfxUniformEngine* GetEngineUniform(void) const;

private:
	void CreateRenderTexture(void);
	void DestroyRenderTexture(void);
	CGfxRenderTexturePtr GetRenderTexture(RenderTextureType type);

private:
	void CreateRenderPass(void);
	void DestroyRenderPass(void);

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

	void SetMainShadowOrtho(int indexLevel, float left, float right, float bottom, float top, float zNear, float zFar);
	void SetMainShadowLookat(int indexLevel, float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz);

private:
	void Update(CTaskGraph& taskGraph, CCamera* pCamera) const;
	void RenderDefault(CTaskGraph& taskGraph, CCamera* pCamera, bool bPresent) const;
	void RenderForwardLighting(CTaskGraph& taskGraph, CCamera* pCamera, bool bShadow, bool bPresent) const;


private:
	CGfxRenderer* m_pRenderer;
	CGfxUniformEngine* m_pEngineUniform;

private:
	CPassDefault* m_pPassDefault;
	CPassForwardLighting* m_pPassForwardLighting;
	CPassShadowMap* m_pPassShadowMap;
	CPassShadowMapBlur* m_pPassShadowMapBlur;

private:
	CGfxRenderTexturePtr m_ptrRenderTextures[RENDER_TEXTURE_COUNT];
};
