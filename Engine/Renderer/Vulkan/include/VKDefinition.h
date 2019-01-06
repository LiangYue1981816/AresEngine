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


class CVKRenderer;

class CVKAllocator;
class CVKInstance;
class CVKDevice;
class CVKQueue;
class CVKMemory;
class CVKMemoryAllocator;
class CVKMemoryManager;
class CVKBuffer;
class CVKIndexBuffer;
class CVKVertexBuffer;
class CVKInstanceBuffer;
class CVKIndirectBuffer;
class CVKImage;
class CVKSampler;
class CVKTexture;
class CVKTexture2D;
class CVKTexture2DArray;
class CVKTextureCubeMap;
class CVKRenderTexture;
class CVKDescriptorSetLayout;
class CVKShader;
class CVKPipeline;

class CVKMesh;
class CVKMeshDraw;
class CVKUniformBuffer;

class CVKSamplerManager;
class CVKTextureManager;
class CVKRenderTextureManager;
class CVKShaderManager;
class CVKMeshManager;
class CVKMeshDrawManager;
