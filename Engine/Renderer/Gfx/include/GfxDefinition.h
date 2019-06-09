#pragma once


class CALL_API CGfxRenderer;

class CALL_API CGfxProfiler;
class CALL_API CGfxProfilerSample;

class CALL_API CGfxResource;
class CALL_API CGfxResourceManager;

class CALL_API CGfxCamera;
class CALL_API CGfxRenderQueue;

class CALL_API CGfxSwapChain;
class CALL_API CGfxRenderPass;
class CALL_API CGfxFrameBuffer;

class CALL_API CGfxShader;
class CALL_API CGfxPipelineCompute;
class CALL_API CGfxPipelineGraphics;

class CALL_API CGfxIndexBuffer;
class CALL_API CGfxVertexBuffer;
class CALL_API CGfxInstanceBuffer;
class CALL_API CGfxIndirectBuffer;

class CALL_API CGfxSampler;
class CALL_API CGfxTexture2D;
class CALL_API CGfxTexture2DArray;
class CALL_API CGfxTextureCubemap;
class CALL_API CGfxRenderTexture;

class CALL_API CGfxUniformVec1;
class CALL_API CGfxUniformVec2;
class CALL_API CGfxUniformVec3;
class CALL_API CGfxUniformVec4;
class CALL_API CGfxUniformMat4;
class CALL_API CGfxUniformEngine;
class CALL_API CGfxUniformCamera;
class CALL_API CGfxUniformBuffer;

class CALL_API CGfxMesh;
class CALL_API CGfxMeshDraw;

class CALL_API CGfxMaterial;
class CALL_API CGfxMaterialPass;

class CALL_API CGfxDescriptorSet;
class CALL_API CGfxDescriptorLayout;

class CALL_API CGfxCommandBuffer;

template<class T>
class CALL_API CGfxResourcePtr;

typedef CGfxResourcePtr<CGfxRenderPass> CGfxRenderPassPtr;
typedef CGfxResourcePtr<CGfxFrameBuffer> CGfxFrameBufferPtr;

typedef CGfxResourcePtr<CGfxTexture2D> CGfxTexture2DPtr;
typedef CGfxResourcePtr<CGfxTexture2DArray> CGfxTexture2DArrayPtr;
typedef CGfxResourcePtr<CGfxTextureCubemap> CGfxTextureCubemapPtr;
typedef CGfxResourcePtr<CGfxRenderTexture> CGfxRenderTexturePtr;
typedef CGfxResourcePtr<CGfxUniformBuffer> CGfxUniformBufferPtr;

typedef CGfxResourcePtr<CGfxMesh> CGfxMeshPtr;
typedef CGfxResourcePtr<CGfxMeshDraw> CGfxMeshDrawPtr;
typedef CGfxResourcePtr<CGfxMaterial> CGfxMaterialPtr;

typedef CGfxResourcePtr<CGfxDescriptorSet> CGfxDescriptorSetPtr;
typedef CGfxResourcePtr<CGfxDescriptorLayout> CGfxDescriptorLayoutPtr;

typedef CGfxResourcePtr<CGfxCommandBuffer> CGfxCommandBufferPtr;
