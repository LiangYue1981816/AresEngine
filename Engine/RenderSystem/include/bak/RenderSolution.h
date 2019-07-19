#pragma once
#include "PreHeader.h"
#include "PassDefault.h"
#include "PassForwardLighting.h"
#include "PassShadow.h"


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


typedef enum RenderSolution {
	RENDER_SOLUTION_DEFAULT = 0,
	RENDER_SOLUTION_FORWARD,
	RENDER_SOLUTION_DEFERRED,
	RENDER_SOLUTION_TILED_BASE_DEFERRED,
	RENDER_SOLUTION_COUNT
} RenderSolution;


class CALL_API CRenderSolution
{
	friend class CEngine;


private:
	CRenderSolution(GfxApi api, RenderSolution solution, void* hInstance, void* hWnd, void* hDC, int width, int height, GfxPixelFormat format);
	virtual ~CRenderSolution(void);


private:
	void CreateAttachments(void);
	void DestroyAttachments(void);

public:
	CGfxCamera* GetMainCamera(void) const;
	CGfxRenderQueue* GetMainCameraQueue(void) const;

	CGfxUniformEngine* GetEngineUniform(void) const;
	CGfxUniformCamera* GetMainCameraUniform(void) const;
	CGfxUniformCamera* GetShadowCameraUniform(int indexLevel) const;

public:
	CGfxRenderTexturePtr GetPresentColorTexture(int indexFrame) const;
	CGfxRenderTexturePtr GetPresentDepthStencilTexture(int indexFrame) const;
	CGfxRenderTexturePtr GetPresentColorTextureMSAA(int indexFrame) const;
	CGfxRenderTexturePtr GetPresentDepthStencilTextureMSAA(int indexFrame) const;

	CGfxRenderTexturePtr GetShadowMapTexture(void) const;

public:
	void SetTime(float t, float dt);

	void SetCameraScissor(float x, float y, float width, float height);
	void SetCameraViewport(float x, float y, float width, float height);
	void SetCameraPerspective(float fovy, float aspect, float zNear, float zFar);
	void SetCameraOrtho(float left, float right, float bottom, float top, float zNear, float zFar);
	void SetCameraLookat(float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz);

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

public:
	void SetRenderSolution(RenderSolution solution);
	void SetEnableMSAA(bool bEnable);

public:
	void UpdateCamera(int indexQueue);
	void Render(int indexQueue);

private:
	void UpdateDefault(void);
	void UpdateForward(void);
	void UpdateDeferred(void);
	void UpdateTiledBaseDeferred(void);

private:
	void RenderDefault(int indexQueue);
	void RenderForward(int indexQueue);
	void RenderDeferred(int indexQueue);
	void RenderTiledBaseDeferred(int indexQueue);


private:
	CGfxRenderer* m_pRenderer;

private:
	CGfxCamera* m_pMainCamera;
	CGfxRenderQueue* m_pMainCameraQueue;

	CGfxUniformEngine* m_pEngineUniform;
	CGfxUniformCamera* m_pMainCameraUniform;
	CGfxUniformCamera* m_pShadowCameraUniform[4];

private:
	CGfxRenderTexturePtr m_ptrPresentColorTexture[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];
	CGfxRenderTexturePtr m_ptrPresentDepthStencilTexture[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];
	CGfxRenderTexturePtr m_ptrPresentColorTextureMSAA[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];
	CGfxRenderTexturePtr m_ptrPresentDepthStencilTextureMSAA[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];

	CGfxRenderTexturePtr m_ptrShadowMapTexture;

private:
	bool m_bEnableMSAA;
	RenderSolution m_solution;

	CPassDefault* m_pPassDefault;
	CPassForwardLighting* m_pPassForwardLighting;
	CPassShadow* m_pPassShadow;
};

extern const uint32_t DEFAULT_PASS_NAME;
extern const uint32_t FORWARD_LIGHTING_PASS_NAME;
extern const uint32_t SHADOW_PASS_NAME;
