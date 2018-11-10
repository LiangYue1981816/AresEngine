#pragma once

class CGfxRenderer;

class CGfxProfiler;
class CGfxProfilerSample;

class CGfxCamera;
class CGfxRenderQueue;

class CGfxSwapChain;
class CGfxRenderPass;
class CGfxFrameBuffer;

class CGfxMesh;
class CGfxIndexBuffer;
class CGfxVertexBuffer;
class CGfxInstanceBuffer;
class CGfxDrawIndirectBuffer;

class CGfxSampler;
class CGfxTextureBase;
class CGfxTexture2D;
class CGfxTexture2DArray;
class CGfxTextureCubeMap;

class CGfxShader;
class CGfxPipelineBase;
class CGfxPipelineCompute;
class CGfxPipelineGraphics;

class CGfxMaterial;
class CGfxMaterialPass;

class CGfxUniformBase;
class CGfxUniformVec1;
class CGfxUniformVec2;
class CGfxUniformVec3;
class CGfxUniformVec4;
class CGfxUniformMat4;
class CGfxUniformCamera;
class CGfxUniformEngine;
class CGfxUniformBuffer;

class CGfxCommandBuffer;


template<class T>
class CGfxResourcePtr;
class CGfxResource;

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
