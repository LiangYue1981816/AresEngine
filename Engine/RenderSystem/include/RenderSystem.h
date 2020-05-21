#pragma once
#include "PreHeader.h"

#include "RenderSystem/include/PassBase.h"
#include "RenderSystem/include/PassBlit.h"
#include "RenderSystem/include/PassPreZ.h"
#include "RenderSystem/include/PassShadow.h"
#include "RenderSystem/include/PassUnlit.h"
#include "RenderSystem/include/PassForwardShading.h"
#include "RenderSystem/include/PassDeferredShading.h"
#include "RenderSystem/include/PassTileDeferredShading.h"
#include "RenderSystem/include/PassCopyColor.h"
#include "RenderSystem/include/PassCopyDepthStencil.h"
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
#define STORAGE_TRANSFER_SCENE_DATA_INDEX_BIND             4
#define STORAGE_CLUSTER_DATA_BIND                          5
#define STORAGE_FULL_LIGHT_LIST_DATA_BIND                  6
#define STORAGE_CULL_LIGHT_LIST_DATA_BIND                  7

#define UNIFORM_ENGINE_NAME                                HashValue("Engine")
#define UNIFORM_CAMERA_NAME                                HashValue("Camera")
#define STORAGE_SCENE_DATA_NAME                            HashValue("SceneData")
#define STORAGE_TRANSFER_SCENE_DATA_NAME                   HashValue("TransferSceneData")
#define STORAGE_TRANSFER_SCENE_DATA_INDEX_NAME             HashValue("TransferSceneDataIndex")
#define STORAGE_CLUSTER_DATA_NAME                          HashValue("ClusterData")
#define STORAGE_FULL_LIGHT_LIST_DATA_NAME                  HashValue("FullLightListData")
#define STORAGE_CULL_LIGHT_LIST_DATA_NAME                  HashValue("CullLightListData")

// Uniform Forward/Deferred
#define UNIFORM_DEPTH_TEXTURE_BIND                         8
#define UNIFORM_COLOR_TEXTURE_BIND                         9
#define UNIFORM_GBUFFER0_TEXTURE_BIND                      10
#define UNIFORM_GBUFFER1_TEXTURE_BIND                      11
#define UNIFORM_GBUFFER2_TEXTURE_BIND                      12
#define UNIFORM_GBUFFER3_TEXTURE_BIND                      13

#define UNIFORM_DEPTH_TEXTURE_NAME                         HashValue("texDepth")
#define UNIFORM_COLOR_TEXTURE_NAME                         HashValue("texColor")
#define UNIFORM_GBUFFER0_TEXTURE_NAME                      HashValue("texGBuffer0")
#define UNIFORM_GBUFFER1_TEXTURE_NAME                      HashValue("texGBuffer1")
#define UNIFORM_GBUFFER2_TEXTURE_NAME                      HashValue("texGBuffer2")
#define UNIFORM_GBUFFER3_TEXTURE_NAME                      HashValue("texGBuffer3")

// Uniform PostProcessing
#define UNIFORM_SHADOW_TEXTURE_BIND                        16
#define UNIFORM_SSAO_TEXTURE_BIND                          17
#define UNIFORM_ADD_TEXTURE_BIND                           18

#define UNIFORM_SHADOW_TEXTURE_NAME                        HashValue("texShadow")
#define UNIFORM_SSAO_TEXTURE_NAME                          HashValue("texSSAO")
#define UNIFORM_ADD_TEXTURE_NAME                           HashValue("texAdd")


// Pass
#define PASS_PREZ_NAME                                     HashValue("PassPreZ")
#define PASS_SHADOW_NAME                                   HashValue("PassShadow")

#define PASS_UNLIT_NAME                                    HashValue("PassUnlit")
#define PASS_FORWARD_SHADING_NAME                          HashValue("PassForwardShading")
#define PASS_DEFERRED_SHADING_NAME                         HashValue("PassDeferredShading")
#define PASS_DEFERRED_SHADING_GBUFFER_NAME                 HashValue("PassDeferredShading_GBuffer")
#define PASS_DEFERRED_SHADING_LIGHTING_NAME                HashValue("PassDeferredShading_Lighting")
#define PASS_TILE_DEFERRED_SHADING_NAME                    HashValue("PassTileDeferredShading")
#define PASS_TILE_DEFERRED_SHADING_GBUFFER_NAME            HashValue("PassTileDeferredShading_GBuffer")
#define PASS_TILE_DEFERRED_SHADING_LIGHTING_NAME           HashValue("PassTileDeferredShading_Lighting")

#define PASS_COPY_COLOR_NAME                               HashValue("PassCopyColor")
#define PASS_COPY_DEPTH_STENCIL_NAME                       HashValue("PassCopyDepthStencil")

#define PASS_SSAO_NAME                                     HashValue("PassSSAO")
#define PASS_BLUR_BOX_NAME                                 HashValue("PassBlurBox")
#define PASS_BLUR_HORIZONTAL_NAME                          HashValue("PassBlurHorizontal")
#define PASS_BLUR_VERTICAL_NAME                            HashValue("PassBlurVertical")
#define PASS_BLEND_ADD_NAME                                HashValue("PassBlendAdd")
#define PASS_LUMINANCE_THRESHOLD_NAME                      HashValue("PassLuminanceThreshold")
#define PASS_COLOR_GRADING_NAME                            HashValue("PassColorGrading")
#define PASS_FINAL_NAME                                    HashValue("PassFinal")

// Pass Material
#define PASS_COPY_COLOR_MATERIAL_NAME                      "PassCopyColor.material"
#define PASS_COPY_DEPTH_STENCIL_MATERIAL_NAME              "PassCopyDepthStencil.material"

#define PASS_TILE_DEFERRED_SHADING_MATERIAL_NAME           "PassTileDeferredShading.material"
#define PASS_SSAO_MATERIAL_NAME                            "PassSSAO.material"
#define PASS_BLUR_BOX_MATERIAL_NAME                        "PassBlurBox.material"
#define PASS_BLUR_HORIZONTAL_MATERIAL_NAME                 "PassBlurHorizontal.material"
#define PASS_BLUR_VERTICAL_MATERIAL_NAME                   "PassBlurVertical.material"
#define PASS_BLEND_ADD_MATERIAL_NAME                       "PassBlendAdd.material"
#define PASS_LUMINANCE_THRESHOLD_MATERIAL_NAME             "PassLuminanceThreshold.material"
#define PASS_COLOR_GRADING_MATERIAL_NAME                   "PassColorGrading.material"
#define PASS_FINAL_MATERIAL_NAME                           "PassFinal.material"


// Render Texture
#define RENDER_TEXTURE_SWAPCHAIN_COLOR0                    0
#define RENDER_TEXTURE_SWAPCHAIN_COLOR1                    1
#define RENDER_TEXTURE_SWAPCHAIN_COLOR2                    2
#define RENDER_TEXTURE_GBUFFER0                            HashValue("RenderTextureGBuffer0")
#define RENDER_TEXTURE_GBUFFER1                            HashValue("RenderTextureGBuffer1")
#define RENDER_TEXTURE_GBUFFER2                            HashValue("RenderTextureGBuffer2")
#define RENDER_TEXTURE_GBUFFER3                            HashValue("RenderTextureGBuffer3")
#define RENDER_TEXTURE_SHADOW                              HashValue("RenderTextureShadow")
#define RENDER_TEXTURE_FULL_DEPTH                          HashValue("RenderTextureFullDepth")
#define RENDER_TEXTURE_FULL_DEPTH_COPY                     HashValue("RenderTextureFullDepthCopy")
#define RENDER_TEXTURE_FULL_HDR_COLOR0                     HashValue("RenderTextureFullHdrColor0")
#define RENDER_TEXTURE_FULL_HDR_COLOR1                     HashValue("RenderTextureFullHdrColor1")
#define RENDER_TEXTURE_QUATER_HDR_COLOR0                   HashValue("RenderTextureQuaterHdrColor0")
#define RENDER_TEXTURE_QUATER_HDR_COLOR1                   HashValue("RenderTextureQuaterHdrColor1")


// GPUScene
#define MAX_GPUSCENE_INSTANCE_COUNT                        (32 * 1024)
#define MAX_GPUSCENE_TRANSFER_COUNT                        (1  * 1024)

// Cluster
#define CLUSTER_HORIZONTAL_TILE_COUNT                      80
#define CLUSTER_VERTICAL_TILE_COUNT                        45
#define CLUSTER_DEPTH_TILE_COUNT                           8


class CALL_API CRenderSystem
{
	friend class CEngine;
	friend class CGPUScene;
	friend class CGPUCluster;
	friend class CPassBase;
	friend class CPassBlit;
	friend class CPassPreZ;
	friend class CPassShadow;
	friend class CPassUnlit;
	friend class CPassForwardShading;
	friend class CPassDeferredShading;
	friend class CPassTileDeferredShading;
	friend class CPassCopyColor;
	friend class CPassCopyDepthStencil;
	friend class CPassSSAO;
	friend class CPassBlurHorizontal;
	friend class CPassBlurVertical;
	friend class CPassBlendAdd;
	friend class CPassLuminanceThreshold;
	friend class CPassColorGrading;
	friend class CPassFinal;


private:
	CRenderSystem(void);
	virtual ~CRenderSystem(void);


private:
	CGPUScene* GetScene(void) const;
	CGPUCluster* GetCluster(void) const;
	CUniformEngine* GetEngineUniform(void) const;

private:
	void Create(GfxApi api, void* hInstance, void* hWnd, void* hDC, int width, int height, GfxPixelFormat format);
	void CreatePasses(void);
	void CreateCommandBuffers(void);
	void CreateRenderTextures(void);
	void CreateRenderTexture(uint32_t name, CGfxRenderTexturePtr ptrRenderTexture);
	void CreateRenderTexture(uint32_t name, GfxPixelFormat format, int width, int height, int samples = 1, bool bTransient = false);

	void Destroy(void);
	void DestroyPasses(void);
	void DestroyCommandBuffers(void);
	void DestroyRenderTextures(void);

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
	void UpdateCamera(CTaskPool& taskPool, CTaskGraph& taskGraph, CCamera* pCamera, uint32_t mask, bool bComputeLOD);
	void UpdateScene(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer);
	void UpdateCluster(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer, CCamera* pCamera);

private:
	void RenderUnlit(CTaskPool& taskPool, CTaskGraph& taskGraph, CCamera* pCamera, bool bPresent);
	void RenderUnlit(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer, CCamera* pCamera, bool bPresent);

	void RenderForwardShading(CTaskPool& taskPool, CTaskGraph& taskGraph, CCamera* pCamera, bool bPresent);
	void RenderForwardShading(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer, CCamera* pCamera, bool bPresent);

	void RenderDeferredShading(CTaskPool& taskPool, CTaskGraph& taskGraph, CCamera* pCamera, bool bPresent);
	void RenderDeferredShading(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer, CCamera* pCamera, bool bPresent);

	void RenderTileDeferredShading(CTaskPool& taskPool, CTaskGraph& taskGraph, CCamera* pCamera, bool bPresent);
	void RenderTileDeferredShading(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer, CCamera* pCamera, bool bPresent);

private:
	void InternalPassCopyColor(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer, CCamera* pCamera, uint32_t rtInColor, uint32_t rtOutColor);
	void InternalPassCopyDepthStencil(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer, CCamera* pCamera, uint32_t rtInDepth, uint32_t rtOutDepth);
	void InternalPassPreZ(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer, CCamera* pCamera, uint32_t rtOutDepth);
	void InternalPassSSAO(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer, CCamera* pCamera, uint32_t rtInDepth, uint32_t rtOutSSAO, uint32_t rtTempBlur);
	void InternalPassShadow(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer, CCamera* pCamera, uint32_t rtOutShadow);
	void InternalPassForwardLighting(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer, CCamera* pCamera, uint32_t rtInShadow, uint32_t rtInSSAO, uint32_t rtOutColor, uint32_t rtOutDepth);
	void InternalPassDeferredLighting(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer, CCamera* pCamera, uint32_t rtInDepth, uint32_t rtInShadow, uint32_t rtInSSAO, uint32_t rtOutColor, uint32_t rtOutDepth, uint32_t rtOutGBuffer0, uint32_t rtOutGBuffer1);
	void InternalPassTileDeferredLighting(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer, CCamera* pCamera, uint32_t rtInDepth, uint32_t rtInShadow, uint32_t rtInSSAO, uint32_t rtOutColor, uint32_t rtOutDepth, uint32_t rtOutGBuffer0, uint32_t rtOutGBuffer1);
	void InternalPassBloom(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer, CCamera* pCamera, uint32_t rtInColor, uint32_t rtOutBloom, uint32_t rtTempThreshold, uint32_t rtTempBlur0, uint32_t rtTempBlur1);
	void InternalPassColorGrading(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer, CCamera* pCamera, uint32_t rtInColor, uint32_t rtOutColor);
	void InternalPassFinal(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer, CCamera* pCamera, uint32_t rtInColor, bool bPresent);


private:
	CGfxRenderer* m_pRenderer;
	CGPUScene* m_pGPUScene;
	CGPUCluster* m_pGPUCluster;
	CUniformEngine* m_pEngineUniform;
	CInstanceBufferPool* m_pInstanceBufferPool;

private:
	CPassPreZ* m_pPassPreZ;
	CPassShadow* m_pPassShadow;
	CPassUnlit* m_pPassUnlit;
	CPassForwardShading* m_pPassForwardShading;
	CPassDeferredShading* m_pPassDeferredShading;
	CPassTileDeferredShading* m_pPassTileDeferredShading;
	CPassCopyColor* m_pPassCopyColor;
	CPassCopyDepthStencil* m_pPassCopyDepthStencil;
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
