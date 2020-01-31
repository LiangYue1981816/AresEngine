#pragma once
#include "PreHeader.h"

#include "GfxGLM.h"
#include "GfxEnum.h"
#include "GfxHelper.h"
#include "GfxDefinition.h"

#include "GfxProfiler.h"

#include "GfxSprivCross.h"
#include "GfxVertexAttribute.h"

#include "GfxResource.h"
#include "GfxResourceManager.h"

#include "GfxCamera.h"

#include "GfxSwapChain.h"
#include "GfxRenderPass.h"
#include "GfxRenderTexture.h"
#include "GfxFrameBuffer.h"

#include "GfxShader.h"
#include "GfxPipelineCompute.h"
#include "GfxPipelineGraphics.h"

#include "GfxIndexBuffer.h"
#include "GfxVertexBuffer.h"
#include "GfxInstanceBuffer.h"
#include "GfxIndirectBuffer.h"

#include "GfxUniformBuffer.h"
#include "GfxStorageBuffer.h"

#include "GfxSampler.h"
#include "GfxTexture2D.h"
#include "GfxTexture2DArray.h"
#include "GfxTextureCubemap.h"

#include "GfxUniformVec1.h"
#include "GfxUniformVec2.h"
#include "GfxUniformVec3.h"
#include "GfxUniformVec4.h"
#include "GfxUniformMat4.h"
#include "GfxUniformCamera.h"

#include "GfxMesh.h"
#include "GfxMeshDraw.h"

#include "GfxMaterial.h"
#include "GfxMaterialPass.h"

#include "GfxDescriptorSet.h"
#include "GfxDescriptorLayout.h"

#include "GfxSemaphore.h"
#include "GfxCommandBuffer.h"


#define DESCRIPTOR_SET_PASS                                0
#define DESCRIPTOR_SET_MATPASS                             1
#define DESCRIPTOR_SET_MESHDRAW                            2
#define DESCRIPTOR_SET_INPUTATTACHMENT                     3
#define DESCRIPTOR_SET_COUNT                               4


#define GfxRenderer() CGfxRenderer::GetInstance()


typedef enum GfxApi {
	GFX_API_GLES3 = 0,
	GFX_API_VULKAN
} GfxApi;


class CALL_API CGfxRenderer
{
public:
	static CGfxRenderer* GetInstance(void);


public:
	CGfxRenderer(void* hInstance, void* hWnd, void* hDC, int width, int height, GfxPixelFormat format);
	virtual ~CGfxRenderer(void);


public:
	virtual GfxApi GetAPI(void) const = 0;
	virtual const glm::mat4& GetBaseMatrix(void) const = 0;


public:
	virtual void SetShaderCachePath(const char* szPath) const;
	virtual void SetShaderIncludePath(const char* szPath) const;


public:
	virtual uint32_t GetLastError(void) const = 0;

public:
	virtual CGfxSwapChain* GetSwapChain(void) const = 0;

public:
	virtual bool IsSupportExtension(const char* extension) const = 0;

public:
	virtual CGfxShader* CreateShader(const char* szFileName, shader_kind kind) = 0;
	virtual CGfxPipelineCompute* CreatePipelineCompute(const CGfxShader* pComputeShader) = 0;
	virtual CGfxPipelineGraphics* CreatePipelineGraphics(const CGfxRenderPass* pRenderPass, const CGfxShader* pVertexShader, const CGfxShader* pFragmentShader, const PipelineState& state, int indexSubpass, int vertexBinding, int instanceBinding) = 0;
	virtual CGfxSampler* CreateSampler(GfxFilter minFilter, GfxFilter magFilter, GfxSamplerMipmapMode mipmapMode, GfxSamplerAddressMode addressMode) = 0;

public:
	virtual CGfxFrameBufferPtr NewFrameBuffer(int width, int height, int numAttachments) = 0;

	virtual CGfxRenderPassPtr GetRenderPass(uint32_t name) = 0;
	virtual CGfxRenderPassPtr NewRenderPass(uint32_t name, int numAttachments, int numSubpasses) = 0;

	virtual CGfxTexture2DPtr GetTexture2D(uint32_t name) = 0;
	virtual CGfxTexture2DPtr NewTexture2D(uint32_t name) = 0;
	virtual CGfxTexture2DPtr NewTexture2D(const char* szFileName, int baseLevel = 0, int numLevels = INT_MAX) = 0;

	virtual CGfxTexture2DArrayPtr GetTexture2DArray(uint32_t name) = 0;
	virtual CGfxTexture2DArrayPtr NewTexture2DArray(uint32_t name) = 0;
	virtual CGfxTexture2DArrayPtr NewTexture2DArray(const char* szFileName, int baseLevel = 0, int numLevels = INT_MAX) = 0;

	virtual CGfxTextureCubemapPtr GetTextureCubemap(uint32_t name) = 0;
	virtual CGfxTextureCubemapPtr NewTextureCubemap(uint32_t name) = 0;
	virtual CGfxTextureCubemapPtr NewTextureCubemap(const char* szFileName, int baseLevel = 0, int numLevels = INT_MAX) = 0;

	virtual CGfxRenderTexturePtr GetRenderTexture(uint32_t name) = 0;
	virtual CGfxRenderTexturePtr NewRenderTexture(uint32_t name) = 0;

	virtual CGfxUniformBufferPtr NewUniformBuffer(size_t size) = 0;
	virtual CGfxStorageBufferPtr NewStorageBuffer(size_t size) = 0;

	virtual CGfxInstanceBufferPtr NewInstanceBuffer(uint32_t instanceFormat, int instanceBinding) = 0;
	virtual CGfxMultiInstanceBufferPtr NewMultiInstanceBuffer(uint32_t instanceFormat, int instanceBinding, int count) = 0;

	virtual CGfxMeshPtr GetMesh(uint32_t name) = 0;
	virtual CGfxMeshPtr NewMesh(uint32_t name) = 0;
	virtual CGfxMeshPtr NewMesh(const char* szFileName, int vertexBinding) = 0;

	virtual CGfxMeshDrawPtr GetMeshDraw(uint32_t name) = 0;
	virtual CGfxMeshDrawPtr NewMeshDraw(uint32_t name, const CGfxMeshPtr ptrMesh, uint32_t nameDraw) = 0;

	virtual CGfxMaterialPtr GetMaterial(uint32_t name) = 0;
	virtual CGfxMaterialPtr NewMaterial(uint32_t name) = 0;
	virtual CGfxMaterialPtr NewMaterial(uint32_t name, const CGfxMaterialPtr ptrMaterialCopyFrom) = 0;
	virtual CGfxMaterialPtr NewMaterial(const char* szFileName, int vertexBinding, int instanceBinding, int baseLevel = 0, int numLevels = INT_MAX) = 0;

	virtual CGfxDescriptorLayoutPtr NewDescriptorLayout(uint32_t set) = 0;

	virtual CGfxDescriptorSetPtr GetDescriptorSet(uint32_t name) = 0;
	virtual CGfxDescriptorSetPtr NewDescriptorSet(uint32_t name, const CGfxDescriptorLayoutPtr ptrDescriptorLayout) = 0;
	virtual CGfxDescriptorSetPtr NewDescriptorSet(uint32_t name, const CGfxDescriptorSetPtr ptrDescriptorSetCopyFrom) = 0;
	virtual CGfxDescriptorSetPtr NewDescriptorSet(const CGfxPipelineGraphics* pPipelineGraphics, const CGfxFrameBuffer* pFrameBuffer, const CGfxRenderPass* pRenderPass, int indexSubpass) = 0;

	virtual CGfxCommandBufferPtr NewCommandBuffer(uint32_t pool, bool bMainCommandBuffer) = 0;

public:
	virtual void BeginRecord(CGfxCommandBufferPtr ptrCommandBuffer) = 0;
	virtual void BeginRecord(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxFrameBufferPtr ptrFrameBuffer, const CGfxRenderPassPtr ptrRenderPass, int indexSubpass) = 0;
	virtual void EndRecord(CGfxCommandBufferPtr ptrCommandBuffer) = 0;

	// Outside RenderPass
	virtual void CmdSetImageLayout(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxTexture2DPtr ptrTexture, GfxImageLayout imageLayout) = 0;
	virtual void CmdSetImageLayout(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxTexture2DArrayPtr ptrTexture, GfxImageLayout imageLayout) = 0;
	virtual void CmdSetImageLayout(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxTextureCubemapPtr ptrTexture, GfxImageLayout imageLayout) = 0;
	virtual void CmdSetImageLayout(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxRenderTexturePtr ptrTexture, GfxImageLayout imageLayout) = 0;
	virtual void CmdSetBufferBarrier(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxStorageBufferPtr ptrBuffer, GfxAccessFlags srcAccessFlags, GfxAccessFlags dstAccessFlags) = 0;

	// Inside RenderPass
	virtual void CmdBeginRenderPass(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxFrameBufferPtr ptrFrameBuffer, const CGfxRenderPassPtr ptrRenderPass) = 0;
	virtual void CmdNextSubpass(CGfxCommandBufferPtr ptrCommandBuffer) = 0;
	virtual void CmdEndRenderPass(CGfxCommandBufferPtr ptrCommandBuffer) = 0;

	virtual void CmdBindPipelineCompute(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxPipelineCompute* pPipelineCompute) = 0;
	virtual void CmdBindPipelineGraphics(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxPipelineGraphics* pPipelineGraphics) = 0;
	virtual void CmdBindDescriptorSet(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxDescriptorSetPtr ptrDescriptorSet) = 0;
	virtual void CmdBindIndexBuffer(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxIndexBufferPtr ptrIndexBuffer) = 0;
	virtual void CmdBindVertexBuffer(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxVertexBufferPtr ptrVertexBuffer) = 0;
	virtual void CmdBindInstanceBuffer(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxInstanceBufferPtr ptrInstanceBuffer) = 0;

	virtual void CmdSetIndexBuffer(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxIndexBufferPtr ptrIndexBuffer, const uint8_t* pBuffer, uint32_t size) = 0;
	virtual void CmdSetVertexBuffer(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxVertexBufferPtr ptrVertexBuffer, const uint8_t* pBuffer, uint32_t size) = 0;
	virtual void CmdSetInstanceBuffer(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxInstanceBufferPtr ptrInstanceBuffer, const uint8_t* pBuffer, uint32_t size) = 0;

	virtual void CmdUniform1i(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int v0) = 0;
	virtual void CmdUniform2i(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int v0, int v1) = 0;
	virtual void CmdUniform3i(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int v0, int v1, int v2) = 0;
	virtual void CmdUniform4i(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int v0, int v1, int v2, int v3) = 0;
	virtual void CmdUniform1f(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, float v0) = 0;
	virtual void CmdUniform2f(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, float v0, float v1) = 0;
	virtual void CmdUniform3f(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, float v0, float v1, float v2) = 0;
	virtual void CmdUniform4f(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, float v0, float v1, float v2, float v3) = 0;
	virtual void CmdUniform1iv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const int* value) = 0;
	virtual void CmdUniform2iv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const int* value) = 0;
	virtual void CmdUniform3iv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const int* value) = 0;
	virtual void CmdUniform4iv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const int* value) = 0;
	virtual void CmdUniform1fv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const float* value) = 0;
	virtual void CmdUniform2fv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const float* value) = 0;
	virtual void CmdUniform3fv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const float* value) = 0;
	virtual void CmdUniform4fv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const float* value) = 0;
	virtual void CmdUniformMatrix2fv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const float* value) = 0;
	virtual void CmdUniformMatrix3fv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const float* value) = 0;
	virtual void CmdUniformMatrix4fv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const float* value) = 0;

	virtual void CmdSetScissor(CGfxCommandBufferPtr ptrCommandBuffer, int x, int y, int width, int height) = 0;
	virtual void CmdSetViewport(CGfxCommandBufferPtr ptrCommandBuffer, int x, int y, int width, int height) = 0;

	virtual void CmdClearDepth(CGfxCommandBufferPtr ptrCommandBuffer, float depth) = 0;
	virtual void CmdClearColor(CGfxCommandBufferPtr ptrCommandBuffer, float red, float green, float blue, float alpha) = 0;

	virtual void CmdDrawInstance(CGfxCommandBufferPtr ptrCommandBuffer, int indexType, int indexOffset, int indexCount, int instanceCount) = 0;
	virtual void CmdDispatch(CGfxCommandBufferPtr ptrCommandBuffer, int numLocalWorkGroupX, int numLocalWorkGroupY, int numLocalWorkGroupZ) = 0;

	virtual void CmdPushDebugGroup(CGfxCommandBufferPtr ptrCommandBuffer, const char* szMessage) = 0;
	virtual void CmdPopDebugGroup(CGfxCommandBufferPtr ptrCommandBuffer) = 0;

	virtual void CmdExecute(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxCommandBufferPtr ptrSecondaryCommandBuffer) = 0;

	// Submit
	virtual void Submit(const CGfxCommandBufferPtr& ptrCommandBuffer, const CGfxSemaphore* pWaitSemaphore) = 0;

public:
	virtual void AcquireNextFrame(void) = 0;
	virtual void Present(const CGfxSemaphore* pWaitSemaphore) = 0;


protected:
	static CGfxRenderer* pInstance;
};
