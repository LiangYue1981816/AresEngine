#pragma once

class CALL_API CGfxRenderer;

class CALL_API CGfxProfiler;
class CALL_API CGfxProfilerSample;

class CALL_API CGfxCamera;
class CALL_API CGfxRenderQueue;

class CALL_API CGfxSwapChain;
class CALL_API CGfxRenderPass;
class CALL_API CGfxFrameBuffer;

class CALL_API CGfxMesh;
class CALL_API CGfxIndexBuffer;
class CALL_API CGfxVertexBuffer;
class CALL_API CGfxInstanceBuffer;
class CALL_API CGfxDrawIndirectBuffer;

class CALL_API CGfxSampler;
class CALL_API CGfxTextureBase;
class CALL_API CGfxTexture2D;
class CALL_API CGfxTexture2DArray;
class CALL_API CGfxTextureCubeMap;

class CALL_API CGfxShader;
class CALL_API CGfxPipelineBase;
class CALL_API CGfxPipelineCompute;
class CALL_API CGfxPipelineGraphics;

class CALL_API CGfxMaterial;
class CALL_API CGfxMaterialPass;

class CALL_API CGfxUniformBase;
class CALL_API CGfxUniformVec1;
class CALL_API CGfxUniformVec2;
class CALL_API CGfxUniformVec3;
class CALL_API CGfxUniformVec4;
class CALL_API CGfxUniformMat4;
class CALL_API CGfxUniformCamera;
class CALL_API CGfxUniformEngine;
class CALL_API CGfxUniformBuffer;

class CALL_API CGfxCommandBuffer;


template<class T>
class CALL_API CGfxResourcePtr;
class CALL_API CGfxResource;
class CALL_API CGfxResourceManager;

typedef CGfxResourcePtr<CGfxRenderPass> CGfxRenderPassPtr;
typedef CGfxResourcePtr<CGfxFrameBuffer> CGfxFrameBufferPtr;

typedef CGfxResourcePtr<CGfxMesh> CGfxMeshPtr;
typedef CGfxResourcePtr<CGfxMaterial> CGfxMaterialPtr;
typedef CGfxResourcePtr<CGfxTexture2D> CGfxTexture2DPtr;
typedef CGfxResourcePtr<CGfxTexture2DArray> CGfxTexture2DArrayPtr;
typedef CGfxResourcePtr<CGfxTextureCubeMap> CGfxTextureCubeMapPtr;

typedef CGfxResourcePtr<CGfxUniformEngine> CGfxUniformEnginePtr;
typedef CGfxResourcePtr<CGfxUniformCamera> CGfxUniformCameraPtr;

typedef CGfxResourcePtr<CGfxCommandBuffer> CGfxCommandBufferPtr;
