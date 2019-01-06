#pragma once
#include "GfxHeader.h"

#include "Vulkan/Vulkan.h"
#include "VKHelper.h"
#include "VKExtension.h"
#include "VKDefinition.h"

#include "VKAllocator.h"
#include "VKInstance.h"
#include "VKDevice.h"
#include "VKQueue.h"
#include "VKMemory.h"
#include "VKMemoryAllocator.h"
#include "VKMemoryManager.h"
#include "VKBuffer.h"
#include "VKIndexBuffer.h"
#include "VKVertexBuffer.h"
#include "VKInstanceBuffer.h"
#include "VKIndirectBuffer.h"
#include "VKImage.h"
#include "VKSampler.h"
#include "VKTexture.h"
#include "VKTexture2D.h"
#include "VKTexture2DArray.h"
#include "VKTextureCubeMap.h"
#include "VKRenderTexture.h"
#include "VkDescriptorSet.h"
#include "VKShader.h"
#include "VKPipeline.h"

#include "VKMesh.h"
#include "VKMeshDraw.h"
#include "VKUniformBuffer.h"

#include "VKSamplerManager.h"
#include "VKTextureManager.h"
#include "VKRenderTextureManager.h"
#include "VKShaderManager.h"
#include "VKMeshManager.h"
#include "VKMeshDrawManager.h"


#define VKRenderer() ((CVKRenderer *)CGfxRenderer::GetInstance())


class CALL_API CVKRenderer : public CGfxRenderer
{
public:
	CVKRenderer(void *hInstance, void *hWnd, void *hDC, int width, int height, GfxPixelFormat pixelFormat);
	virtual ~CVKRenderer(void);


public:
	uint32_t GetLastError(void) const;

public:
	CGfxSwapChain* GetSwapChain(void) const;
	CGfxMaterialPass* GetGlobalPass(void) const;

public:
	CGfxShader* CreateShader(const char *szFileName, shader_kind kind);
	CGfxPipelineCompute* CreatePipelineCompute(const CGfxShader *pComputeShader);
	CGfxPipelineGraphics* CreatePipelineGraphics(const CGfxShader *pVertexShader, const CGfxShader *pFragmentShader, const PipelineState &state);
	CGfxSampler* CreateSampler(int mipLevels, GfxFilter minFilter, GfxFilter magFilter, GfxSamplerMipmapMode mipmapMode, GfxSamplerAddressMode addressMode);

public:
	CGfxRenderPassPtr NewRenderPass(int numAttachments, int numSubpasses);
	CGfxFrameBufferPtr NewFrameBuffer(int width, int height);

	bool IsHaveMesh(uint32_t name);
	CGfxMeshPtr NewMesh(uint32_t name);
	CGfxMeshPtr NewMesh(const char *szFileName, uint32_t vertexBinding = 0);

	bool IsHaveMeshDraw(uint32_t name);
	CGfxMeshDrawPtr NewMeshDraw(uint32_t name, const CGfxMeshPtr &ptrMesh, int indexDraw, uint32_t instanceFormat, uint32_t instanceBinding = 1);

	bool IsHaveMaterial(uint32_t name);
	CGfxMaterialPtr NewMaterial(uint32_t name);
	CGfxMaterialPtr NewMaterial(const char *szFileName);

	bool IsHaveTexture2D(uint32_t name);
	bool IsHaveTexture2DArray(uint32_t name);
	bool IsHaveTextureCubeMap(uint32_t name);
	CGfxTexture2DPtr NewTexture2D(uint32_t name);
	CGfxTexture2DPtr NewTexture2D(const char *szFileName);
	CGfxTexture2DArrayPtr NewTexture2DArray(uint32_t name);
	CGfxTexture2DArrayPtr NewTexture2DArray(const char *szFileName);
	CGfxTextureCubeMapPtr NewTextureCubeMap(uint32_t name);
	CGfxTextureCubeMapPtr NewTextureCubeMap(const char *szFileName);

	bool IsHaveRenderTexture(uint32_t name);
	CGfxRenderTexturePtr NewRenderTexture(uint32_t name);

	CGfxUniformBufferPtr NewUniformBuffer(size_t size);
	CGfxCommandBufferPtr NewCommandBuffer(bool bMainCommandBuffer);

public:
	bool CmdBeginRenderPass(CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxFrameBufferPtr &ptrFrameBuffer, const CGfxRenderPassPtr &ptrRenderPass);
	bool CmdNextSubpass(CGfxCommandBufferPtr &ptrCommandBuffer);
	bool CmdEndRenderPass(CGfxCommandBufferPtr &ptrCommandBuffer);

	bool CmdBindPipelineCompute(CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxPipelineCompute *pPipelineCompute);
	bool CmdBindPipelineGraphics(CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxPipelineGraphics *pPipelineGraphics);
	bool CmdBindMaterialPass(CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxMaterialPtr &ptrMaterial, uint32_t namePass);
	bool CmdBindUniformBuffer(CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxUniformBufferPtr &ptrUniformBuffer, uint32_t nameUniform);

	bool CmdUniform1i(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int v0);
	bool CmdUniform2i(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int v0, int v1);
	bool CmdUniform3i(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int v0, int v1, int v2);
	bool CmdUniform4i(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int v0, int v1, int v2, int v3);
	bool CmdUniform1f(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, float v0);
	bool CmdUniform2f(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, float v0, float v1);
	bool CmdUniform3f(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, float v0, float v1, float v2);
	bool CmdUniform4f(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, float v0, float v1, float v2, float v3);
	bool CmdUniform1iv(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int count, const int *value);
	bool CmdUniform2iv(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int count, const int *value);
	bool CmdUniform3iv(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int count, const int *value);
	bool CmdUniform4iv(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int count, const int *value);
	bool CmdUniform1fv(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int count, const float *value);
	bool CmdUniform2fv(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int count, const float *value);
	bool CmdUniform3fv(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int count, const float *value);
	bool CmdUniform4fv(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int count, const float *value);
	bool CmdUniformMatrix2fv(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int count, const float *value);
	bool CmdUniformMatrix3fv(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int count, const float *value);
	bool CmdUniformMatrix4fv(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int count, const float *value);

	bool CmdClearDepth(CGfxCommandBufferPtr &ptrCommandBuffer, float depth);
	bool CmdClearColor(CGfxCommandBufferPtr &ptrCommandBuffer, float red, float green, float blue, float alpha);

	bool CmdSetScissor(CGfxCommandBufferPtr &ptrCommandBuffer, int x, int y, int width, int height);
	bool CmdSetViewport(CGfxCommandBufferPtr &ptrCommandBuffer, int x, int y, int width, int height);

	bool CmdSetInstanceBufferData(CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxMeshDrawPtr &ptrMeshDraw, const uint8_t *pInstanceBuffer, uint32_t size);

	bool CmdDrawInstance(CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxMeshDrawPtr &ptrMeshDraw, int instanceCount);
	bool CmdDrawIndirect(CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxMeshDrawPtr &ptrMeshDraw);
	bool CmdDrawScreen(CGfxCommandBufferPtr &ptrCommandBuffer);

	bool CmdExecute(CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxCommandBufferPtr &ptrSecondaryCommandBuffer);

	bool CmdPresent(CGfxCommandBufferPtr &ptrCommandBuffer);

	bool CmdPushDebugGroup(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szMessage);
	bool CmdPopDebugGroup(CGfxCommandBufferPtr &ptrCommandBuffer);

public:
	void Submit(const CGfxCommandBufferPtr &ptrCommandBuffer);
	void Present(void);


private:
	CVKInstance *m_pInstance;
	CVKDevice *m_pDevice;
};
