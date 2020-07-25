#pragma once


class CGfxProfiler;
class CGfxProfilerSample;

class CGfxResource;
class CGfxResourceManager;

class CGfxCamera;

class CGfxRenderer;

class CGfxSwapChain;
class CGfxRenderPass;
class CGfxRenderTexture;
class CGfxFrameBuffer;

class CGfxShader;
class CGfxPipelineCompute;
class CGfxPipelineGraphics;

class CGfxIndexBuffer;
class CGfxVertexBuffer;
class CGfxInstanceBuffer;
class CGfxIndirectBuffer;

class CGfxMultiIndexBuffer;
class CGfxMultiVertexBuffer;
class CGfxMultiIndirectBuffer;
class CGfxMultiInstanceBuffer;

class CGfxUniformBuffer;
class CGfxStorageBuffer;

class CGfxSampler;
class CGfxTexture2D;
class CGfxTexture2DArray;
class CGfxTextureCubemap;

class CGfxUniformVec1;
class CGfxUniformVec2;
class CGfxUniformVec3;
class CGfxUniformVec4;
class CGfxUniformMat4;

class CGfxMesh;
class CGfxMeshDraw;

class CGfxMaterial;
class CGfxMaterialPass;

class CGfxDescriptorSet;
class CGfxDescriptorLayout;

class CGfxSemaphore;
class CGfxCommandBuffer;

template<class T>
class CGfxResourcePtr;

typedef CGfxResourcePtr<CGfxRenderPass> CGfxRenderPassPtr;
typedef CGfxResourcePtr<CGfxRenderTexture> CGfxRenderTexturePtr;
typedef CGfxResourcePtr<CGfxFrameBuffer> CGfxFrameBufferPtr;

typedef CGfxResourcePtr<CGfxMesh> CGfxMeshPtr;
typedef CGfxResourcePtr<CGfxMeshDraw> CGfxMeshDrawPtr;
typedef CGfxResourcePtr<CGfxMaterial> CGfxMaterialPtr;

typedef CGfxResourcePtr<CGfxTexture2D> CGfxTexture2DPtr;
typedef CGfxResourcePtr<CGfxTexture2DArray> CGfxTexture2DArrayPtr;
typedef CGfxResourcePtr<CGfxTextureCubemap> CGfxTextureCubemapPtr;

typedef CGfxResourcePtr<CGfxIndexBuffer> CGfxIndexBufferPtr;
typedef CGfxResourcePtr<CGfxVertexBuffer> CGfxVertexBufferPtr;
typedef CGfxResourcePtr<CGfxInstanceBuffer> CGfxInstanceBufferPtr;
typedef CGfxResourcePtr<CGfxIndirectBuffer> CGfxIndirectBufferPtr;

typedef CGfxResourcePtr<CGfxMultiIndexBuffer> CGfxMultiIndexBufferPtr;
typedef CGfxResourcePtr<CGfxMultiVertexBuffer> CGfxMultiVertexBufferPtr;
typedef CGfxResourcePtr<CGfxMultiInstanceBuffer> CGfxMultiInstanceBufferPtr;
typedef CGfxResourcePtr<CGfxMultiIndirectBuffer> CGfxMultiIndirectBufferPtr;

typedef CGfxResourcePtr<CGfxUniformBuffer> CGfxUniformBufferPtr;
typedef CGfxResourcePtr<CGfxStorageBuffer> CGfxStorageBufferPtr;

typedef CGfxResourcePtr<CGfxDescriptorSet> CGfxDescriptorSetPtr;
typedef CGfxResourcePtr<CGfxDescriptorLayout> CGfxDescriptorLayoutPtr;

typedef CGfxResourcePtr<CGfxCommandBuffer> CGfxCommandBufferPtr;
