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

#include "VKImage.h"

#include "VKSwapChain.h"
#include "VKRenderPass.h"
#include "VKRenderPassManager.h"
#include "VKFrameBuffer.h"
#include "VKFrameBufferManager.h"
#include "VKRenderTexture.h"
#include "VKRenderTextureManager.h"

#include "VKShader.h"
#include "VKShaderManager.h"
#include "VKPipeline.h"
#include "VKPipelineCompute.h"
#include "VKPipelineGraphics.h"
#include "VKPipelineManager.h"

#include "VKBuffer.h"
#include "VKIndexBuffer.h"
#include "VKVertexBuffer.h"
#include "VKInstanceBuffer.h"
#include "VKIndirectBuffer.h"

#include "VKSampler.h"
#include "VKSamplerManager.h"
#include "VKTexture.h"
#include "VKTexture2D.h"
#include "VKTexture2DArray.h"
#include "VKTextureCubeMap.h"
#include "VKTextureManager.h"

#include "VKUniformBuffer.h"
#include "VKUniformBufferManager.h"

#include "VKMesh.h"
#include "VKMeshManager.h"
#include "VKMeshDraw.h"
#include "VKMeshDrawManager.h"

#include "VKMaterial.h"
#include "VKMaterialPass.h"
#include "VKMaterialManager.h"

#include "VKDescriptorLayout.h"
#include "VKDescriptorPool.h"
#include "VKDescriptorSet.h"
#include "VKDescriptorSetManager.h"

#include "VKCommandPool.h"
#include "VKCommandBuffer.h"
#include "VKCommandBufferManager.h"


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
	CGfxMaterialPass* GetGlobalMaterialPass(void) const;
	CGfxMaterialPass* GetCurrentMaterialPass(void) const;

public:
	bool IsSupportExtension(const char *extension) const;

public:
	CGfxShader* CreateShader(const char *szFileName, shader_kind kind);
	CGfxPipelineCompute* CreatePipelineCompute(const CGfxShader *pComputeShader);
	CGfxPipelineGraphics* CreatePipelineGraphics(const CGfxRenderPass *pRenderPass, const CGfxShader *pVertexShader, const CGfxShader *pFragmentShader, const PipelineState &state, uint32_t indexSubpass, uint32_t vertexBinding = 0, uint32_t instanceBinding = 1);
	CGfxSampler* CreateSampler(GfxFilter minFilter, GfxFilter magFilter, GfxSamplerMipmapMode mipmapMode, GfxSamplerAddressMode addressMode);

public:
	CGfxFrameBufferPtr NewFrameBuffer(int width, int height, int numAttachments);

	CGfxRenderPassPtr GetRenderPass(uint32_t name);
	CGfxRenderPassPtr NewRenderPass(uint32_t name, int numAttachments, int numSubpasses);

	CGfxRenderTexturePtr GetRenderTexture(uint32_t name);
	CGfxRenderTexturePtr NewRenderTexture(uint32_t name);

	CGfxMeshPtr GetMesh(uint32_t name);
	CGfxMeshPtr NewMesh(uint32_t name);
	CGfxMeshPtr NewMesh(const char *szFileName, uint32_t vertexBinding = 0);

	CGfxMeshDrawPtr GetMeshDraw(uint32_t name);
	CGfxMeshDrawPtr NewMeshDraw(uint32_t name, const CGfxMeshPtr ptrMesh, int indexDraw, uint32_t instanceFormat, uint32_t instanceBinding = 1);

	CGfxMaterialPtr GetMaterial(uint32_t name);
	CGfxMaterialPtr NewMaterial(uint32_t name);
	CGfxMaterialPtr NewMaterial(const char *szFileName, uint32_t vertexBinding = 0, uint32_t instanceBinding = 1);

	CGfxTexture2DPtr GetTexture2D(uint32_t name);
	CGfxTexture2DPtr NewTexture2D(uint32_t name);
	CGfxTexture2DPtr NewTexture2D(const char *szFileName);

	CGfxTexture2DArrayPtr GetTexture2DArray(uint32_t name);
	CGfxTexture2DArrayPtr NewTexture2DArray(uint32_t name);
	CGfxTexture2DArrayPtr NewTexture2DArray(const char *szFileName);

	CGfxTextureCubeMapPtr GetTextureCubeMap(uint32_t name);
	CGfxTextureCubeMapPtr NewTextureCubeMap(uint32_t name);
	CGfxTextureCubeMapPtr NewTextureCubeMap(const char *szFileName);

	CGfxUniformBufferPtr NewUniformBuffer(size_t size);
	CGfxCommandBufferPtr NewCommandBuffer(uint32_t pool, bool bMainCommandBuffer);

public:
	bool CmdBeginRenderPass(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxFrameBufferPtr ptrFrameBuffer, const CGfxRenderPassPtr ptrRenderPass);
	bool CmdNextSubpass(CGfxCommandBufferPtr ptrCommandBuffer);
	bool CmdEndRenderPass(CGfxCommandBufferPtr ptrCommandBuffer);
	bool CmdBindPipelineCompute(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxPipelineCompute *pPipelineCompute);
	bool CmdBindPipelineGraphics(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxPipelineGraphics *pPipelineGraphics);
	bool CmdBindMaterialPass(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxMaterialPtr ptrMaterial, uint32_t namePass);
	bool CmdBindUniformBuffer(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxUniformBufferPtr ptrUniformBuffer, uint32_t nameUniform);
	bool CmdUniform1i(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, int v0);
	bool CmdUniform2i(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, int v0, int v1);
	bool CmdUniform3i(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, int v0, int v1, int v2);
	bool CmdUniform4i(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, int v0, int v1, int v2, int v3);
	bool CmdUniform1f(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, float v0);
	bool CmdUniform2f(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, float v0, float v1);
	bool CmdUniform3f(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, float v0, float v1, float v2);
	bool CmdUniform4f(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, float v0, float v1, float v2, float v3);
	bool CmdUniform1iv(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, int count, const int *value);
	bool CmdUniform2iv(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, int count, const int *value);
	bool CmdUniform3iv(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, int count, const int *value);
	bool CmdUniform4iv(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, int count, const int *value);
	bool CmdUniform1fv(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, int count, const float *value);
	bool CmdUniform2fv(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, int count, const float *value);
	bool CmdUniform3fv(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, int count, const float *value);
	bool CmdUniform4fv(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, int count, const float *value);
	bool CmdUniformMatrix2fv(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, int count, const float *value);
	bool CmdUniformMatrix3fv(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, int count, const float *value);
	bool CmdUniformMatrix4fv(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, int count, const float *value);
	bool CmdClearDepth(CGfxCommandBufferPtr ptrCommandBuffer, float depth);
	bool CmdClearColor(CGfxCommandBufferPtr ptrCommandBuffer, float red, float green, float blue, float alpha);
	bool CmdSetScissor(CGfxCommandBufferPtr ptrCommandBuffer, int x, int y, int width, int height);
	bool CmdSetViewport(CGfxCommandBufferPtr ptrCommandBuffer, int x, int y, int width, int height);
	bool CmdSetInstanceBufferData(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxMeshDrawPtr ptrMeshDraw, const uint8_t *pInstanceBuffer, uint32_t size);
	bool CmdDrawInstance(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxMeshDrawPtr ptrMeshDraw);
	bool CmdDrawIndirect(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxMeshDrawPtr ptrMeshDraw);
	bool CmdDrawScreen(CGfxCommandBufferPtr ptrCommandBuffer);
	bool CmdExecute(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxCommandBufferPtr ptrSecondaryCommandBuffer);
	bool CmdPresent(CGfxCommandBufferPtr ptrCommandBuffer);
	bool CmdPushDebugGroup(CGfxCommandBufferPtr ptrCommandBuffer, const char *szMessage);
	bool CmdPopDebugGroup(CGfxCommandBufferPtr ptrCommandBuffer);

public:
	void Submit(const eastl::vector<CGfxCommandBufferPtr> &ptrCommandBuffers);
	void AcquireNextFrame(void);
	void Present(void);


private:
	CVKInstance *m_pInstance;
	CVKDevice *m_pDevice;
	CVKSwapChain *m_pSwapChain;

private:
	CVKShaderManager *m_pShaderManager;
	CVKPipelineComputeManager *m_pPipelineComputeManager;
	CVKPipelineGraphicsManager *m_pPipelineGraphicsManager;
	CVKSamplerManager *m_pSamplerManager;
	CVKTexture2DManager *m_pTexture2DManager;
	CVKTexture2DArrayManager *m_pTexture2DArrayManager;
	CVKTextureCubeMapManager *m_pTextureCubeMapManager;
	CVKUniformBufferManager *m_pUniformBufferManager;
	CVKMeshManager *m_pMeshManager;
	CVKMeshDrawManager *m_pMeshDrawManager;
	CVKMaterialManager *m_pMaterialManager;
	CVKRenderPassManager *m_pRenderPassManager;
	CVKRenderTextureManager *m_pRenderTextureManager;
	CVKFrameBufferManager *m_pFrameBufferManager;
	CVKCommandBufferManager *m_pCommandBufferManager;

private:
	CVKMaterialPass *m_pGlobalMaterialPass;
	CVKMaterialPass *m_pCurrentMaterialPass;
	CVKPipelineCompute *m_pCurrentPipelineCompute;
	CVKPipelineGraphics *m_pCurrentPipelineGraphics;
};
