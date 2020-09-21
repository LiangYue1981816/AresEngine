#pragma once


#define CALL_VK_FUNCTION_ASSERT(func)            \
{                                                \
	VkResult err = func;                         \
	ASSERT(err == VK_SUCCESS);                   \
}

#define CALL_VK_FUNCTION_BREAK(func)             \
{                                                \
	VkResult err = func;                         \
	if (err != VK_SUCCESS) {                     \
		CVKInstance::SetLastError(err);          \
		ASSERT(false);                           \
		break;                                   \
	}                                            \
}

#define CALL_VK_FUNCTION_RETURN(func)            \
{                                                \
	VkResult err = func;                         \
	if (err != VK_SUCCESS) {                     \
		CVKInstance::SetLastError(err);          \
		ASSERT(false);                           \
		return;                                  \
	}                                            \
}

#define CALL_VK_FUNCTION_RETURN_BOOL(func)       \
{                                                \
	VkResult err = func;                         \
	if (err != VK_SUCCESS) {                     \
		CVKInstance::SetLastError(err);          \
		ASSERT(false);                           \
		return false;                            \
	}                                            \
}

#define CALL_VK_FUNCTION_RETURN_RESULT(func)     \
{                                                \
	VkResult err = func;                         \
	if (err != VK_SUCCESS) {                     \
		CVKInstance::SetLastError(err);          \
		ASSERT(false);                           \
		return err;                              \
	}                                            \
}

#define CALL_VK_FUNCTION_RETURN_NULLPTR(func)    \
{                                                \
	VkResult err = func;                         \
	if (err != VK_SUCCESS) {                     \
		CVKInstance::SetLastError(err);          \
		ASSERT(false);                           \
		return nullptr;                          \
	}                                            \
}


typedef VkSwapchainKHR VkSwapchain;
typedef VkSurfaceKHR VkSurface;


class CVKAllocator;
class CVKInstance;
class CVKDevice;
class CVKQueue;
class CVKMemory;
class CVKMemoryAllocator;
class CVKMemoryManager;
class CVKRenderer;
class CVKSwapChain;
class CVKRenderPass;
class CVKRenderTexture;
class CVKFrameBuffer;
class CVKShader;
class CVKPipeline;
class CVKPipelineCompute;
class CVKPipelineGraphics;
class CVKBuffer;
class CVKIndexBuffer;
class CVKVertexBuffer;
class CVKUniformBuffer;
class CVKStorageBuffer;
class CVKInstanceBuffer;
class CVKIndirectBuffer;
class CVKTransferBuffer;
class CVKSampler;
class CVKTexture;
class CVKTexture2D;
class CVKTexture2DArray;
class CVKTextureCubemap;
class CVKMesh;
class CVKMeshDraw;
class CVKMaterial;
class CVKMaterialPass;
class CVKDescriptorPool;
class CVKDescriptorSet;
class CVKDescriptorLayout;
class CVKSemaphore;
class CVKCommandBuffer;

class CVKFrameBufferManager;
class CVKRenderPassManager;
class CVKRenderTextureManager;
class CVKTexture2DManager;
class CVKTexture2DArrayManager;
class CVKTextureCubemapManager;
class CVKDescriptorSetManager;
