#pragma once
#include "PreHeader.h"


// Vertex&Instance Attributes
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


// Uniform Base
#define UNIFORM_ENGINE_BIND                                0
#define UNIFORM_CAMERA_BIND                                1

#define UNIFORM_ENGINE_NAME                                HashValue("Engine")
#define UNIFORM_CAMERA_NAME                                HashValue("Camera")

// Uniform Forward/Deferred
#define UNIFORM_DEPTH_TEXTURE_BIND                         2
#define UNIFORM_COLOR_TEXTURE_BIND                         2
#define UNIFORM_ALBEDO_AO_TEXTURE_BIND                     3
#define UNIFORM_NORMAL_ROUGHNESS_METALLIC_TEXTURE_BIND     4

#define UNIFORM_DEPTH_TEXTURE_NAME                         HashValue("texDepth")
#define UNIFORM_COLOR_TEXTURE_NAME                         HashValue("texColor")
#define UNIFORM_ALBEDO_AO_TEXTURE_NAME                     HashValue("texAlbedoAO")
#define UNIFORM_NORMAL_ROUGHNESS_METALLIC_TEXTURE_NAME     HashValue("texNormalRoughnessMetallic")

// Uniform PostProcessing
#define UNIFORM_SHADOW_TEXTURE_BIND                        5
#define UNIFORM_SSAO_TEXTURE_BIND                          6

#define UNIFORM_SHADOW_TEXTURE_NAME                        HashValue("texShadow")
#define UNIFORM_SSAO_TEXTURE_NAME                          HashValue("texSSAO")


// Pass
#define PASS_PREZ_NAME                                     HashValue("PassPreZ")
#define PASS_SSAO_NAME                                     HashValue("PassSSAO")
#define PASS_SHADOW_NAME                                   HashValue("PassShadow")
#define PASS_DEFAULT_NAME                                  HashValue("PassDefault")
#define PASS_FORWARD_LIGHTING_NAME                         HashValue("PassForwardLighting")
#define PASS_COLOR_GRADING_NAME                            HashValue("PassColorGrading")
#define PASS_FINAL_NAME                                    HashValue("PassFinal")

#define PASS_SSAO_MATERIAL_NAME                            "PassSSAO.material"
#define PASS_COLOR_GRADING_MATERIAL_NAME                   "PassColorGrading.material"
#define PASS_FINAL_MATERIAL_NAME                           "PassFinal.material"


// RenderTexture
#define RENDER_TEXTURE_SWAPCHAIN_COLOR0                    HashValue("RenderTextureSwapChainColor0")
#define RENDER_TEXTURE_SWAPCHAIN_COLOR1                    HashValue("RenderTextureSwapChainColor1")
#define RENDER_TEXTURE_SWAPCHAIN_COLOR2                    HashValue("RenderTextureSwapChainColor2")
#define RENDER_TEXTURE_SHADOW                              HashValue("RenderTextureShadow")
#define RENDER_TEXTURE_FRAMEBUFFER_DEPTH                   HashValue("RenderTextureFrameBufferDepth")
#define RENDER_TEXTURE_FRAMEBUFFER_COLOR                   HashValue("RenderTextureFrameBufferColor")
#define RENDER_TEXTURE_FRAMEBUFFER_SSAO                    HashValue("RenderTextureFrameBufferHalf")
#define RENDER_TEXTURE_FRAMEBUFFER_FINAL                   HashValue("RenderTextureFrameBufferFinal")


class CALL_API CRenderSystem
{
	friend class CEngine;
	friend class CPassPreZ;
	friend class CPassSSAO;
	friend class CPassShadow;
	friend class CPassDefault;
	friend class CPassForwardLighting;
	friend class CPassColorGrading;
	friend class CPassFinal;


private:
	CRenderSystem(GfxApi api, void* hInstance, void* hWnd, void* hDC, int width, int height, GfxPixelFormat format);
	virtual ~CRenderSystem(void);


private:
	CGfxUniformEngine* GetEngineUniform(void) const;

private:
	void CreateRenderPass(void);
	void DestroyRenderPass(void);

public:
	void CreateRenderTexture(uint32_t name, CGfxRenderTexturePtr ptrRenderTexture);
	void CreateRenderTexture(uint32_t name, GfxPixelFormat format, int width, int height, int samples = 1, bool bTransient = false);
	void DestroyRenderTexture(void);
	CGfxRenderTexturePtr GetRenderTexture(uint32_t name);

public:
	CPassPreZ* GetPassPreZ(void) const;
	CPassSSAO* GetPassSSAO(void) const;
	CPassShadow* GetPassShadow(void) const;
	CPassDefault* GetPassDefault(void) const;
	CPassForwardLighting* GetPassForwardLighting(void) const;
	CPassColorGrading* GetPassColorGrading(void) const;
	CPassFinal* GetPassFinal(void) const;

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
	CPassPreZ* m_pPassPreZ;
	CPassSSAO* m_pPassSSAO;
	CPassShadow* m_pPassShadow;
	CPassDefault* m_pPassDefault;
	CPassForwardLighting* m_pPassForwardLighting;
	CPassColorGrading* m_pPassColorGrading;
	CPassFinal* m_pPassFinal;

private:
	eastl::unordered_map<uint32_t, CGfxRenderTexturePtr> m_ptrRenderTextures;

private:
	CGfxCommandBufferPtr m_ptrCommandBuffer[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];
};
