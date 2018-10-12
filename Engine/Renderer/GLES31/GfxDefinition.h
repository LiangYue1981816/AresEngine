#pragma once

class CGfxRenderer;
class CGfxCamera;

class CGfxCommandBase;
class CGfxCommandBeginPass;
class CGfxCommandBindCamera;
class CGfxCommandBindInputTexture;
class CGfxCommandBindMaterial;
class CGfxCommandBindMesh;
class CGfxCommandBuffer;
class CGfxCommandClearColor;
class CGfxCommandClearDepth;
class CGfxCommandDrawElements;
class CGfxCommandDrawIndirect;
class CGfxCommandDrawInstance;
class CGfxCommandEndPass;
class CGfxCommandExecute;
class CGfxCommandSetBlend;
class CGfxCommandSetColorWrite;
class CGfxCommandSetCullFace;
class CGfxCommandSetDepthTest;
class CGfxCommandSetDepthWrite;
class CGfxCommandSetPolygonOffset;
class CGfxCommandSetScissor;
class CGfxCommandSetViewport;

class CGfxIndexBuffer;
class CGfxInstanceBuffer;
class CGfxVertexBuffer;
class CGfxVertexArrayObject;

class CGfxRenderPass;
class CGfxRenderPassManager;

class CGfxFrameBuffer;
class CGfxFrameBufferManager;

class CGfxMesh;
class CGfxMeshManager;

class CGfxMaterial;
class CGfxMaterialPass;
class CGfxMaterialManager;

class CGfxShader;
class CGfxShaderManager;
class CGfxShaderCompiler;

class CGfxPipelineBase;
class CGfxPipelineCompute;
class CGfxPipelineGraphics;
class CGfxPipelineManager;

class CGfxSampler;
class CGfxSamplerManager;

class CGfxTextureBase;
class CGfxTexture2D;
class CGfxTexture2DArray;
class CGfxTextureCubeMap;
class CGfxTextureManager;

class CGfxUniformMat4;
class CGfxUniformVec1;
class CGfxUniformVec2;
class CGfxUniformVec3;
class CGfxUniformVec4;
class CGfxUniformBuffer;
class CGfxUniformCamera;
class CGfxUniformEngine;

class CGfxProfiler;
class CGfxProfilerSample;


template<class T>
class CGfxResourcePtr;
class CGfxResource;
class CGfxResourceManager;

typedef CGfxResourcePtr<CGfxRenderPass> CGfxRenderPassPtr;
typedef CGfxResourcePtr<CGfxFrameBuffer> CGfxFrameBufferPtr;
typedef CGfxResourcePtr<CGfxMesh> CGfxMeshPtr;
typedef CGfxResourcePtr<CGfxMaterial> CGfxMaterialPtr;
typedef CGfxResourcePtr<CGfxTexture2D> CGfxTexture2DPtr;
typedef CGfxResourcePtr<CGfxTexture2DArray> CGfxTexture2DArrayPtr;
typedef CGfxResourcePtr<CGfxTextureCubeMap> CGfxTextureCubeMapPtr;
