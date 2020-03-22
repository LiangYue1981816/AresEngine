#pragma once
#include "PreHeader.h"

#include "RenderSystem/include/PassBase.h"
#include "RenderSystem/include/PassPreZ.h"
#include "RenderSystem/include/PassShadow.h"
#include "RenderSystem/include/PassDefault.h"
#include "RenderSystem/include/PassForwardLighting.h"
#include "RenderSystem/include/PassDeferredLighting.h"
#include "RenderSystem/include/PassBlit.h"
#include "RenderSystem/include/PassCopy.h"
#include "RenderSystem/include/PassSSAO.h"
#include "RenderSystem/include/PassBlurBox.h"
#include "RenderSystem/include/PassBlurHorizontal.h"
#include "RenderSystem/include/PassBlurVertical.h"
#include "RenderSystem/include/PassBlendAdd.h"
#include "RenderSystem/include/PassLuminanceThreshold.h"
#include "RenderSystem/include/PassColorGrading.h"
#include "RenderSystem/include/PassFinal.h"


// Vertex&Instance Attributes
#define VERTEX_ATTRIBUTE_POSITION                          0x00000001
#define VERTEX_ATTRIBUTE_NORMAL                            0x00000002
#define VERTEX_ATTRIBUTE_BINORMAL                          0x00000004
#define VERTEX_ATTRIBUTE_COLOR                             0x00000008
#define VERTEX_ATTRIBUTE_TEXCOORD0                         0x00000010
#define VERTEX_ATTRIBUTE_TEXCOORD1                         0x00000020
#define VERTEX_ATTRIBUTE_COUNT                             6
#define VERTEX_BINDING                                     0

#define INSTANCE_ATTRIBUTE_INDEX                           0x00000001
#define INSTANCE_ATTRIBUTE_COUNT                           1
#define INSTANCE_FORMAT                                    INSTANCE_ATTRIBUTE_INDEX
#define INSTANCE_BINDING                                   1


// Uniform Base
#define UNIFORM_ENGINE_BIND                                0
#define UNIFORM_CAMERA_BIND                                1
#define STORAGE_SCENE_DATA_BIND                            2
#define STORAGE_TRANSFER_SCENE_DATA_BIND                   3

#define UNIFORM_ENGINE_NAME                                HashValue("Engine")
#define UNIFORM_CAMERA_NAME                                HashValue("Camera")
#define STORAGE_SCENE_DATA_NAME                            HashValue("SceneData")
#define STORAGE_TRANSFER_SCENE_DATA_NAME                   HashValue("TransferSceneData")

// Uniform Forward/Deferred
#define UNIFORM_DEPTH_TEXTURE_BIND                         4
#define UNIFORM_COLOR_TEXTURE_BIND                         5
#define UNIFORM_GBUFFER0_TEXTURE_BIND                      6
#define UNIFORM_GBUFFER1_TEXTURE_BIND                      7
#define UNIFORM_GBUFFER2_TEXTURE_BIND                      8
#define UNIFORM_GBUFFER3_TEXTURE_BIND                      9

#define UNIFORM_DEPTH_TEXTURE_NAME                         HashValue("texDepth")
#define UNIFORM_COLOR_TEXTURE_NAME                         HashValue("texColor")
#define UNIFORM_GBUFFER0_TEXTURE_NAME                      HashValue("texGBuffer0")
#define UNIFORM_GBUFFER1_TEXTURE_NAME                      HashValue("texGBuffer1")
#define UNIFORM_GBUFFER2_TEXTURE_NAME                      HashValue("texGBuffer2")
#define UNIFORM_GBUFFER3_TEXTURE_NAME                      HashValue("texGBuffer3")

// Uniform PostProcessing
#define UNIFORM_SHADOW_TEXTURE_BIND                        10
#define UNIFORM_SSAO_TEXTURE_BIND                          11
#define UNIFORM_ADD_TEXTURE_BIND                           12

#define UNIFORM_SHADOW_TEXTURE_NAME                        HashValue("texShadow")
#define UNIFORM_SSAO_TEXTURE_NAME                          HashValue("texSSAO")
#define UNIFORM_ADD_TEXTURE_NAME                           HashValue("texAdd")


// Pass
#define PASS_PREZ_NAME                                     HashValue("PassPreZ")
#define PASS_SHADOW_NAME                                   HashValue("PassShadow")
#define PASS_DEFAULT_NAME                                  HashValue("PassDefault")
#define PASS_FORWARD_LIGHTING_NAME                         HashValue("PassForwardLighting")
#define PASS_DEFERRED_LIGHTING_NAME                        HashValue("PassDeferredLighting")
#define PASS_DEFERRED_LIGHTING_GBUFFER_NAME                HashValue("PassDeferredLighting_GBuffer")
#define PASS_DEFERRED_LIGHTING_SHADING_NAME                HashValue("PassDeferredLighting_Shading")
#define PASS_DEFERRED_LIGHTING_SHADOW_NAME                 HashValue("PassDeferredLighting_Shadow")
#define PASS_COPY_NAME                                     HashValue("PassCopy")
#define PASS_SSAO_NAME                                     HashValue("PassSSAO")
#define PASS_BLUR_BOX_NAME                                 HashValue("PassBlurBox")
#define PASS_BLUR_HORIZONTAL_NAME                          HashValue("PassBlurHorizontal")
#define PASS_BLUR_VERTICAL_NAME                            HashValue("PassBlurVertical")
#define PASS_BLEND_ADD_NAME                                HashValue("PassBlendAdd")
#define PASS_LUMINANCE_THRESHOLD_NAME                      HashValue("PassLuminanceThreshold")
#define PASS_COLOR_GRADING_NAME                            HashValue("PassColorGrading")
#define PASS_FINAL_NAME                                    HashValue("PassFinal")

#define PASS_COPY_MATERIAL_NAME                            "PassCopy.material"
#define PASS_SSAO_MATERIAL_NAME                            "PassSSAO.material"
#define PASS_BLUR_BOX_MATERIAL_NAME                        "PassBlurBox.material"
#define PASS_BLUR_HORIZONTAL_MATERIAL_NAME                 "PassBlurHorizontal.material"
#define PASS_BLUR_VERTICAL_MATERIAL_NAME                   "PassBlurVertical.material"
#define PASS_BLEND_ADD_MATERIAL_NAME                       "PassBlendAdd.material"
#define PASS_LUMINANCE_THRESHOLD_MATERIAL_NAME             "PassLuminanceThreshold.material"
#define PASS_COLOR_GRADING_MATERIAL_NAME                   "PassColorGrading.material"
#define PASS_FINAL_MATERIAL_NAME                           "PassFinal.material"


// RenderTexture
#define RENDER_TEXTURE_SWAPCHAIN_COLOR0                    0
#define RENDER_TEXTURE_SWAPCHAIN_COLOR1                    1
#define RENDER_TEXTURE_SWAPCHAIN_COLOR2                    2
#define RENDER_TEXTURE_GBUFFER0                            HashValue("RenderTextureGBuffer0")
#define RENDER_TEXTURE_GBUFFER1                            HashValue("RenderTextureGBuffer1")
#define RENDER_TEXTURE_GBUFFER2                            HashValue("RenderTextureGBuffer2")
#define RENDER_TEXTURE_GBUFFER3                            HashValue("RenderTextureGBuffer3")
#define RENDER_TEXTURE_SHADOW                              HashValue("RenderTextureShadow")
#define RENDER_TEXTURE_FULL_DEPTH                          HashValue("RenderTextureFullDepth")
#define RENDER_TEXTURE_FULL_HDR_COLOR0                     HashValue("RenderTextureFullHdrColor0")
#define RENDER_TEXTURE_FULL_HDR_COLOR1                     HashValue("RenderTextureFullHdrColor1")
#define RENDER_TEXTURE_QUATER_HDR_COLOR0                   HashValue("RenderTextureQuaterHdrColor0")
#define RENDER_TEXTURE_QUATER_HDR_COLOR1                   HashValue("RenderTextureQuaterHdrColor1")


class CALL_API CRenderSystem
{
	friend class CEngine;
	friend class CPassBase;
	friend class CPassPreZ;
	friend class CPassShadow;
	friend class CPassDefault;
	friend class CPassForwardLighting;
	friend class CPassDeferredLighting;
	friend class CPassBlit;
	friend class CPassCopy;
	friend class CPassSSAO;
	friend class CPassBlurBox;
	friend class CPassBlurHorizontal;
	friend class CPassBlurVertical;
	friend class CPassBlendAdd;
	friend class CPassLuminanceThreshold;
	friend class CPassColorGrading;
	friend class CPassFinal;


private:
	CRenderSystem(GfxApi api, void* hInstance, void* hWnd, void* hDC, int width, int height, GfxPixelFormat format);
	virtual ~CRenderSystem(void);


private:
	CGPUScene* GetGPUScene(void) const;
	CUniformEngine* GetEngineUniform(void) const;

private:
	void CreatePasses(void);
	void DestroyPasses(void);

private:
	void CreateRenderTextures(void);
	void DestroyRenderTextures(void);

	void CreateRenderTexture(uint32_t name, CGfxRenderTexturePtr ptrRenderTexture);
	void CreateRenderTexture(uint32_t name, GfxPixelFormat format, int width, int height, int samples = 1, bool bTransient = false);

	CGfxRenderTexturePtr GetRenderTexture(uint32_t name) const;

public:
	int AddInstance(void);
	void RemoveInstance(int index);
	void ModifyInstanceData(int index, const InstanceData &data, int indexThread = 0);

	int GetDefaultInstanceIndex(void) const;
	int GetPostProcessInstnaceIndex(void) const;

	const InstanceData& GetInstanceData(int index);
	const CGfxMultiInstanceBufferPtr GetInstanceBuffer(uint32_t instanceFormat, int instanceBinding);

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
	void UpdateCamera(CTaskGraph& taskGraph, CCamera* pCamera, uint32_t mask, bool bComputeLOD);
	void UpdateGPUScene(CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer);

private:
	void RenderDefault(CTaskGraph& taskGraph, CCamera* pCamera, bool bPresent);
	void RenderDefault(CTaskGraph& taskGraph, CCamera* pCamera, bool bPresent, CGfxCommandBufferPtr ptrCommandBuffer);

	void RenderForwardLighting(CTaskGraph& taskGraph, CCamera* pCamera, bool bPresent);
	void RenderForwardLighting(CTaskGraph& taskGraph, CCamera* pCamera, bool bPresent, CGfxCommandBufferPtr ptrCommandBuffer);

	void RenderDeferredLighting(CTaskGraph& taskGraph, CCamera* pCamera, bool bPresent);
	void RenderDeferredLighting(CTaskGraph& taskGraph, CCamera* pCamera, bool bPresent, CGfxCommandBufferPtr ptrCommandBuffer);

	void RenderTileBaseDeferredLighting(CTaskGraph& taskGraph, CCamera* pCamera, bool bPresent);
	void RenderTileBaseDeferredLighting(CTaskGraph& taskGraph, CCamera* pCamera, bool bPresent, CGfxCommandBufferPtr ptrCommandBuffer);


private:
	CGfxRenderer* m_pRenderer;
	CGPUScene* m_pGPUScene;
	CUniformEngine* m_pEngineUniform;
	CInstanceBufferPool* m_pInstanceBufferPool;

private:
	CPassPreZ* m_pPassPreZ;
	CPassShadow* m_pPassShadow;
	CPassDefault* m_pPassDefault;
	CPassForwardLighting* m_pPassForwardLighting;
	CPassDeferredLighting* m_pPassDeferredLighting;
	CPassSSAO* m_pPassSSAO;
	CPassBlurHorizontal* m_pPassSSAOBlurHorizontal;
	CPassBlurVertical* m_pPassSSAOBlurVertical;
	CPassLuminanceThreshold* m_pPassBloomLuminanceThreshold;
	CPassBlurHorizontal* m_pPassBloomBlurHorizontal;
	CPassBlurVertical* m_pPassBloomBlurVertical;
	CPassBlendAdd* m_pPassBloomBlendAdd;
	CPassColorGrading* m_pPassColorGrading;
	CPassFinal* m_pPassFinal;

private:
	eastl::unordered_map<uint32_t, CGfxRenderTexturePtr> m_ptrRenderTextures;

private:
	CGfxCommandBufferPtr m_ptrComputeCommandBuffer[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];
	CGfxCommandBufferPtr m_ptrGraphicCommandBuffer[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];
};
