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

#define CALL_VK_FUNCTION_RETURN_NULLPTR(func)    \
{                                                \
	VkResult err = func;                         \
	if (err != VK_SUCCESS) {                     \
		CVKInstance::SetLastError(err);          \
		ASSERT(false);                           \
		return nullptr;                          \
	}                                            \
}


class CVKRenderer;

class CVKAllocator;
class CVKInstance;
class CVKDevice;
class CVKQueue;

class CVKMemory;
class CVKMemoryAllocator;
class CVKMemoryManager;

class CVKSwapChain;
class CVKRenderPass;
class CVKRenderPassManager;
class CVKFrameBuffer;
class CVKFrameBufferManager;

class CVKShader;
class CVKShaderManager;

class CVKPipeline;
class CVKPipelineCompute;
class CVKPipelineComputeManager;
class CVKPipelineGraphics;
class CVKPipelineGraphicsManager;

class CVKTransferBuffer;
class CVKTransferManager;

class CVKBuffer;
class CVKIndexBuffer;
class CVKVertexBuffer;
class CVKInstanceBuffer;
class CVKIndirectBuffer;

class CVKUniformBuffer;
class CVKUniformBufferManager;

class CVKSampler;
class CVKSamplerManager;
class CVKTexture;
class CVKTexture2D;
class CVKTexture2DManager;
class CVKTexture2DArray;
class CVKTexture2DArrayManager;
class CVKTextureCubemap;
class CVKTextureCubemapManager;
class CVKRenderTexture;
class CVKRenderTextureManager;

class CVKMesh;
class CVKMeshManager;
class CVKMeshDraw;
class CVKMeshDrawManager;

class CVKMaterial;
class CVKMaterialPass;
class CVKMaterialManager;

class CVKDescriptorPool;
class CVKDescriptorSet;
class CVKDescriptorSetManager;
class CVKDescriptorLayout;
class CVKDescriptorLayoutManager;

class CVKCommandBuffer;
class CVKCommandBufferManager;
