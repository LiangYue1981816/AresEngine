#pragma once


#define CALL_BOOL_FUNCTION_BREAK(func)       \
{                                            \
	bool err = func;                         \
	if (err != true) {                       \
		break;                               \
	}                                        \
}

#define CALL_BOOL_FUNCTION_RETURN(func)      \
{                                            \
	bool err = func;                         \
	if (err != true) {                       \
		return;                              \
	}                                        \
}

#define CALL_BOOL_FUNCTION_RETURN_BOOL(func) \
{                                            \
	bool err = func;                         \
	if (err != true) {                       \
		return false;                        \
	}                                        \
}

#define CALL_BOOL_FUNCTION_RETURN_NULLPTR(func) \
{                                            \
	bool err = func;                         \
	if (err != true) {                       \
		return nullptr;                      \
	}                                        \
}

#define CALL_VK_FUNCTION_BREAK(func)         \
{                                            \
	VkResult err = func;                     \
	if (err != VK_SUCCESS) {                 \
		CVKInstance::SetLastError(err);      \
		break;                               \
	}                                        \
}

#define CALL_VK_FUNCTION_RETURN(func)        \
{                                            \
	VkResult err = func;                     \
	if (err != VK_SUCCESS) {                 \
		CVKInstance::SetLastError(err);      \
		return;                              \
	}                                        \
}

#define CALL_VK_FUNCTION_RETURN_BOOL(func)   \
{                                            \
	VkResult err = func;                     \
	if (err != VK_SUCCESS) {                 \
		CVKInstance::SetLastError(err);      \
		return false;                        \
	}                                        \
}

#define CALL_VK_FUNCTION_RETURN_NULLPTR(func)\
{                                            \
	VkResult err = func;                     \
	if (err != VK_SUCCESS) {                 \
		CVKInstance::SetLastError(err);      \
		return nullptr;                      \
	}                                        \
}


class CVKRenderer;

class CVKAllocator;

class CVKInstance;
class CVKDevice;
class CVKQueue;

class CVKMemory;
class CVKMemoryAllocator;
class CVKMemoryManager;

class CVKImage;

class CVKSwapChain;
class CVKRenderPass;
class CVKRenderPassManager;
class CVKFrameBuffer;
class CVKFrameBufferManager;
class CVKRenderTexture;
class CVKRenderTextureManager;

class CVKShader;
class CVKShaderManager;
class CVKPipeline;
class CVKPipelineCompute;
class CVKPipelineComputeManager;
class CVKPipelineGraphics;
class CVKPipelineGraphicsManager;

class CVKBuffer;
class CVKIndexBuffer;
class CVKVertexBuffer;
class CVKInstanceBuffer;
class CVKIndirectBuffer;

class CVKSampler;
class CVKSamplerManager;
class CVKTexture;
class CVKTexture2D;
class CVKTexture2DManager;
class CVKTexture2DArray;
class CVKTexture2DArrayManager;
class CVKTextureCubeMap;
class CVKTextureCubeMapManager;

class CVKMesh;
class CVKMeshManager;
class CVKMeshDraw;
class CVKMeshDrawManager;

class CVKMaterial;
class CVKMaterialPass;
class CVKMaterialManager;

class CVKUniformBuffer;
class CVKUniformBufferManager;

class CVKDescriptorLayout;
class CVKDescriptorPool;
class CVKDescriptorSet;
class CVKDescriptorSetManager;

class CVKCommandBuffer;
class CVKCommandBufferPool;
class CVKCommandBufferManager;
