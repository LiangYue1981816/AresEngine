#pragma once

template<class T>
class CGfxResourcePtr;
class CGfxResource;
class CGfxCamera;
class CGfxCameraManager;
class CGfxCommandBase;
class CGfxCommandBuffer;
class CGfxFrameBuffer;
class CGfxFrameBufferManager;
class CGfxIndexBuffer;
class CGfxInstanceBuffer;
class CGfxMaterial;
class CGfxMaterialManager;
class CGfxMesh;
class CGfxMeshManager;
class CGfxProgram;
class CGfxProgramManager;
class CGfxRenderer;
class CGfxSampler;
class CGfxSamplerManager;
class CGfxTextureBase;
class CGfxTexture2D;
class CGfxTexture2DArray;
class CGfxTextureCubeMap;
class CGfxTextureManager;
class CGfxUniformBuffer;
class CGfxUniformCamera;
class CGfxUniformEngine;
class CGfxUniformMat4;
class CGfxUniformVec1;
class CGfxUniformVec2;
class CGfxUniformVec3;
class CGfxUniformVec4;
class CGfxVertexArrayObject;
class CGfxVertexBuffer;

typedef CGfxResourcePtr<CGfxFrameBuffer> CGfxFrameBufferPtr;
typedef CGfxResourcePtr<CGfxTexture2D> CGfxTexture2DPtr;
typedef CGfxResourcePtr<CGfxTexture2DArray> CGfxTexture2DArrayPtr;
typedef CGfxResourcePtr<CGfxTextureCubeMap> CGfxTextureCubeMapPtr;
typedef CGfxResourcePtr<CGfxMesh> CGfxMeshPtr;
typedef CGfxResourcePtr<CGfxMaterial> CGfxMaterialPtr;
